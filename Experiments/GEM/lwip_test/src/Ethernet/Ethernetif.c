#include <stdio.h>
#include <stdlib.h>
#include "xparameters.h"
#include "xil_printf.h"
#include "xemacps.h"
#include "xemacps_hw.h"
#include "xstatus.h"
#include "xil_mmu.h"
#include "xil_cache.h"
#include "xscugic.h"

#include "sleep.h"

#include "lwip/netif.h"
#include "netif/ethernet.h"
#include "lwip/ip.h"
#include "lwip/pbuf.h"

#include "Ethernetif.h"

EMACPS_device_struct *EMACPS_device_ptr;
EMACPS_device_struct EMACPS_device;
XScuGic EMACPS_GIC;
u32 GemVersion;

u8 EMACps_bdmem[0x100000] __attribute__ ((aligned (0x100000)));
u8 EMACps_Rxbuf[MAX_RX_BUF_DESC][0x800] __attribute__ ((aligned (0x10000)));

u8 *EMACps_RxBDPtr;
u8 *EMACps_TxBDPtr;

u32 nRxFrames;
u32 nTxFrames;

u8 dstaddr[8] = {0x6c,0x4B,0x90,0x18,0xc2,0x59};

struct pbuf Mainpbuf;


static err_t EMACps_init(struct netif *netif);
static void EMACps_Reset(EMACPS_device_struct *devicePtr);
static void EMACpd_Stop(EMACPS_device_struct *devicePtr);
static void EMACps_Config(EMACPS_device_struct *devicePtr);
static void EMACps_ClkSetup(EMACPS_device_struct *devicePtr, u16 EmacPsIntrId);
static long EMACps_ConfigPHY(EMACPS_device_struct *devicePtr);
static u32 EMACps_DetectPHY(EMACPS_device_struct *devicePtr);
static long EMACps_PhyRead(EMACPS_device_struct *devicePtr, u32 PhyAddress,
                           u32 RegisterNum, u16 *PhyDataPtr);
static void EMACps_InitBufDesc(EMACPS_device_struct *devicePtr);
static void EMACps_devStart(EMACPS_device_struct *devicePtr);
static LONG EMACps_SetupIntrSystem(XScuGic *IntcInstancePtr,
                                   EMACPS_device_struct *devicePtr,
                                   u16 EmacPsIntrId);
static void EMACps_RxHandler(void *CallBackRef);
static void EMACps_IntrHandler(void *deviceptr);
static err_t EMACps_datasend(struct netif *netif, struct pbuf *p);
static err_t EMACps_datasend_ip(struct netif *netif, struct pbuf *p, const ip4_addr_t *ipaddr);

struct netif *EMAC_add(struct netif *netif,
                       ip_addr_t *ipaddr, ip_addr_t *netmask, ip_addr_t *gw,
                       const char *macaddr)
{
   EMACPS_device.config.DeviceId = XPAR_XEMACPS_0_DEVICE_ID;
   EMACPS_device.config.BaseAddress = XPAR_XEMACPS_0_BASEADDR;
   EMACPS_device.config.IsCacheCoherent = XPAR_XEMACPS_0_IS_CACHE_COHERENT;

   EMACPS_device.macaddrlen = 6;
   netif->hwaddr_len = 6;
   for (int i = 0; i < 6; i++)
   {
      netif->hwaddr[i] = MAC_ADDRESS[i];
      EMACPS_device.macaddr[i] = MAC_ADDRESS[i];
   }

   netif->name[0] = 'E';
   netif->name[1] = '0';

   return netif_add(netif, ipaddr, netmask, gw,
                    (void*)&EMACPS_device,
                    EMACps_init,
                    ethernet_input);
}

struct netif *EMAC_Read(struct netif *netif)
{
   u32 bufstate = 0;
   u32 bufflags;
   u32 bufadd;
   EMACPS_device_struct *devicePtr = netif->state;
   struct pbuf *p;

