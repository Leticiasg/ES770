/* ***************************************************************** */
/* File name:        timer_counter.c                                 */
/* File description: This file has a couple of useful functions to   */
/*                   control peripheral cooler fan speed using PWM   */
/* Author name:      dloubach                                        */
/* Revisor names:    fcpinheiro,lopfrigerio                          */
/* Creation date:    09jan2015                                       */
/* Revision date:    20jun2018                                       */
/* ***************************************************************** */

#include "KL25Z/es670_peripheral_board.h"
#include "fsl_tpm_hal.h"

#define MOTOR_COUNT 999 /* (system clock = 8MHz)/(max fan frequency = 8kHz) */
#define MOTOR_MAX_PWM 100

/* ************************************************** */
/* Method name: 	   timer_initTPM1AsPWM            */
/* Method description: configure Timer1 to act as PWM */
/* Input params:	   n/a 			                  */
/* Outpu params:	   n/a 		                      */
/* ************************************************** */
void timer_initTPM1AsPWM(void)
{
	/* un-gate TPM1 clock*/
	SIM_SCGC6 |= SIM_SCGC6_TPM1(CGC_CLOCK_ENABLED);

	/* TPM1 as PWM up counting mode */
	TPM1_SC |= TPM_SC_CPWMS(TPM_SC_CPWMS_UPCNT);
	
	/* LPTPM Counter increments on every LPTPM counter clock */
	TPM1_SC |= TPM_SC_CMOD(TPM_SC_CMOD_EVRCNTCLK);
	
	/* Prescale 1:1 */
	TPM1_SC |= TPM_SC_PS(TPM_SC_PS_1);   

	/* TPM1 as PWM Edge Aligned */
	TPM1_C1SC |= TPM_CnSC_MSB(TPM_CNSC_MSB_PWM) | TPM_CnSC_MSA(TPM_CNSC_MSA_PWM) | TPM_CnSC_ELSB(TPM_CNSC_ELSB_PWM) | TPM_CnSC_ELSA(TPM_CNSC_ELSA_PWM);

	/* configure PWM period */
	TPM1_MOD = TPM_MOD_MOD(MOTOR_COUNT);
	
	/* Channel 0 Config - Heater */
	TPM1_C0SC |= TPM_CnSC_MSB(TPM_CNSC_MSB_PWM) | TPM_CnSC_MSA(TPM_CNSC_MSA_PWM) | TPM_CnSC_ELSB(TPM_CNSC_ELSB_PWM) | TPM_CnSC_ELSA(TPM_CNSC_ELSA_PWM);

	/* duty cycle for motors is initially 0% */
	TPM1_C0V = TPM_CnV_VAL(0);

	TPM1_C1V = TPM_CnV_VAL(0);
}

///* ************************************************ */
///* Method name:        timer_coolerfan_init         */
///* Method description: Initialize the cooler device */
///*                     as PWM controlled            */
///* Input params:       n/a                          */
///* Output params:      n/a                          */
///* ************************************************ */
//void timer_coolerfan_init(void)
//{
//    /* un-gate port clock*/
//    SIM_SCGC5 |= SIM_SCGC5_PORTA(CGC_CLOCK_ENABLED);
//
//    /* set pin as PWM */
//    PORTA_PCR13 = PORT_PCR_MUX(FAN_PWM_ALT);
//}
//
///* ************************************************ */
///* Method name:        timer_initHeater             */
///* Method description: Initialize the heater device */
///* Input params:       n/a                          */
///* Output params:      n/a                          */
///* ************************************************ */
//void timer_initHeater(void)
//{
//    /* un-gate port clock*/
//    SIM_SCGC5 |= SIM_SCGC5_PORTA(CGC_CLOCK_ENABLED);
//
//    /* set pin as PWM */
//    PORTA_PCR12 |= PORT_PCR_MUX(HEATER_PWM_ALT);
//}

void timer_initMotor(void)
{
    /* un-gate port clock*/
    SIM_SCGC5 |= SIM_SCGC5_PORTA(CGC_CLOCK_ENABLED);

    /* set motor 1 and 2 pins as PWM */
    PORTA_PCR12 |= PORT_PCR_MUX(MOTOR_PWM_ALT);
    PORTA_PCR13 |= PORT_PCR_MUX(MOTOR_PWM_ALT);
}


/* ************************************************** */
/* Method name:        timer_setFanDutyCycle          */
/* Method description: Set the cooler device to       */
/*                     desired duty cycle             */
/* Input params:       cDutyCycle = duty cycle (in %) */
/* Output params:      n/a                            */
/* ************************************************** */
//void timer_setFanDutyCycle(char cDutyCycle)
//{
//	unsigned int uiMod;
//	if(cDutyCycle > FAN_MAX_PWM)
//	{
//		cDutyCycle = FAN_MAX_PWM;
//	}
//	if(cDutyCycle < 0)
//	{
//		cDutyCycle = 0;
//	}
//	uiMod = (unsigned int) (((float) cDutyCycle/100)*FAN_COUNT);
//	TPM1_C1V = TPM_CnV_VAL(uiMod);
//}
//
///* ************************************************ */
///* Method name:        timer_changeHeaterPwm        */
///* Method description: change the cooler pwm        */
///* Input params:       pwm desired (percentage)     */
///* Output params:      n/a                          */
///* ************************************************ */
//void timer_changeHeaterPwm(int iPwm)
//{
//	if(iPwm > HEATER_MAX_PWM)
//	{
//		iPwm = HEATER_MAX_PWM;
//	}
//	iPwm = (unsigned int) (((float) iPwm/100)*FAN_COUNT);
//	TPM1_C0V = iPwm;
//}

void timer_changeMotor1Pwm(int iPwm)
{
	if(iPwm > MOTOR_MAX_PWM)
	{
		iPwm = MOTOR_MAX_PWM;
	}
	if(iPwm < 0)
	{
		iPwm = 0;
	}
	iPwm = (unsigned int) (((float) iPwm/100)*MOTOR_COUNT);
	TPM1_C0V = iPwm;
}

void timer_changeMotor2Pwm(int iPwm)
{
	if(iPwm > MOTOR_MAX_PWM)
	{
		iPwm = MOTOR_MAX_PWM;
	}
	if(iPwm < 0)
	{
		iPwm = 0;
	}
	iPwm = (unsigned int) (((float) iPwm/100)*MOTOR_COUNT);
	TPM1_C1V = iPwm;
}

