#ifndef _GPIO_H_
   #define _GPIO_H_

   #include "xemacps.h"

   #define SLCR_LOCK_ADDR        (XPS_SYS_CTRL_BASEADDR + 0x4)
   #define SLCR_UNLOCK_ADDR      (XPS_SYS_CTRL_BASEADDR + 0x8)

   #define SLCR_LOCK_KEY_VALUE      0x767B
   #define SLCR_UNLOCK_KEY_VALUE    0xDF0D
   #define SLCR_ADDR_GEM_RST_CTRL      (XPS_SYS_CTRL_BASEADDR + 0x214)

   #define MIO48PIN_ADDR              (XPS_SYS_CTRL_BASEADDR + 0x7C0)

   extern int PHYLinkStatusChanged;

   extern void GPIOInit(void);
   extern void GPIOIntrHandler(void *deviceptr);

#endif
