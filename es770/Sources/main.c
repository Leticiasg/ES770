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
#include "Util/adc.h"
#include "Util/pid.h"
#include "Util/util.h"
#include "Util/debugUart.h"
#include "Util/tc_hal.h"
#include "Util/timer_counter.h"
#include "mcg_hal.h"
#include "Fan/fan_hal.h"
#include "Tacometer/tacometer_hal.h"
#include "Serial/serial_hal.h"
#include "KL25Z/es670_peripheral_board.h"
//#include "Protocolo/cmdmachine_hal.h"
#include <stdio.h>

/* defines */

 /* in micro seconds = mili seconds * 1000 */
/* recommend value for operating control */
#define CYCLIC_EXECUTIVE_PERIOD    300*1000

#define MAXSPEED_STABLE_TIME 6000 /* aprox. time to stabilization in miliseconds */


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

void main_initOutputLeds(void)
{
	 SIM_SCGC5 |= SIM_SCGC5_PORTE(CGC_CLOCK_ENABLED);
	 SIM_SCGC5 |= SIM_SCGC5_PORTB(CGC_CLOCK_ENABLED);

	 GPIOE_PDDR |= GPIO_PDDR_PDD(OUTPUT_LED_PIN1 | OUTPUT_LED_PIN2 | OUTPUT_LED_PIN3 | OUTPUT_LED_PIN4);
	 GPIOB_PDDR |= GPIO_PDDR_PDD(OUTPUT_LED_PIN5);
}

void main_OuputLeds_setVal(char cLedNum,int iVal)
{

        switch(cLedNum)
        {
            case 1:
                if(iVal)
                	GPIOE_PSOR = GPIO_PSOR_PTSO( (0x01U << OUTPUT_LED_PIN1) );
                else
                	GPIOE_PCOR = GPIO_PCOR_PTCO( (0x01U << OUTPUT_LED_PIN1) );
                break;
            case 2:
            	if(iVal)
					GPIOE_PSOR = GPIO_PSOR_PTSO( (0x01U << OUTPUT_LED_PIN2) );
				else
					GPIOE_PCOR = GPIO_PCOR_PTCO( (0x01U << OUTPUT_LED_PIN2) );
				break;
            case 3:
            	if(iVal)
					GPIOE_PSOR = GPIO_PSOR_PTSO( (0x01U << OUTPUT_LED_PIN3) );
				else
					GPIOE_PCOR = GPIO_PCOR_PTCO( (0x01U << OUTPUT_LED_PIN3) );
				break;
            case 4:
            	if(iVal)
					GPIOE_PSOR = GPIO_PSOR_PTSO( (0x01U << OUTPUT_LED_PIN4) );
				else
					GPIOE_PCOR = GPIO_PCOR_PTCO( (0x01U << OUTPUT_LED_PIN4) );
				break;
            case 5:
            	if(iVal)
					GPIOB_PSOR = GPIO_PSOR_PTSO( (0x01U << OUTPUT_LED_PIN5) );
				else
					GPIOB_PCOR = GPIO_PCOR_PTCO( (0x01U << OUTPUT_LED_PIN5) );
				break;
        } /* switch(cLedNum) */

}

