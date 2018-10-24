/* ***************************************************************** */
/* File name:        tacometer_hal.c                                 */
/* File description: This file implements functions to initiate and  */
/*                   get cooler fan speed using tacometer peripheral */
/* Author name:      fcpinheiro,lopfrigerio                          */
/* Creation date:    16may2018                                       */
/* Revision date:    23may2018                                       */
/* ***************************************************************** */

#include "tacometer_hal.h"
#include "../KL25Z/es670_peripheral_board.h"

#define PADS_NO 7
#define MAXSPEED_STABLE_TIME 10000 /* aprox. time to stabilization in miliseconds */

/* ************************************************** */
/* Method name:        tacometer_init                 */
/* Method description: Initiate tacometer implemented */
/*                     on TPM0 module                 */
/* Input params:       n/a                            */
/* Output params:      n/a                            */
/* ************************************************** */
void tacometer_init()
{
	/* un-gate tacometer pin */
	SIM_SCGC5 |= SIM_SCGC5_PORTE(CGC_CLOCK_ENABLED);

	/* un-gate TPM0 */
	SIM_SCGC6 |= SIM_SCGC6_TPM0(CGC_CLOCK_ENABLED);

	/* set tacometer pin as FTM_CLKIN0 */
	PORTE_PCR29 = PORT_PCR_MUX(FTM_CLKIN0);

	/* define prescale as 1 */
	TPM0_SC |= TPM_SC_PS(TPM_SC_PS_1) | TPM_SC_CMOD(TPM_SC_CMOD_RSNGEDGE);

	/* increments counter on LPTPM_EXTCLK rising edge */
//	TPM0_SC |= TPM_SC_CMOD(TPM_SC_CMOD_RSNGEDGE);

	/* select OSCERCCLK clock source */
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(SIM_SOPT2_TPMSRC_OSCERCCLK) | SIM_SOPT2_CLKOUTSEL(SIM_SOPT2_CLKOUTSEL_OSCERCCLK);

	/* select OSCERCCLK as clock output */
//	SIM_SOPT2 |= SIM_SOPT2_CLKOUTSEL(SIM_SOPT2_CLKOUTSEL_OSCERCCLK);

	/* select TPM0 external clock as TPM_CLKIN0 pin */
	SIM_SOPT4 |= SIM_SOPT4_TPM0CLKSEL(SIM_SOPT4_TPMCLKSEL_TPM_CLKIN0);
}

/* *************************************************** */
/* Method name:        tacometer_getSpeed              */
/* Method description: Returns speed based on cooler   */
/*                     physical configuration and main */
/*                     loop time                       */
/* Input params:       uiCeeTime = main loop time in   */ 
/*                     microseconds                    */
/* Output params:      tacometer measured speed in rps */
/* *************************************************** */
int tacometer_getSpeed(unsigned int uiCeeTime)
{
	unsigned int uiTacoCount;

	/* get counter value*/
	uiTacoCount = TPM_CNT_COUNT(TPM0_CNT);

	/* clear counter register */
	TPM0_CNT = 0x0u;

	/* return speed [rps] */
	return (uiTacoCount * 1000*1000)/(PADS_NO*uiCeeTime) ;
}
