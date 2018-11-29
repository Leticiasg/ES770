/* ***************************************************************** */
/* File name:        timer_counter.c                                 */
/* File description: This file has a couple of useful functions to   */
/*                   control peripheral cooler fan speed using PWM   */
/* Author name:      dloubach                                        */
/* Revisor names:    fcpinheiro,lsgoncalves                          */
/* Creation date:    09jan2015                                       */
/* Revision date:    nov2018                                         */
/* ***************************************************************** */

#include "KL25Z/es670_peripheral_board.h"
#include "fsl_tpm_hal.h"

#define MOTOR_COUNT 	39999
#define MOTOR_MAX_PWM 	100

/* ************************************************** */
/* Method name: 	   timer_initTPM1AsPWM            */
/* Method description: configure Timer1 to act as PWM */
/* Input params:	   iPwm1,iPwm2                    */
/* Outpu params:	   n/a 		                      */
/* ************************************************** */
void timer_initTPM1AsPWM(int iPwm1, int iPwm2)
{
	/* un-gate TPM1 clock*/
	SIM_SCGC6 |= SIM_SCGC6_TPM1(CGC_CLOCK_ENABLED);

	/* TPM1 as PWM up counting mode */
//	TPM1_SC |= TPM_SC_CPWMS(TPM_SC_CPWMS_UPCNT);
	
	/* LPTPM Counter increments on every LPTPM counter clock */
//	TPM1_SC |= TPM_SC_CMOD(TPM_SC_CMOD_EVRCNTCLK);
	
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(SIM_SOPT2_TPMSRC_OSCERCCLK);

	/* Prescale 1:1 */
//	TPM1_SC |= TPM_SC_PS(TPM_SC_PS_1);
	TPM1_SC |= TPM_SC_CPWMS(TPM_SC_CPWMS_UPCNT) | TPM_SC_CMOD(TPM_SC_CMOD_EVRCNTCLK) | TPM_SC_CMOD(TPM_SC_CMOD_EVRCNTCLK);

	/* TPM1 as PWM Edge Aligned */
	TPM1_C1SC |= TPM_CnSC_MSB(TPM_CNSC_MSB_PWM) | TPM_CnSC_MSA(TPM_CNSC_MSA_PWM) | TPM_CnSC_ELSB(TPM_CNSC_ELSB_PWM) | TPM_CnSC_ELSA(TPM_CNSC_ELSA_PWM);

	/* configure PWM period */
	TPM1_MOD = TPM_MOD_MOD(MOTOR_COUNT);
	
	/* Channel 0 Config - Heater */
	TPM1_C0SC |= TPM_CnSC_MSB(TPM_CNSC_MSB_PWM) | TPM_CnSC_MSA(TPM_CNSC_MSA_PWM) | TPM_CnSC_ELSB(TPM_CNSC_ELSB_PWM) | TPM_CnSC_ELSA(TPM_CNSC_ELSA_PWM);

	/* set initial PWM values for motors */
	TPM1_C0V = TPM_CnV_VAL(iPwm1*100/MOTOR_COUNT);
	TPM1_C1V = TPM_CnV_VAL(iPwm2*100/MOTOR_COUNT);
}

void timer_initMotor(void)
{
    /* un-gate port clock*/
    SIM_SCGC5 |= SIM_SCGC5_PORTA(CGC_CLOCK_ENABLED);

    /* set motor 1 and 2 pins as PWM */
    PORTA_PCR12 |= PORT_PCR_MUX(MOTOR_PWM_ALT);
    PORTA_PCR13 |= PORT_PCR_MUX(MOTOR_PWM_ALT);
}

void timer_initMotorAsGpio(void)
{

	 SIM_SCGC5 |= SIM_SCGC5_PORTA(CGC_CLOCK_ENABLED);

	 PORTA_PCR12 |= PORT_PCR_MUX(MOTOR_GPIO_ALT);
	 PORTA_PCR13 |= PORT_PCR_MUX(MOTOR_GPIO_ALT);

	 GPIOA_PDDR |= GPIO_PDDR_PDD(MOTOR1_DIR | MOTOR2_DIR);

}

void timer_MotorGpioEnable(void)
{
	GPIOA_PSOR = GPIO_PSOR_PTSO( (0x01U << MOTOR1_PIN) );
	GPIOA_PSOR = GPIO_PSOR_PTSO( (0x01U << MOTOR2_PIN) );
}

void timer_MotorGpioDisable(void)
{
	GPIOA_PCOR = GPIO_PCOR_PTCO( (0x01U << MOTOR1_PIN) );
	GPIOA_PCOR = GPIO_PCOR_PTCO( (0x01U << MOTOR2_PIN) );
}

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
	TPM1_C0V = TPM_CnV_VAL(iPwm);
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
	TPM1_C1V = TPM_CnV_VAL(iPwm);
}

