/* ***************************************************************** */
/* File name:        main.c                                          */
/* File description: File dedicated to the ES670 prototype projects  */
/*                   involving the FRDM-KL25Z board together with is */
/*                   daughter board containing more peripherals      */
/*                                                                   */
/*                   Processor MKL25Z128VLK4 characteristics         */
/*                   48 MHz ARM Cortex-M0+ core                      */
/*                   128 KB program flash memory                     */
/*                   16 KB SRAM                                      */
/*                   Voltage range: 1.71 to 3.6 V                    */
/*                                                                   */
/* Author name:      fcpinheiro,lopfrigerio                          */
/* Creation date:    02may2018                                       */
/* Revision date:    20jun2018                                       */
/* ***************************************************************** */

/* system includes */
#include "fsl_device_registers.h"

/* our includes */
#include "Util/debugUart.h"
#include "Util/tc_hal.h"
#include "Util/motores.h"
#include "Util/adc.h"
#include "Util/pid.h"
#include "mcg_hal.h"
#include "KL25Z/es670_peripheral_board.h"

/* defines */

#define PORT_PCR_MUX_CLR_MASK	!(PORT_PCR_MUX_MASK)
#define BLACK_VALUE 7

 /* in micro seconds = mili seconds * 1000 */
/* recommend value for operating control */
#define CYCLIC_EXECUTIVE_PERIOD    20*1000

/* globals */

/* cyclic executive flag */
volatile unsigned int uiFlagNextPeriod = 0;

/* ************************************************ */
/* Method name:        main_cyclicExecuteIsr        */
/* Method description: cyclic executive interrupt   */
/*                     service routine              */
/* Input params:       n/a                          */
/* Output params:      n/a                          */
/* ************************************************ */
void main_cyclicExecuteIsr(void)
{
    /* set the cyclic executive flag */
    uiFlagNextPeriod = 1;
}

void main_disablePinNmi(void)
{
	SIM_SCGC5 |= SIM_SCGC5_PORTA(CGC_CLOCK_ENABLED);

	PORTA_PCR4 &= PORT_PCR_MUX_CLR_MASK;
//	PORTA_PCR4 |= PORT_PCR_MUX(ALT1);

//	GPIOA_PDDR |= GPIO_PDDR_PDD(LS3_DIR_OUTPUT);
}

int main(void)
{
	int i = 0;
	int j = 0;
	int iSensor[5] = {0,0,0,0,0};
	int iSensorMax[5] = {0,0,0,0,0};
	int iNormSensor[5] = {0,0,0,0,0};
	int iBoolSensor[5] = {0,0,0,0,0};
	int iGetMaxVal = 1;
	int iLineVal = 0;
	int iPwm1 = 30;
	int iPwm2 = 30;
//	double dLineError = 0.0;
	double dLineVal = 0.0;
	double dControl = 0.0;
	PID_DATA pidStr;

	/* initialization functions */
	mcg_clockInit();
//	debugUart_init();
	main_disablePinNmi();
	adc_initADCModule();
	adc_initConvertion2(i);
	pid_PidInitialize(&pidStr);
//	pwm_init(10,10);
//	motor_init();


	/* functions before loop */
	SIM_SCGC5 |= SIM_SCGC5_PORTA(CGC_CLOCK_ENABLED);

//	seta_pwm(0,50);
//	seta_pwm(1,50);
//	pwm_init(50,20);

	tc_installLptmr0(CYCLIC_EXECUTIVE_PERIOD, main_cyclicExecuteIsr);

	/* cooperative cyclic executive main loop */
    for (;;)
	{

		if(adc_isAdcDone())
		{
			iSensor[i] = adc_read2();
			if(j < 50)
				if(iSensor[i] > iSensorMax[i])
					iSensorMax[i] = iSensor[i];
			iNormSensor[i] = adc_normalizeReadValue10(iSensor[i],iSensorMax[i]);
			iBoolSensor[i] = (iNormSensor[i]>BLACK_VALUE);

			i++;
			if(i>4)
			{
				i=0;
				iGetMaxVal = 0;
			}
			adc_initConvertion2(i);
		}

		iLineVal = 0*(iBoolSensor[4]-iBoolSensor[0]) + 2*(iBoolSensor[3]-iBoolSensor[1]);
		dLineVal = (double)iLineVal;

		dControl = pid_PidUpdate(&pidStr,dLineVal,0.0);

		if(j>=50 && !iGetMaxVal && (iNormSensor[0] > BLACK_VALUE) ||
				(iNormSensor[1] > BLACK_VALUE) ||
				(iNormSensor[2] > BLACK_VALUE) ||
				(iNormSensor[3] > BLACK_VALUE) ||
				(iNormSensor[4] > BLACK_VALUE))
		{
			iPwm1 = (int)(20+dControl);
			iPwm2 = (int)(30-dControl);
			pwm_init(iPwm1,iPwm2);
		}
		else
		{
			pwm_init(0,0);
		}

    	/* WAIT FOR CYCLIC EXECUTIVE PERIOD */
		while(!uiFlagNextPeriod);
		uiFlagNextPeriod = 0;
		j++;

    }
    /* Never leave main */
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