int main(void)
{
	/* local variable declarations */

	/* cooler fan duty cycle*/
//	static int iFanDcycle = 0;
//
//	/* led status vector, indicating if led in position is on or off*/
//	static led_status_type_e lstvLedSt[] = {LED_OFF,LED_OFF,LED_OFF,LED_OFF};
//
//	/* character received on serial */
//	unsigned int uiReceiveBuff = 0;
//
//	/* interpreted command from serial */
//	static char cvInterpMsg[6] = {0,0,0,0,0,0};
//
//	/* buzzer action time (in miliseconds) */
//	static int iBuzTime = 0;
//
//	/* ADC conversion value */
//	int iAdcValue = 0;
//
//	/* measured temperature */
//	int iTemp = 0;
//
//	/* cooler speed measured by tacometer */
//	int iTacoSpeed = 0;
//
//	/* cooler fan measured max speed in RPS */
//	int iMaxSpeed = 0;
//
//	/* cooler fan reference speed in RPS */
//	int iRefSpeed = 0;
//
//	/* heater duty cycle (in percentage) */
//	static int iHeaterDcycle = 0;

	/* PID controller data struct */
//	PID_DATA pdtContrData;

	/* PID out value */
//	double dPidOut = 0;
//
//	/* PID Kp constant */
//	int iKp;
//
//	/* PID Ki constant */
//	int iKi;
//
//	/* PID Kd constant */
//	int iKd;
//
//	/* PID out converted to PWM*/
//	int iPwm = 0;
//
//	/* first line of LCD message */
//	char cvLcdMsg1[16] = "                ";
//
//	/* second line of LCD message */
//	char cvLcdMsg2[16] = "                ";
//
//	int iCnt = 0;

	int ref = 0x15;



	/* initialization functions */
	mcg_clockInit();
//	timer_initTPM1AsPWM();
//	timer_coolerfan_init();
//	tacometer_init();
	debugUart_init();
	adc_initADCModule();
//	timer_initHeater();
//	lcd_initLcd();
//	buzzer_init();
	tc_installLptmr0(CYCLIC_EXECUTIVE_PERIOD, main_cyclicExecuteIsr);

	/* initiate first adc conversion */
	adc_initConvertion();

	main_initOutputLeds();

	/* set cooler fan PWM to 100% for reference setting */
//	timer_setFanDutyCycle(100);

	/* initialize PID struct */
//	pid_PidInitialize(&pdtContrData);


	/* cooperative cyclic executive main loop */
    for (;;) {

    	int iSensor1 = adc_getValue(1);
    	int iLedVal1 = iSensor1>ref;
    	main_OuputLeds_setVal(1,(iLedVal1));

    	int iSensor2 = adc_getValue(2);
    	int iLedVal2 = iSensor2>ref;
		main_OuputLeds_setVal(1,(iLedVal2));

		int iSensor3 = adc_getValue(3);
		int iLedVal3 = iSensor3>ref;
		main_OuputLeds_setVal(1,(iLedVal3));

		int iSensor4 = adc_getValue(4);
		int iLedVal4 = iSensor4>ref;
		main_OuputLeds_setVal(1,(iLedVal4));

		int iSensor5 = adc_getValue(5);
		int iLedVal5 = iSensor5>ref;
		main_OuputLeds_setVal(1,(iLedVal5));


//    	printf("%d %d %d %d %d\n",iSensor1,iSensor2,iSensor3,iSensor4,iSensor5);

//    	/* CCE functions here */
//
//    	/* check if buffer has character */
//		uiReceiveBuff = serial_receiveBuffer();
//		if(0 != uiReceiveBuff)
//		{
//			cmdmachine_interpretCmd(uiReceiveBuff,cvInterpMsg);
//		}
//
//		if(NONE_MSG != cvInterpMsg[0])
//		{
//			/* send response to host and execute commands */
//			cmdmachine_retCmdReponse(cvInterpMsg,lstvLedSt,&iBuzTime,&iFanDcycle,&iHeaterDcycle,&iRefSpeed,&pdtContrData);
//		}
//
//		/* set heater duty cycle */
//		timer_changeHeaterPwm(iHeaterDcycle);
//
//		/* control cooler fan speed */
//
//		/* get fan speed */
//		iTacoSpeed = tacometer_getSpeed(CYCLIC_EXECUTIVE_PERIOD);
//
//		/* if stabilization time is out control speed, else get max speed */
//		if(MAXSPEED_STABLE_TIME <= iCnt*CYCLIC_EXECUTIVE_PERIOD/1000)
//		{
//			/* max speed limiter */
//			if(iRefSpeed > iMaxSpeed)
//			{
//				iRefSpeed = iMaxSpeed;
//			}
//
//			/* generate control signal */
//			iPwm = (int)pid_PidUpdate(&pdtContrData,iTacoSpeed,iRefSpeed);
//
//			/* duty cycle limiter */
//			if(iPwm > 100)
//			{
//				iPwm = 100;
//			}
//			else if(iPwm < 0)
//			{
//				iPwm = 0;
//			}
//
//			/* set duty cycle using control signal */
//			timer_setFanDutyCycle(iPwm);
//		}
//		else
//		{
//			iCnt++;
//			iMaxSpeed = iTacoSpeed;
//		}
//
//
//		/* output led status */
//		uiFunctions_writeLeds(lstvLedSt);
//
//
//		/* truncate control constants for writing on LCD */
//		iKp = (int)pdtContrData.dKp;
//		iKi = (int)pdtContrData.dKi;
//		iKd = (int)pdtContrData.dKd;
//
//		/* Row 1: control constants */
//		sprintf(cvLcdMsg2,"P:%2d I:%2d D:%2d",iKp,iKi,iKd);
//
//		/* Row 2: Current duty cycle (in percentage), current speed (in RPS), reference speed (in RPS) */
//		sprintf(cvLcdMsg1,"PWM%3d V%2d S%2d",iPwm,iTacoSpeed,iRefSpeed);
//		lcd_writeBothRows(cvLcdMsg1,cvLcdMsg2);
//
//		/* output buzzer */
//    	uiFunctions_setBuzTime(&iBuzTime,CYCLIC_EXECUTIVE_PERIOD);
//
//    	/* convert and send temperature to host by serial */
//    	uiFunctions_computeAndSendTemp(&iAdcValue,&iTemp,CYCLIC_EXECUTIVE_PERIOD);

		/* WAIT FOR CYCLIC EXECUTIVE PERIOD */
//		while(!uiFlagNextPeriod);
//		uiFlagNextPeriod = 0;

    }
    /* Never leave main */
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////