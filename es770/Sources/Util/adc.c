/* ***************************************************************** */
/* File name:        adc.c                                           */
/* File description: This file has a couple of useful functions to   */
/*                   operate ADC and convert read value to           */
/*                   temperature                                     */
/* Author name:      fcpinheiro,lopfrigerio                          */
/* Creation date:    07jun2018                                       */
/* Revision date:    20jun2018                                       */
/* ***************************************************************** */

#include "KL25Z/es670_peripheral_board.h"
#include "adc.h"

#define ADC0_SC1A_COCO (ADC0_SC1A >> 7) /* complete conversion flag */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                   TABELA PARA USO DO SENSOR DE TEMPERATURA            *
 *                   modificado para o range 0 - 3v3                     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
const unsigned char tabela_temp[256] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,					/*15*/
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,					/*31*/
		1, 1, 2, 2, 3, 3, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6,					/*47*/
		7, 7, 8, 8, 8, 8, 9, 9, 10, 10, 10, 10, 11, 11, 12, 12,			/*63*/
		12, 12, 13, 13, 14, 14, 15, 15, 15, 15, 16, 16, 16, 17, 17, 17,	/*79*/
		17, 18, 18, 19, 19, 19, 19, 20, 20, 21, 21, 21, 21, 22, 22, 23,	/*95*/
		23, 24, 24, 24, 24, 25, 25, 26, 26, 26, 26, 27, 27, 28, 28, 28,	/*111*/
		28, 29, 29, 30, 30, 30, 30, 31, 31, 32, 32, 32, 32, 33, 33, 34,	/*127*/
		34, 35, 35, 35, 35, 36, 36, 37, 37, 37, 37, 38, 38, 39, 39, 39,	/*143*/
		39, 40, 40, 41, 41, 41, 41, 42, 42, 43, 43, 44, 44, 44, 44, 45,	/*159*/
		45, 46, 46, 46, 46, 47, 47, 48, 48, 48, 48, 49, 49, 50, 50, 50,	/*175*/
		50, 51, 51, 52, 52, 53, 53, 53, 53, 54, 54, 55, 55, 55, 55, 56,	/*191*/
		56, 57, 57, 57, 57, 58, 58, 59, 59, 59, 59, 60, 60, 61, 61, 62,	/*207*/
		62, 62, 62, 63, 63, 64, 64, 64, 64, 65, 65, 66, 66, 66, 66, 67,	/*223*/
		67, 68, 68, 68, 68, 69, 69, 70, 70, 71, 71, 71, 71, 72, 72, 72,	/*239*/
		73, 73, 73, 73, 74, 74, 75, 75, 75, 75, 76, 76, 77, 77, 77, 77	/*255*/
};

/* ************************************************** */
/* Method name: 	   adc_initADCModule              */
/* Method description: configure ADC module           */
/* Input params:	   n/a 			                  */
/* Outpu params:	   n/a 			                  */
/* ************************************************** */
void adc_initADCModule(void)
{
   /* un-gate port clock*/
   SIM_SCGC6 |= SIM_SCGC6_ADC0(CGC_CLOCK_ENABLED);	//Enable clock for ADC

   /* un-gate port clock*/
   SIM_SCGC5 |= SIM_SCGC5_PORTE(CGC_CLOCK_ENABLED);

   /* set pin as ADC In */
   PORTE_PCR20 |= PORT_PCR_MUX(VOLTSENSOR_ADC_ALT);   //Voltage Sensor
   PORTE_PCR21 |= PORT_PCR_MUX(TEMPSENSOR_ADC_ALT);   //Temperature Sensor

   /* configure ADC with short sample time configuration, clock Divide = 1, Normal power */
   ADC0_CFG1 |= (ADC_CFG1_ADICLK(ADC_CFG1_ADICLK_BUS2) |
		   ADC_CFG1_MODE(ADC_CFG1_MODE_8BIT) |
		   ADC_CFG1_ADLSMP(ADC_CFG1_ADLSMP_SHORT) |
		   ADC_CFG1_ADIV(ADC_CFG1_ADIV_1) | ADC_CFG1_ADLPC(ADC_CFG1_ADLPC_NORMAL));

   /* configure ADC with reference voltage to external pins, dma disabled, compare function disabled, software trigger */
   ADC0_SC2 |= (ADC_SC2_REFSEL(ADC_SC2_REFSEL_EXT) |
		   ADC_SC2_DMAEN(ADC_SC2_DMAEN_DISABLE) |
		   ADC_SC2_ACFE(ADC_SC2_ACFE_DISABLE) |
		   ADC_SC2_ADTRG(ADC_SC2_ADTRG_SOFT));

   /* configure ADC with default time, normal conversion sequence, disable adack clock, 'a' channels*/
   ADC0_CFG2 |= (ADC_CFG2_ADLSTS(ADC_CFG2_ADLSTS_DEFAULT) |
		   ADC_CFG2_ADHSC(ADC_CFG2_ADHSC_NORMAL) |
		   ADC_CFG2_ADACKEN(ADC_CFG2_ADACKEN_DISABLE) |
		   ADC_CFG2_MUXSEL(ADC_CFG2_MUXSEL_ACHS));
}

