#include <stdio.h>
#include <stdlib.h>
#include "xparameters.h"
#include "xil_printf.h"
#include "xemacps_hw.h"

#include "lwip/init.h"
#include "lwip/netif.h"
#include "lwip/ip.h"
#include "lwip/udp.h"

#include "UserApp.h"
#include "SysTimer.h"
#include "GIC.h"
#include "..\GPIO\Gpio.h"

int main(void)
{
   u8 sec;
   print("Entered Main loop\r\n");

   /* System Timer init */
   SysTimerInit();

   /* Global interrupt controller init */
   GicInit();

   GPIOInit();

   App_init();

   /* Start Timer */
   SysTimerStart();

   sec = SysTime.sec;

   /* Start Application */
   App_CommInit();
   while(1)
   {
      /* Check for connection */
      if(sec != SysTime.sec)
      {
         sec = SysTime.sec;
         App_CheckConnection();
      }

      /* Read Ethernet data */
      EMACps_Read(&usrAppLowLwvelDriver.netif[0]);

      App_CheckCommunication();
   }
}