   bufstate = *((u32 *)devicePtr->RxbdRing.PresentAdd);
   bufflags = *((u32 *)devicePtr->RxbdRing.PresentAdd + 1);

   if(bufstate & RBD_RDY_BIT_MASK)
   {
      nRxFrames = nRxFrames + 1;
      printf("Frames Received: %ld\nFrames present add: %lx\n",nRxFrames,devicePtr->RxbdRing.PresentAdd);
      bufadd = bufstate & ~(RBD_RDY_BIT_MASK | RBD_WRAP_BIT_MASK);
      printf("Buf1: %08lx\nBuf2: %08lx\n",bufstate,bufflags);

      p = pbuf_alloc(PBUF_LINK, bufflags & RBD_DATA_MASK, PBUF_POOL);

      if (p == NULL)
      {
         printf("etharp_raw: could not allocate pbuf for ARP request.\n");
         return 0;
      }
      else
      {

         p->next = NULL;
         p->payload = (void *)bufadd;
         p->len = bufflags & RBD_DATA_MASK;
         p->tot_len = bufflags & RBD_DATA_MASK;

         Xil_DCacheFlushRange((UINTPTR)p->payload, p->len);

         netif->input(p, netif);
      }

      if(devicePtr->RxbdRing.PresentAdd == devicePtr->RxbdRing.LastAdd)
      {
         bufadd = (bufadd | RBD_WRAP_BIT_MASK) & ~RBD_RDY_BIT_MASK;
         ((u32 *)devicePtr->RxbdRing.PresentAdd)[0] = bufadd;
         ((u32 *)devicePtr->RxbdRing.PresentAdd)[1] = 0;

         devicePtr->RxbdRing.PresentAdd = devicePtr->RxbdRing.StartAdd;
      }
      else
      {
         bufadd = bufadd & ~(RBD_RDY_BIT_MASK | RBD_WRAP_BIT_MASK);
         ((u32 *)devicePtr->RxbdRing.PresentAdd)[0] = bufadd ;
         ((u32 *)devicePtr->RxbdRing.PresentAdd)[1] = 0;

         devicePtr->RxbdRing.PresentAdd = devicePtr->RxbdRing.PresentAdd + 8;
      }

      printf("\n\n\n\n\n\n");
      pbuf_free(p);
      p = NULL;
   }

   return ERR_OK;

}/* End of EMAC_Read */
/************************* Static Members *******************************/
static err_t EMACps_init(struct netif *netif)
{
   netif->flags = NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;
   netif->linkoutput = EMACps_datasend;
   netif->output = EMACps_datasend_ip;

   /* Assign Pointer */
   EMACPS_device_ptr = netif->state;

   /* Initialize device */
   EMACps_Reset(EMACPS_device_ptr);

   /* Read GemVersion */
   GemVersion = ((Xil_In32(EMACPS_device_ptr->config.BaseAddress + 0xFC)) >> 16) & 0xFFF;

   /* Config device */
   EMACps_Config(EMACPS_device_ptr);

   nRxFrames = 0;
   nTxFrames = 0;

   EMACPS_device_ptr->recv_handler = EMACps_RxHandler;

   /* Setup interrupts */
   EMACps_SetupIntrSystem(&EMACPS_GIC, EMACPS_device_ptr, EMACPS_IRPT_INTR);

   /* Start device */
   EMACps_devStart(EMACPS_device_ptr);

   return ERR_OK;

}/* End of EMACps_init */