/*
*/
void adc_init(uint8_t res)
{
	// Enable clocks
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK; // ADC 0 clock

	// Configure ADC
	ADC0_CFG1 = 0; // Reset register

	ADC0_CFG1 |= (ADC_CFG1_MODE(res) | // resolução da conversão
	            ADC_CFG1_ADICLK(0) | // Input Bus Clock
	            ADC_CFG1_ADIV(1));	 // Clock divide by 2

	ADC0_SC3 = 0; // Reset SC3

	ADC0_SC1A |= ADC_SC1_ADCH(31); // Disable module
}


/* ************************************************** */
/* Method name: 	   adc_initConvertion             */
/* Method description: init a conversion from D to A  */
/* Input params:	   n/a 							  */
/* Outpu params:	   n/a 							  */
/* ************************************************** */
void adc_initConvertion(void)
{
	/* Sw-trigger is selected -> conversion is initiated following a write to SC1A. Also, configures it with single-ended conversion and ch a4 */
	ADC0_SC1A &= (ADC_SC1_ADCH(ADC_SC1_ADCH_AD4) |
		   ADC_SC1_DIFF(ADC_SC1_DIFF_SINGLEENDED) |
		   ADC_SC1_AIEN(ADC_SC1_AIEN_DISABLE));
}

int adc_getValue(int i)
{
	switch(i)
	{
		case 1: return adc_read(ADC_4);
				break;

		case 2: return adc_read(ADC_5);
				break;

		case 3: return adc_read(ADC_6);
				break;

		case 4: return adc_read(ADC_7);
				break;

		case 5: return adc_read(ADC_10);
				break;
	}

	return 0;
}

/* ************************************************** */
/* Method name: 	   adc_isAdcDone                  */
/* Method description: check if conversion is done    */
/* Input params:	   n/a 		                      */
/* Outpu params:	   n/a 			                  */
/* ************************************************** */
int adc_isAdcDone(void)
{
   if(ADC0_SC1A_COCO) /* watch complete conversion flag */
      return 1; /* if the conversion is complete, return 1 */
   else
      return 0; /* if the conversion is still taking place, return 0 */
}


/* */
uint16_t adc_read(uint16_t ch)
{
	ADC0_SC1A = ch & ADC_SC1_ADCH_MASK; //Write to SC1A to start conversion
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK); // Conversion in progress
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); // Run until the conversion is complete
	return ADC0_RA;
}

/* ************************************************** */
/* Method name: 	   adc_getConvertionValue         */
/* Method description: retrieve converted value       */
/* Input params:	   n/a 			                  */
/* Outpu params:	   n/a 			                  */
/* ************************************************** */
int adc_getConvertionValue(void)
{
   return ADC_R_D(ADC0_RA); // return the register value that keeps the result of convertion
}

/* ************************************************** */
/* Method name: 	   adc_convertAdc2Temp            */
/* Method description: retrieve temperature           */
/*                     correspondent to ADC           */
/*                     converted value                */
/* Input params:	   iAdc = 8-bit ADC converted     */
/* Outpu params:	   iTemp = temperature in         */
/*                     Celsius degrees                */
/* ************************************************** */
int adc_convertAdc2Temp(int iAdc)
{
	int iTemp;
	iTemp = tabela_temp[iAdc];
	return iTemp;
}

