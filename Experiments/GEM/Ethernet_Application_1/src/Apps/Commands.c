#include "xil_types.h"

#include "Commands.h"
#include "UserApp.h"

static void CMD_UpdateCRC(u8 *buf, u16 len);
int dataval = 0;

u16 CMD_prepare(u16 cmd, u8 *buf)
{
   u16 len;

   buf[0] = (u8)(cmd >> 8);
   buf[1] = (u8)cmd;

   /* Check command */
   switch(cmd)
   {
      case LINK_REQ :

         buf[2] = 0x00;
         buf[3] = 0x00;

         CMD_UpdateCRC(buf,4);

         len = 6;
         dataval = 0;

         break;
      case DATA_SND:
         buf[2] = 0x00;
         buf[3] = 0x02;
         buf[4] = (u8)(dataval >> 8);
         buf[5] = (u8)dataval;

         dataval = dataval + 1;

         CMD_UpdateCRC(buf,6);

         len = 8;

         break;
   }

   return len;

}/* End of CMD_prepare */

void CMD_process(UserAppComStruct *ptr)
{
   u16 cmd;

   /* Check CRC */

   /* Read command */
   cmd = ptr->rcvptr[0] << 8 | ptr->rcvptr[1];

   switch(cmd)
   {
      case LINK_REPLY:

         ptr->Comflags &= ~(UDP_PACK_ACK_RCV | UDP_LINK_PRG);
         ptr->Comflags |= UDP_LINK_ACTIVE;

         break;
      case DATA_CRC_OK:

         ptr->Comflags &= ~(UDP_PACK_ACK_RCV | UDP_LINK_PRG);
         ptr->Comflags |= UDP_LINK_ACTIVE;

         break;
   }


}/* End of CMD_process */

static void CMD_UpdateCRC(u8 *buf, u16 len)
{
   buf[len] = 0xAA;
   buf[len + 1] = 0xAA;

}/* End of CMD_UpdateCRC */
