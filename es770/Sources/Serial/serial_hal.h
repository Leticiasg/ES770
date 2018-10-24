/* *********************************************************************** */
/* File name:        serial_hal.h                                          */
/* File description: This file contains headers of functions for           */
/*                   control of read and write serial comm operations      */
/* Author name:      fcpinheiro,lopfrigerio                                */
/* Creation date:    19apr2018                                             */
/* Revision date:    25apr2018                                             */
/* *********************************************************************** */

#ifndef SOURCES_SERIAL_SERIAL_HAL_H_
#define SOURCES_SERIAL_SERIAL_HAL_H_

/* ************************************************************ */
/* Method name:        serial_setConfig                         */
/* Method description: Initialize the Uart serial communication */
/* Input params:       n/a                                      */
/* Output params:      n/a                                      */
/* ************************************************************ */
void serial_setConfig();

/* ************************************************************ */
/* Method name:        serial_sendBuffer                        */
/* Method description: Send character to host via serial comm   */
/* Input params:       cRetChar = char to be sent               */
/* Output params:      n/a                                      */
/* ************************************************************ */
void serial_sendBuffer(char cRetChar);

/* *************************************************************** */
/* Method name:        serial_sendBuffer                           */
/* Method description: Receive character to host via serial comm   */
/* Input params:       n/a                                         */
/* Output params:      ucReceivedBuffer = char to be interpreted   */
/* *************************************************************** */
int serial_receiveBuffer();



#endif /* SOURCES_SERIAL_SERIAL_HAL_H_ */
