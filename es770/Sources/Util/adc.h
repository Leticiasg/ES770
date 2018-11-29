/* ***************************************************************** */
/* File name:        adc.h                                           */
/* File description: Header file containing the function/methods     */
/*                   prototypes of adc.c                             */
/* Author name:      fcpinheiro,lopfrigerio                          */
/* Creation date:    07jun2018                                       */
/* Revision date:    20jun2018                                       */
/* ***************************************************************** */


#define ADC_0	0	//  PTE20
#define ADC_1	4	//  PTE21
#define ADC_2	3	//  PTE22
#define ADC_3	7	//  PTE23
#define ADC_4	8	//  PTB0
#define ADC_5	9	//  PTB1
#define ADC_6	12	//  PTB2
#define ADC_7	13	//  PTB3
#define ADC_8	14	//  PTC0
#define ADC_9	15	//  PTC1
#define ADC_10 11	//  PTC2
#define ADC_11 5	//  PTD1   LED AZUL FRDM-KL25Z
#define ADC_12 6	//  PTD5
#define ADC_13 7	//  PTD6

#ifndef UTIL_ADC_H_
#define UTIL_ACD_H_

/* ************************************************** */
/* Method name: 	   adc_initADCModule          */
/* Method description: configure ADC module           */
/* Input params:	   n/a 			      */
/* Outpu params:	   n/a 			      */
/* ************************************************** */
void adc_initADCModule(void);


/* ************************************************** */
/* Method name: 	   adc_initConvertion             */
/* Method description: init a conversion from D to A  */
/* Input params:	   n/a 							  */
/* Outpu params:	   n/a 							  */
/* ************************************************** */
void adc_initConvertion(void);


/* ************************************************** */
/* Method name: 	   adc_isAdcDone                  */
/* Method description: check if conversion is done    */
/* Input params:	   n/a 		                      */
/* Outpu params:	   n/a 			                  */
/* ************************************************** */
int adc_isAdcDone(void);


/* ************************************************** */
/* Method name: 	   adc_getConvertionValue         */
/* Method description: retrieve converted value       */
/* Input params:	   n/a 			                  */
/* Outpu params:	   n/a 			                  */
/* ************************************************** */
int adc_getConvertionValue(void);

/* ************************************************** */
/* Method name: 	   adc_convertAdc2Temp            */
/* Method description: retrieve temperature           */
/*                     correspondent to ADC           */
/*                     converted value                */
/* Input params:	   iAdc = 8-bit ADC converted     */
/* Outpu params:	   iTemp = temperature in         */
/*                     Celsius degrees                */
/* ************************************************** */
int adc_convertAdc2Temp(int iAdc);

uint16_t adc_read(uint16_t ch);

int adc_getValue(int i);

uint16_t adc_read2(void);

void adc_initConvertion2(uint16_t ch);

int adc_normalizeReadValue10(int iVal, int iMax);

#endif /* UTIL_ADC_H_ */
