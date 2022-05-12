#include <stdio.h>
#include <stdlib.h>
#include "xparameters.h"
#include "xil_printf.h"



#include "EthernetHW.h"



/*
EthernetFrame TxFrame;
EthernetFrame RxFrame;
*/









#if 0
static void EMACps_FrameHdrFormatMAC(EthernetFrame * FramePtr, char *DestAddr)
{
   char *Frame = (char *) FramePtr;
   char *SourceAddress = (char *)devicePtr->macaddr;
   s32 Index;

   /* Destination address */
   for (Index = 0; Index < XEMACPS_MAC_ADDR_SIZE; Index++) {
      *Frame++ = *DestAddr++;
   }

   /* Source address */
   for (Index = 0; Index < XEMACPS_MAC_ADDR_SIZE; Index++) {
      *Frame++ = *SourceAddress++;
   }
}

static void EMACps_FrameHdrFormatType(EthernetFrame * FramePtr, u16 FrameType)
{
   char *Frame = (char *) FramePtr;

   /*
    * Increment to type field
    */
   Frame = Frame + 12;
   /*
    * Do endian swap from little to big-endian.
    */
   FrameType = Xil_EndianSwap16(FrameType);
   /*
    * Set the type
    */
   *(u16 *) Frame = FrameType;
}

static void EMACps_FrameSetPayloadData(EthernetFrame * FramePtr, u32 PayloadSize)
{
   u32 BytesLeft = PayloadSize;
   u8 *Frame;

   /*
    * Set the frame pointer to the start of the payload area
    */
   Frame = (u8 *) FramePtr + XEMACPS_HDR_SIZE;

   *(Frame + 0) = 0x00;
   *(Frame + 1) = 0x01;

   *(Frame + 2) = 0x08;
   *(Frame + 3) = 0x00;

   *(Frame + 4) = 0x06;

   *(Frame + 5) = 0x04;

   *(Frame + 6) = 0x00;
   *(Frame + 7) = 0x01;

   *(Frame + 8) = devicePtr->macaddr[0];
   *(Frame + 9) = devicePtr->macaddr[1];
   *(Frame + 10) = devicePtr->macaddr[2];
   *(Frame + 11) = devicePtr->macaddr[3];
   *(Frame + 12) = devicePtr->macaddr[4];
   *(Frame + 13) = devicePtr->macaddr[5];

   *(Frame + 14) = 192;
   *(Frame + 15) = 168;
   *(Frame + 16) = 1;
   *(Frame + 17) = 10;

   *(Frame + 18) = 0x00;
   *(Frame + 19) = 0x00;
   *(Frame + 20) = 0x00;
   *(Frame + 21) = 0x00;
   *(Frame + 22) = 0x00;
   *(Frame + 23) = 0x00;

   *(Frame + 24) = 192;
   *(Frame + 25) = 168;
   *(Frame + 26) = 1;
   *(Frame + 27) = 2;

   for(BytesLeft = 0; BytesLeft < 20; BytesLeft++)
   {
      *(Frame + 28 + BytesLeft) = 0x20;
   }

}

#endif

#if 0
void EMACps_datasend(EMACPS_device_struct *devicePtr)
{
   u32 PayloadSize;
   u32 TxFrameLength;
   u32 Reg;

   PayloadSize = 48;
   TxFrameLength = XEMACPS_HDR_SIZE + PayloadSize;

   /*
    * Setup packet to be transmitted
    */
   EMACps_FrameHdrFormatMAC(&TxFrame, (char *)MAC_ADDRESS_broadcast);
   EMACps_FrameHdrFormatType(&TxFrame, 0x0806);
   EMACps_FrameSetPayloadData(&TxFrame, PayloadSize);

   /* Clear all interrupts */
   XEmacPs_WriteReg(devicePtr->config.BaseAddress, XEMACPS_ISR_OFFSET,
                    XEMACPS_IXR_ALL_MASK);

   Xil_DCacheFlushRange((UINTPTR)&TxFrame, sizeof(EthernetFrame));

   /* Set Buf decsriptor */
   ((u32 *)EMACps_TxBDPtr)[0] = (u32)TxFrame;
   ((u32 *)EMACps_TxBDPtr)[1] &= ~(DATALEN_MASK );
   ((u32 *)EMACps_TxBDPtr)[1] = LAST_BUF_FRM_MASK |
                                (TxFrameLength & DATALEN_MASK);

   ((u32 *)EMACps_TxBDPtr)[1] &= ~(USED_BIT_MASK );

   //Xil_DCacheFlushRange((UINTPTR)EMACps_TxBDPtr, 64);

   /* Tx En */
   Reg = XEmacPs_ReadReg(devicePtr->config.BaseAddress, XEMACPS_NWCTRL_OFFSET);
   XEmacPs_WriteReg(devicePtr->config.BaseAddress,XEMACPS_NWCTRL_OFFSET,
                    Reg | (u32)XEMACPS_NWCTRL_TXEN_MASK);

   /* Tx start */
   Reg = XEmacPs_ReadReg(devicePtr->config.BaseAddress, XEMACPS_NWCTRL_OFFSET);
   XEmacPs_WriteReg(devicePtr->config.BaseAddress, XEMACPS_NWCTRL_OFFSET,
                    Reg | XEMACPS_NWCTRL_STARTTX_MASK);
}

#endif
