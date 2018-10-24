/* ***************************************************************** */
/* File name:        tacometer_hal.h                                 */
/* File description: Header file containing the function/methods     */
/*                   prototypes of tacometer_hal.c                   */
/* Author name:      fcpinheiro,lopfrigerio                          */
/* Creation date:    16may2018                                       */
/* Revision date:    23may2018                                       */
/* ***************************************************************** */

#ifndef SOURCES_UTIL_TACOMETER_HAL_H_
#define SOURCES_UTIL_TACOMETER_HAL_H_

/* ************************************************** */
/* Method name:        tacometer_init                 */
/* Method description: Initiate tacometer implemented */
/*                     on TPM0 module                 */
/* Input params:       n/a                            */
/* Output params:      n/a                            */
/* ************************************************** */
void tacometer_init();

/* *************************************************** */
/* Method name:        tacometer_getSpeed              */
/* Method description: Returns speed based on cooler   */
/*                     physical configuration and main */
/*                     loop time                       */
/* Input params:       uiCeeTime = main loop time in   */ 
/*                     microseconds                    */
/* Output params:      tacometer measured speed in rps */
/* *************************************************** */
int tacometer_getSpeed(unsigned int uiCeeTime);

#endif /* SOURCES_UTIL_TIMER_COUNTER_H_ */
