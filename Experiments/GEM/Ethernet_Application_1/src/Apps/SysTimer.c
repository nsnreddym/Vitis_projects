#include <stdio.h>
#include <stdlib.h>
#include "xparameters.h"
#include "xil_printf.h"
#include "xscutimer.h"

#include "SysTimer.h"


#include "xgpiops_hw.h"

XScuTimer SysTimer;
XScuTimer *SysTimer_ptr;

SYSTIME SysTime;
u32 SysTimeTick;

extern int linkfail;

void SysTimerInit(void)
{
   XScuTimer_Config config;

   SysTimer_ptr = &SysTimer;

   config.DeviceId = XPAR_PS7_SCUTIMER_0_DEVICE_ID;
   config.BaseAddr = XPAR_PS7_SCUTIMER_0_BASEADDR;

   /* Config timer */
   XScuTimer_CfgInitialize(SysTimer_ptr, &config, XPAR_PS7_SCUTIMER_0_BASEADDR);

   /* Load Timer value */
   XScuTimer_LoadTimer(SysTimer_ptr, SYSTIME_TICK_CNT);

   /* Enable Auto reload */
   XScuTimer_EnableAutoReload(SysTimer_ptr);

}

void SysTimerStart(void)
{
   SysTimeTick = 0;

   /* Enable interrupt */
   XScuTimer_EnableInterrupt(SysTimer_ptr);

   /* Start Timer */
   XScuTimer_Start(SysTimer_ptr);

}/* End of SysTimerInit */

void TimerCreate(TIMEOUTTIMER* tmr, u32 expiretime)
{
   tmr->expire = expiretime;
   tmr->strt = SysTimeTick;
   tmr->state = TIMER_NOTEXPIRED;

}/* End of TimerCreate */

TIMER_STATES TimerIsExpired(TIMEOUTTIMER* tmr)
{
   if(tmr->state == TIMER_EXPIRED)
   {
      tmr->state = TIMER_NOT_RUNNING;
   }
   else
   {
      if(tmr->expire <= (SysTimeTick - tmr->strt))
      {
         tmr->state = TIMER_EXPIRED;
      }
      else
      {
         tmr->state = TIMER_NOTEXPIRED;
      }
   }

   return tmr->state;

}/* End of TimerIsExpired */

void Systimer_IntrHandler(void *deviceptr)
{
   SysTimeTick = SysTimeTick + 1;

   if(SysTime.usec < MAX_uSEC)
   {
      SysTime.usec = SysTime.usec + 1;
   }
   else
   {
      SysTime.usec = 0;
      if(SysTime.sec < MAX_SEC)
      {
         SysTime.sec = SysTime.sec + 1;
      }
      else
      {
         SysTime.sec = 0;
         if(SysTime.min < MAX_MIN)
         {
            SysTime.min = SysTime.min + 1;
         }
         else
         {
            SysTime.min = 0;
            if(SysTime.hrs < MAX_HRS)
            {
               SysTime.hrs = SysTime.hrs + 1;
            }
            else
            {
               SysTime.hrs = 0;

            }
         }
      }
   }

   /* Clear interrupt */
   XScuTimer_ClearInterruptStatus((XScuTimer *)deviceptr);


}/* End of Systimer_IntrHandler */