static void EMACps_Reset(EMACPS_device_struct *devicePtr)
{
   EMACpd_Stop(devicePtr);

   /* Clear network control register */
   XEmacPs_WriteReg(devicePtr->config.BaseAddress,
               XEMACPS_NWCTRL_OFFSET, 0);

   /* Clear Statistic registers */
   XEmacPs_WriteReg(devicePtr->config.BaseAddress,
               XEMACPS_NWCTRL_OFFSET, XEMACPS_NWCTRL_STATCLR_MASK);

   /* Clear RX Status registers */
   XEmacPs_WriteReg(devicePtr->config.BaseAddress,
               XEMACPS_RXSR_OFFSET, 0x0000000F);

   /* Clear TX Status registers */
   XEmacPs_WriteReg(devicePtr->config.BaseAddress,
               XEMACPS_TXSR_OFFSET, 0x000000FF);

   /* Clear buffer Queues */
   XEmacPs_WriteReg(devicePtr->config.BaseAddress,
               XEMACPS_TXSR_OFFSET, 0x000000FF);
}/* End of EMACps_Reset */

static void EMACpd_Stop(EMACPS_device_struct *devicePtr)
{
   u32 Reg;

   /* Disable all interrupts */
   XEmacPs_WriteReg(devicePtr->config.BaseAddress, XEMACPS_IDR_OFFSET,
            XEMACPS_IXR_ALL_MASK);

   /* Disable the receiver & transmitter */
   Reg = XEmacPs_ReadReg(devicePtr->config.BaseAddress,
            XEMACPS_NWCTRL_OFFSET);
   Reg &= (u32)(~XEMACPS_NWCTRL_RXEN_MASK);
   Reg &= (u32)(~XEMACPS_NWCTRL_TXEN_MASK);
   XEmacPs_WriteReg(devicePtr->config.BaseAddress,
            XEMACPS_NWCTRL_OFFSET, Reg);

}/* End of EMACpd_Stop */

static void EMACps_Config(EMACPS_device_struct *devicePtr)
{
   u32 Reg;

   /* Update Net config register */
   Reg = XEMACPS_NWCFG_RXCHKSUMEN_MASK +
         XEMACPS_NWCFG_FCSREM_MASK +
         XEMACPS_NWCFG_LENERRDSCRD_MASK +
         XEMACPS_NWCFG_PAUSEEN_MASK +
         XEMACPS_NWCFG_1000_MASK +
         XEMACPS_NWCFG_UCASTHASHEN_MASK +
         XEMACPS_NWCFG_FDEN_MASK;

   /* MDIO Clock */
   Reg &= (u32)(~XEMACPS_NWCFG_MDCCLKDIV_MASK);
   Reg |= ((u32)(MDC_DIV_224) << XEMACPS_NWCFG_MDC_SHIFT_MASK);

   /* Load Net config register */
   XEmacPs_WriteReg(devicePtr->config.BaseAddress,
               XEMACPS_NWCFG_OFFSET, Reg);

   /* Setup clock */
   EMACps_ClkSetup(devicePtr, XPS_GEM0_INT_ID);

   /* Set MAC address */
   Reg = ((u32)devicePtr->macaddr[3]) << 24 |
         ((u32)devicePtr->macaddr[2]) << 16 |
         ((u32)devicePtr->macaddr[1]) << 8 |
         ((u32)devicePtr->macaddr[0]);
   XEmacPs_WriteReg(devicePtr->config.BaseAddress,
               XEMACPS_LADDR1L_OFFSET, Reg);
   Reg = 0;
   Reg = ((u32)devicePtr->macaddr[5]) << 8 |
         ((u32)devicePtr->macaddr[4]);
   XEmacPs_WriteReg(devicePtr->config.BaseAddress,
               XEMACPS_LADDR1H_OFFSET, Reg);

   /* DMA settings */
   Reg = (((u32)0x19) << XEMACPS_DMACR_RXBUF_SHIFT) +
         XEMACPS_DMACR_TCPCKSUM_MASK +
         XEMACPS_DMACR_TXSIZE_MASK +
         XEMACPS_DMACR_RXSIZE_MASK;
   XEmacPs_WriteReg(devicePtr->config.BaseAddress,
               XEMACPS_DMACR_OFFSET, Reg);

   /* Load Net control register */
   XEmacPs_WriteReg(devicePtr->config.BaseAddress,
               XEMACPS_NWCTRL_OFFSET, XEMACPS_NWCTRL_MDEN_MASK);

   /* Config Phy */
   EMACps_ConfigPHY(devicePtr);

   /* Initialise BD */
   EMACps_InitBufDesc(devicePtr);
}/* End of EMACps_Config */

