/* ***************************************************************** */
/* File name:        serial_hal.c                                    */
/* File description: This file has a couple of useful functions to   */
/*                   control read from and write to serial comm      */
/* Author name:      fcpinheiro,lopfrigerio                          */
/* Creation date:    19apr2018                                       */
/* Revision date:    25apr2018                                       */
/* ***************************************************************** */

#include "serial_hal.h"
#include "fsl_debug_console.h"
#include "Util/debugUart.h"
#include "KL25Z/es670_peripheral_board.h"
#include <MKL25Z4_extension.h>

/* ************************************************************ */
/* Method name:        serial_setConfig                         */
/* Method description: Initialize the Uart serial communication */
/* Input params:       n/a                                      */
/* Output params:      n/a                                      */
/* ************************************************************ */
void serial_setConfig()
{
	debugUart_init();
}


/* ************************************************************ */
/* Method name:        serial_sendBuffer                        */
/* Method description: Send character to host via serial comm   */
/* Input params:       cRetChar = char to be sent               */
/* Output params:      n/a                                      */
/* ************************************************************ */
void serial_sendBuffer(char cRetChar)
{
	PUTCHAR(cRetChar);
}

/* *************************************************************** */
/* Method name:        serial_sendBuffer                           */
/* Method description: Receive character to host via serial comm   */
/* Input params:       n/a                                         */
/* Output params:      ucReceivedBuffer = char to be interpreted   */
/* *************************************************************** */
int serial_receiveBuffer()
{
	unsigned char ucReceivedBuffer = 0;

	if(UART0_BRD_S1_RDRF(UART0))
	{
		ucReceivedBuffer = GETCHAR();
	}

	return ucReceivedBuffer;
}
