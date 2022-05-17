#ifndef _ETHERNETIF_H_
   #define _ETHERNETIF_H_

   #include "xemacps.h"

   //#define DEBUG_ETHDATA

   #ifdef DEBUG_ETHDATA
      #define printf_eth  printf
   #else
      #define printf_eth(x,...)
   #endif

   typedef char EthernetFrame[XEMACPS_MAX_FRAME_SIZE] __attribute__ ((aligned(64)));

   typedef struct
   {
      u16 DeviceId;  /**< Unique ID  of device */
      u32 BaseAddress;/**< Physical base address of IPIF registers */
      u8 IsCacheCoherent; /**< Applicable only to A53 in EL1 mode; */
   }EMACPS_device_config;

   typedef struct
   {
      u32 StartAdd;
      u32 LastAdd;
      u32 PresentAdd;
   }EMACPS_BD_RING;

#ifdef InterruptsEnable
   typedef void (*INTHANDLER)(void *CallBackRef);
#endif

   typedef struct
   {
      EMACPS_device_config config;
      int macaddrlen;
      char macaddr[6];
      EMACPS_BD_RING RxbdRing;
      EMACPS_BD_RING TxbdRing;
#ifdef InterruptsEnable
      INTHANDLER recv_handler;
#endif
   }EMACPS_device_struct;

   #define MAX_TX_BUF_DESC 32
   #define MAX_RX_BUF_DESC 32

   /* TX Buffer Descriptor control bits */
   #define USED_BIT_MASK         0x80000000U
   #define WRAP_BIT_MASK         0x40000000U
   #define LAST_BUF_FRM_MASK     0x00008000U
   #define DATALEN_MASK          0x00003FFFU

   /* RX Buffer Descriptor control bits */
   #define RBD_RDY_BIT_MASK          0x00000001U
   #define RBD_WRAP_BIT_MASK         0x00000002U

   #define RBD_MATCH_FOUND           0x08000000U
   #define RBD_DATA_MASK             0x00001FFFU

#ifdef InterruptsEnable
   #define EMACPS_IRPT_INTR   XPS_GEM0_INT_ID
#endif


   #define EMACPS_SLCR_DIV_MASK  0xFC0FC0FF
   #define SLCR_GEM0_CLK_CTRL_ADDR     (XPS_SYS_CTRL_BASEADDR + 0x140)
   #define SLCR_GEM1_CLK_CTRL_ADDR     (XPS_SYS_CTRL_BASEADDR + 0x144)

   /* Link status Register */
   #define PHY_STATUS_REG        1
   #define PHY_DETECT_REG1       2
   #define PHY_DETECT_REG2       3
   #define PHY_INTR_EN_REG       0x12
   #define PHY_INTR_STATUS_REG   0x13
   #define PHY_LINK_REAL_REG   0x11


   #define IEEE_STAT_LINK_STATUS             0x04
   #define IEEE_STAT_LINK_STATUS_REAL        0x400


   extern const char MAC_ADDRESS[];
   extern const char MAC_ADDRESS_broadcast[];
   extern EMACPS_device_struct *EMACPS_device_ptr;
   extern EMACPS_device_struct EMACPS_device;


   extern err_t EMACps_init(struct netif *netif);
   extern err_t EMACps_Read(struct netif *netif);
   extern u8 EthernetLinkStatus(EMACPS_device_struct *devicePtr);

#endif