static void EMACps_ClkSetup(EMACPS_device_struct *devicePtr, u16 EmacPsIntrId)
{
   u32 ClkCntrl;
   if (GemVersion == 2)
   {
      /* SLCR unlock */
      *(volatile unsigned int *)(SLCR_UNLOCK_ADDR) = SLCR_UNLOCK_KEY_VALUE;

      if (EmacPsIntrId == XPS_GEM0_INT_ID)
      {
         #ifdef XPAR_PS7_ETHERNET_0_ENET_SLCR_1000MBPS_DIV0
         /* GEM0 1G clock configuration*/
         ClkCntrl = *(volatile unsigned int *)(SLCR_GEM0_CLK_CTRL_ADDR);
         ClkCntrl &= EMACPS_SLCR_DIV_MASK;
         ClkCntrl |= (XPAR_PS7_ETHERNET_0_ENET_SLCR_1000MBPS_DIV1 << 20);
         ClkCntrl |= (XPAR_PS7_ETHERNET_0_ENET_SLCR_1000MBPS_DIV0 << 8);
         *(volatile unsigned int *)(SLCR_GEM0_CLK_CTRL_ADDR) = ClkCntrl;
         #endif
      }

      /* SLCR lock */
      *(unsigned int *)(SLCR_LOCK_ADDR) = SLCR_LOCK_KEY_VALUE;
      sleep(1);
   }
}/* End of EMACps_ClkSetup */

static long EMACps_ConfigPHY(EMACPS_device_struct *devicePtr)
{
   u32 PhyAddr;
   long Status;
   u16 PhyIdentity;

   /* Detect PHY */
   PhyAddr = EMACps_DetectPHY(devicePtr);

   if (PhyAddr >= 32)
   {
      print("Error detect phy");
      return XST_FAILURE;
   }

   Status = EMACps_PhyRead(devicePtr, PhyAddr, PHY_DETECT_REG1, &PhyIdentity);

   return Status;
}/* End of EMACps_ConfigPHY */

static u32 EMACps_DetectPHY(EMACPS_device_struct *devicePtr)
{
   u32 PhyAddr;
   u32 Status;
   u16 PhyReg1;
   u16 PhyReg2;

   for (PhyAddr = 0; PhyAddr <= 31; PhyAddr++)
   {
      Status = EMACps_PhyRead(devicePtr, PhyAddr,
                               PHY_DETECT_REG1, &PhyReg1);

      Status |= EMACps_PhyRead(devicePtr, PhyAddr,
                                PHY_DETECT_REG2, &PhyReg2);

      if ((Status == XST_SUCCESS) &&
          (PhyReg1 > 0x0000) && (PhyReg1 < 0xffff) &&
          (PhyReg2 > 0x0000) && (PhyReg2 < 0xffff))
      {
         /* Found a valid PHY address */
         return PhyAddr;
      }
   }
   return PhyAddr;      /* default to 32(max of iteration) */
}/* End of EMACps_DetectPHY */

