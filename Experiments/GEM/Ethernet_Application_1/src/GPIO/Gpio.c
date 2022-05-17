#include <stdio.h>
#include <stdlib.h>
#include "xparameters.h"
#include "xil_printf.h"
#include "xgpiops_hw.h"
#include "Xil_types.h"

#include "Gpio.h"

int PHYLinkStatusChanged;

void GPIOInit(void)
{
   u32 data;

   PHYLinkStatusChanged = 0;

   /* Set direction of GPIO as input */
   data = XGpioPs_ReadReg(XPAR_PS7_GPIO_0_BASEADDR,
                          XGPIOPS_REG_MASK_OFFSET * 1 + XGPIOPS_DIRM_OFFSET);
   data = data & ~((u32)1 << 16);
   XGpioPs_WriteReg(XPAR_PS7_GPIO_0_BASEADDR,
                    XGPIOPS_REG_MASK_OFFSET * 1 + XGPIOPS_DIRM_OFFSET,
                    data);

   /* Disable all interrupts */
   XGpioPs_WriteReg(XPAR_PS7_GPIO_0_BASEADDR,
                    XGPIOPS_REG_MASK_OFFSET * 1 + XGPIOPS_INTDIS_OFFSET,
                    0x3FFFFF);

   /* Edge triggered */
   XGpioPs_WriteReg(XPAR_PS7_GPIO_0_BASEADDR,
                    XGPIOPS_REG_MASK_OFFSET * 1 + XGPIOPS_INTTYPE_OFFSET,
                    0x10000);

   /* Polarity as falling edge */
   XGpioPs_WriteReg(XPAR_PS7_GPIO_0_BASEADDR,
                    XGPIOPS_REG_MASK_OFFSET * 1 + XGPIOPS_INTPOL_OFFSET,
                    0x00000);

   /* Interrupt on single edge */
   XGpioPs_WriteReg(XPAR_PS7_GPIO_0_BASEADDR,
                    XGPIOPS_REG_MASK_OFFSET * 1 + XGPIOPS_INTANY_OFFSET,
                    0x00000);

   /* Enable Interrupt */
   XGpioPs_WriteReg(XPAR_PS7_GPIO_0_BASEADDR,
                    XGPIOPS_REG_MASK_OFFSET * 1 + XGPIOPS_INTEN_OFFSET,
                    0x10000);
}/* End of GPIOInit */

void GPIOIntrHandler(void *deviceptr)
{
   u32 data;

   /* Read Status Register */
   data = XGpioPs_ReadReg(XPAR_PS7_GPIO_0_BASEADDR,
                          XGPIOPS_REG_MASK_OFFSET * 1 + XGPIOPS_INTSTS_OFFSET);

   /* Clear Status Register */
   XGpioPs_WriteReg(XPAR_PS7_GPIO_0_BASEADDR,
                    XGPIOPS_REG_MASK_OFFSET * 1 + XGPIOPS_INTSTS_OFFSET,
                    data);

   /* Update status changed flag */
   PHYLinkStatusChanged = TRUE;

   //printf("Interrupt triggered\r\n");
}

