#ifndef _SYSTIMER_H_
   #define _SYSTIMER_H_
   #include "Xil_types.h"
   #include "xscutimer.h"

   typedef enum
   {
      TIMER_NOT_RUNNING = 0,
      TIMER_NOTEXPIRED,
      TIMER_EXPIRED
   }TIMER_STATES;

   typedef struct
   {
      u8 hrs;
      u8 min;
      u8 sec;
      u16 usec;
   }SYSTIME;

   typedef struct
   {
      u32 strt;
      u32 expire;
      TIMER_STATES state;
   }TIMEOUTTIMER;

   #define MAX_uSEC     999
   #define MAX_SEC      59
   #define MAX_MIN      59
   #define MAX_HRS      23

   #define SYSTIMER_CLK          (XPAR_CPU_CORTEXA9_CORE_CLOCK_FREQ_HZ /2)

   #define SYSTIME_TICK_CNT      (unsigned long int)(SYSTIMER_CLK / 1000)
   //#define SYSTIME_TICK_CNT      (unsigned long int)(SYSTIMER_CLK)

   #define SYSTIMER_IS_EXPIRE(ptr)    XScuTimer_IsExpired(ptr)

   extern XScuTimer *SysTimer_ptr;
   extern SYSTIME SysTime;

   extern void SysTimerInit(void);
   extern void SysTimerStart(void);
   extern void TimerCreate(TIMEOUTTIMER* tmr, u32 expiretime);
   extern TIMER_STATES TimerIsExpired(TIMEOUTTIMER* tmr);
   extern void Systimer_IntrHandler(void *deviceptr);

#endif