static long EMACps_PhyRead(EMACPS_device_struct *devicePtr, u32 PhyAddress,
                    u32 RegisterNum, u16 *PhyDataPtr)
{
   u32 Mgtcr;
   volatile u32 Ipisr;
   u32 IpReadTemp;
   long Status;
   u32 MDIOState;

   /* Make sure no other PHY operation is currently in progress */
   MDIOState = XEmacPs_ReadReg(devicePtr->config.BaseAddress, XEMACPS_NWSR_OFFSET);
   if ((!(MDIOState & XEMACPS_NWSR_MDIOIDLE_MASK))==TRUE)
   {
      Status = (LONG)(XST_EMAC_MII_BUSY);
   }
   else
   {
      /* Construct Mgtcr mask for the operation */
      Mgtcr = XEMACPS_PHYMNTNC_OP_MASK | XEMACPS_PHYMNTNC_OP_R_MASK |
            (PhyAddress << XEMACPS_PHYMNTNC_PHAD_SHFT_MSK) |
            (RegisterNum << XEMACPS_PHYMNTNC_PREG_SHFT_MSK);

      /* Write Mgtcr and wait for completion */
      XEmacPs_WriteReg(devicePtr->config.BaseAddress,
               XEMACPS_PHYMNTNC_OFFSET, Mgtcr);

      do
      {
         Ipisr = XEmacPs_ReadReg(devicePtr->config.BaseAddress,XEMACPS_NWSR_OFFSET);
         IpReadTemp = Ipisr;

      } while ((IpReadTemp & XEMACPS_NWSR_MDIOIDLE_MASK) == 0x00000000U);

      /* Read data */
      *PhyDataPtr = (u16)XEmacPs_ReadReg(devicePtr->config.BaseAddress,
                                         XEMACPS_PHYMNTNC_OFFSET);
      Status = (LONG)(XST_SUCCESS);
   }
   return Status;

}/* End of EMACps_PhyRead */

static void EMACps_InitBufDesc(EMACPS_device_struct *devicePtr)
{
   int i;

   /*
    * The BDs need to be allocated in uncached memory. Hence the 1 MB
    * address range that starts at "bd_space" is made uncached.
    */
   Xil_SetTlbAttributes((INTPTR)EMACps_bdmem, DEVICE_MEMORY);
   EMACps_RxBDPtr = &(EMACps_bdmem[0]);
   EMACps_TxBDPtr = &(EMACps_bdmem[0x10000]);

   for(i = 0; i < MAX_RX_BUF_DESC - 1; i++)
   {
      ((u32 *)EMACps_RxBDPtr)[2*i] = ((u32)(EMACps_Rxbuf[i])) & ~RBD_RDY_BIT_MASK;
      ((u32 *)EMACps_RxBDPtr)[2*i + 1] = 0;
   }
   ((u32 *)EMACps_RxBDPtr)[2*i] = (((u32)(EMACps_Rxbuf[i])) | RBD_WRAP_BIT_MASK )&
                                  ~RBD_RDY_BIT_MASK;
   ((u32 *)EMACps_RxBDPtr)[2*i + 1] = 0;

   devicePtr->RxbdRing.StartAdd = (INTPTR)EMACps_RxBDPtr;
   devicePtr->RxbdRing.LastAdd = (INTPTR)EMACps_RxBDPtr + (MAX_RX_BUF_DESC -1)*8;
   devicePtr->RxbdRing.PresentAdd = (INTPTR)EMACps_RxBDPtr;

   /* Update Queue base register */
   XEmacPs_WriteReg(devicePtr->config.BaseAddress,
                    XEMACPS_RXQBASE_OFFSET, (INTPTR)EMACps_RxBDPtr);

   for(i = 0; i < MAX_TX_BUF_DESC - 1; i++)
   {
      ((u32 *)EMACps_TxBDPtr)[2*i] = 0;
      ((u32 *)EMACps_TxBDPtr)[2*i + 1] = USED_BIT_MASK;
   }
   ((u32 *)EMACps_TxBDPtr)[2*i] = 0;
   ((u32 *)EMACps_TxBDPtr)[2*i + 1] = USED_BIT_MASK | WRAP_BIT_MASK;

   devicePtr->TxbdRing.StartAdd = (INTPTR)EMACps_TxBDPtr;
   devicePtr->TxbdRing.LastAdd = (INTPTR)EMACps_TxBDPtr + (MAX_RX_BUF_DESC -1)*8;
   devicePtr->TxbdRing.PresentAdd = (INTPTR)EMACps_TxBDPtr;


   /* Update Queue base register */
   XEmacPs_WriteReg(devicePtr->config.BaseAddress,
                    XEMACPS_TXQBASE_OFFSET, (INTPTR)EMACps_TxBDPtr);
}/* End of EMACps_InitBufDesc */

