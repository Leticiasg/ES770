/* ***************************************************************** */
/* File name:        pid.h                                           */
/* File description: Header file containing the function/methods     */
/*                   prototypes of pid.c                             */
/* Author name:      fcpinheiro,lopfrigerio                          */
/* Creation date:    21jun2018                                       */
/* Revision date:    21jun2018                                       */
/* ***************************************************************** */
#ifndef SOURCES_UTIL_PID_H_
#define SOURCES_UTIL_PID_H_

/* controller constants, last read sensor value and accumulated error struct */
typedef struct PID_DATA
{
	double dKp,dKi,dKd;
	double dSensorVal;
	double dErrorSum;
} PID_DATA;

/* ****************************************************** */
/* Method name:        pid_PidInitialize                  */
/* Method description: Initialize PID controller struct   */
/* Input params:       pdtPidData = PID controller struct */
/* Output params:      n/a                                */
/* ****************************************************** */
void pid_PidInitialize(PID_DATA* pdtPidData);

/* ****************************************************** */
/* Method name:        pid_PidUpdate                      */
/* Method description: Generate controlling signal        */
/* Input params:       pdtPidData = PID controller struct */
/*                     dSensorValue = sensor read value   */
/*					   dReferenceValue = reference value  */
/* Output params:      n/a                                */
/* ****************************************************** */
double pid_PidUpdate(PID_DATA* pdtPidData, double dSensorValue, double dReferenceValue);

#endif /* SOURCES_UTIL_PID_H_ */
