#ifndef _COMMANDS_H_
   #define _COMMANDS_H_

   #include "UserApp.h"

   #define LINK_REQ                    0x0001
   #define LINK_REPLY                  0x0002
   #define DATA_SND                    0x0003
   #define DATA_CRC_OK                 0x0004

   extern u16 CMD_prepare(u16 cmd, u8 *buf);
   extern void CMD_process(UserAppComStruct *ptr);
#endif