static void EMACps_devStart(EMACPS_device_struct *devicePtr)
{
   u32 Reg;

   /* clear any existed int status */
   XEmacPs_WriteReg(devicePtr->config.BaseAddress, XEMACPS_ISR_OFFSET,
                    XEMACPS_IXR_ALL_MASK);

   /* Rx En */
   Reg = XEmacPs_ReadReg(devicePtr->config.BaseAddress, XEMACPS_NWCTRL_OFFSET);
   XEmacPs_WriteReg(devicePtr->config.BaseAddress,XEMACPS_NWCTRL_OFFSET,
                    Reg | (u32)XEMACPS_NWCTRL_RXEN_MASK);
#if 0
   /* Enable Rx interrupt */

   Reg = XEmacPs_ReadReg(devicePtr->config.BaseAddress, XEMACPS_IER_OFFSET);
   XEmacPs_WriteReg(devicePtr->config.BaseAddress,XEMACPS_IER_OFFSET,
                    (u32)XEMACPS_IXR_FRAMERX_MASK);
#endif
}/* End of EMACps_devStart */

static LONG EMACps_SetupIntrSystem(XScuGic *IntcInstancePtr,
                                   EMACPS_device_struct *devicePtr,
                                   u16 EmacPsIntrId)
{
   LONG Status;
   XScuGic_Config *GicConfig;

   Xil_ExceptionInit();

   /*
   * Initialize the interrupt controller driver so that it is ready to
   * use.
   */
   GicConfig = XScuGic_LookupConfig(0);
   if (NULL == GicConfig)
   {
      return XST_FAILURE;
   }

   Status = XScuGic_CfgInitialize(IntcInstancePtr, GicConfig,
                                  GicConfig->CpuBaseAddress);
   if (Status != XST_SUCCESS)
   {
      return XST_FAILURE;
   }

   /*
   * Connect the interrupt controller interrupt handler to the hardware
   * interrupt handling logic in the processor.
   */
   Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
                               (Xil_ExceptionHandler)XScuGic_InterruptHandler,
                               IntcInstancePtr);

   /*
   * Connect a device driver handler that will be called when an
   * interrupt for the device occurs, the device driver handler performs
   * the specific interrupt processing for the device.
   */
   Status = XScuGic_Connect(IntcInstancePtr, EmacPsIntrId,
                           (Xil_InterruptHandler) EMACps_IntrHandler,
                           (void *) devicePtr);
   if (Status != XST_SUCCESS)
   {
      print("Unable to connect ISR to interrupt controller");
      return XST_FAILURE;
   }

   /*
   * Enable interrupts from the hardware
   */
   XScuGic_Enable(IntcInstancePtr, EmacPsIntrId);

   /*
   * Enable interrupts in the processor
   */
   Xil_ExceptionEnable();

   return XST_SUCCESS;

}/* End of EMACps_SetupIntrSystem */

static void EMACps_RxHandler(void *CallBackRef)
{
   print("Data recvied");
   nRxFrames = nRxFrames + 1;
}

static void EMACps_IntrHandler(void *deviceptr)
{
   u32 RegISR;
   u32 RegSR;
   u32 RegCtrl;
   u32 RegQ1ISR = 0U;
   EMACPS_device_struct *InstancePtr = (EMACPS_device_struct *)deviceptr;

   RegISR = XEmacPs_ReadReg(InstancePtr->config.BaseAddress,
                            XEMACPS_ISR_OFFSET);

   /* Receive complete interrupt */
   if ((RegISR & XEMACPS_IXR_FRAMERX_MASK) != 0x00000000U)
   {
      XEmacPs_WriteReg(InstancePtr->config.BaseAddress,
                       XEMACPS_RXSR_OFFSET,
                       ((u32)XEMACPS_RXSR_FRAMERX_MASK |
                       (u32)XEMACPS_RXSR_BUFFNA_MASK));
      InstancePtr->recv_handler(InstancePtr);

      XEmacPs_WriteReg(InstancePtr->config.BaseAddress,
                       XEMACPS_ISR_OFFSET,
                       ((u32)XEMACPS_IXR_FRAMERX_MASK));
   }

}

