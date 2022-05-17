#include <stdio.h>
#include <stdlib.h>
#include "xparameters.h"
#include "xil_printf.h"
#include "xscugic.h"
#include "xscutimer.h"

#include "SysTimer.h"
#include "..\GPIO\Gpio.h"
#include "GIC.h"





XScuGic GicInstance;
XScuGic *GicInstanceptr;

long GicInit(void)
{
   long Status;

   XScuGic_Config *GicConfig;
   GicInstanceptr = &GicInstance;

   Xil_ExceptionInit();

   /* Read Config */
   GicConfig = XScuGic_LookupConfig(0);
   if (NULL == GicConfig)
   {
      return XST_FAILURE;
   }

   /* Configuration Initialize */
   Status = XScuGic_CfgInitialize(GicInstanceptr, GicConfig,
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
                               GicInstanceptr);

   /*
    * Connect a device driver handler that will be called when an
    * interrupt for the device occurs, the device driver handler performs
    * the specific interrupt processing for the device.
    */
   Status = XScuGic_Connect(GicInstanceptr, SYSTIMER_IRPT_INTR,
                           (Xil_InterruptHandler) Systimer_IntrHandler,
                           (void *) SysTimer_ptr);
   if (Status != XST_SUCCESS)
   {
      print("Unable to connect ISR to interrupt controller");
      return XST_FAILURE;
   }




   Status = XScuGic_Connect(GicInstanceptr, XPS_GPIO_INT_ID,
                           (Xil_InterruptHandler) GPIOIntrHandler,
                           (void *) 0);
   if (Status != XST_SUCCESS)
   {
      print("Unable to connect ISR to interrupt controller");
      return XST_FAILURE;
   }

   /*
    * Enable interrupts from the hardware
    */
   XScuGic_Enable(GicInstanceptr, SYSTIMER_IRPT_INTR);
   XScuGic_Enable(GicInstanceptr, XPS_GPIO_INT_ID);

   /*
    * Enable interrupts in the processor
    */
   Xil_ExceptionEnable();

   return XST_SUCCESS;

}/* End of GicInit */