//void (EMACPS_device_struct *devicePtr)
static err_t EMACps_datasend_ip(struct netif *netif, struct pbuf *p, const ip4_addr_t *ipaddr)
{
   return ethernet_output(netif, p, (const struct eth_addr*)netif->hwaddr, (const struct eth_addr*)dstaddr, ETHTYPE_IP);
}
static err_t EMACps_datasend(struct netif *netif, struct pbuf *p)
{
   EMACPS_device_struct *devicePtr = (EMACPS_device_struct *)netif->state;
   u32 Reg;

   /* Clear all interrupts */
   XEmacPs_WriteReg(devicePtr->config.BaseAddress, XEMACPS_ISR_OFFSET,
                    XEMACPS_IXR_ALL_MASK);

   //mem_cpy(&TxFrame,)

   Xil_DCacheFlushRange((UINTPTR)p->payload, p->len);

   /* Set Buf decsriptor */
   ((u32 *)devicePtr->TxbdRing.PresentAdd)[0] = (u32)(p->payload);
   ((u32 *)devicePtr->TxbdRing.PresentAdd)[1] &= ~(DATALEN_MASK );
   ((u32 *)devicePtr->TxbdRing.PresentAdd)[1] = LAST_BUF_FRM_MASK |
                                (p->len & DATALEN_MASK);

   if(devicePtr->TxbdRing.PresentAdd == devicePtr->TxbdRing.LastAdd)
   {
      ((u32 *)devicePtr->TxbdRing.PresentAdd)[1] |= WRAP_BIT_MASK;
   }
   ((u32 *)devicePtr->TxbdRing.PresentAdd)[1] &= ~(USED_BIT_MASK );

   //Xil_DCacheFlushRange((UINTPTR)EMACps_TxBDPtr, 64);

   /* Tx En */
   Reg = XEmacPs_ReadReg(devicePtr->config.BaseAddress, XEMACPS_NWCTRL_OFFSET);
   XEmacPs_WriteReg(devicePtr->config.BaseAddress,XEMACPS_NWCTRL_OFFSET,
                    Reg | (u32)XEMACPS_NWCTRL_TXEN_MASK);

   /* Tx start */
   Reg = XEmacPs_ReadReg(devicePtr->config.BaseAddress, XEMACPS_NWCTRL_OFFSET);
   XEmacPs_WriteReg(devicePtr->config.BaseAddress, XEMACPS_NWCTRL_OFFSET,
                    Reg | XEMACPS_NWCTRL_STARTTX_MASK);

   /* Wait until send */
   while(1)
   {
      Reg = XEmacPs_ReadReg(devicePtr->config.BaseAddress, XEMACPS_TXSR_OFFSET);

      if(Reg & XEMACPS_TXSR_TXCOMPL_MASK)
      {
         XEmacPs_WriteReg(devicePtr->config.BaseAddress, XEMACPS_TXSR_OFFSET,
                          XEMACPS_TXSR_TXCOMPL_MASK);

         //devicePtr->TxbdRing.PresentAdd
         break;
      }
   }
   nTxFrames = nTxFrames + 1;
   printf("Frames Sent: %ld\nFrames present add: %lx\n",nTxFrames,devicePtr->TxbdRing.PresentAdd);

   if(devicePtr->TxbdRing.PresentAdd == devicePtr->TxbdRing.LastAdd)
   {
      devicePtr->TxbdRing.PresentAdd = devicePtr->TxbdRing.StartAdd;
   }
   else
   {
      devicePtr->TxbdRing.PresentAdd = devicePtr->TxbdRing.PresentAdd + 8;
   }



   pbuf_free(p);
   p = NULL;

   return ERR_OK;
}
