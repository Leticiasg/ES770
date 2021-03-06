/* ***************************************************************** */
/* File name:        pid.c                                           */
/* File description: This file has a couple of useful functions to   */
/*                   operate a PID controller                        */
/* Author name:      fcpinheiro,lsgoncalves                          */
/* Creation date:    nov2018                                         */
/* Revision date:    nov2018                                         */
/* ***************************************************************** */

#include "pid.h"

/* ****************************************************** */
/* Method name:        pid_PidInitialize                  */
/* Method description: Initialize PID controller struct   */
/* Input params:       pdtPidData = PID controller struct */
/* Output params:      n/a                                */
/* ****************************************************** */
void pid_PidInitialize(PID_DATA* pdtPidData)
{
	pdtPidData->dKp = 10.0;
	pdtPidData->dKi = 0.0;
	pdtPidData->dKd = 20.0;
	pdtPidData->dSensorVal = 0.0;
	pdtPidData->dErrorSum = 0.0;
}

/* ****************************************************** */
/* Method name:        pid_PidUpdate                      */
/* Method description: Generate controlling signal        */
/* Input params:       pdtPidData = PID controller struct */
/*                     dSensorValue = sensor read value   */
/*		       dReferenceValue = reference value  */
/* Output params:      n/a                                */
/* ****************************************************** */
double pid_PidUpdate(PID_DATA* pdtPidData, double dSensorValue, double dReferenceValue)
{
	double dPterm, dIterm, dDterm;
	double dError, dDifference;

	dError = dReferenceValue - dSensorValue;

	/* proportional term */
	dPterm = pdtPidData->dKp * dError;

	/* accumulated error */
	pdtPidData->dErrorSum += dError;

	/* integral term */
	dIterm = pdtPidData->dKi * pdtPidData->dErrorSum;

	dDifference = pdtPidData->dSensorVal - dSensorValue;

	/* update for next iteration */
	pdtPidData->dSensorVal = dSensorValue;

	/* derivative term */
	dDterm = pdtPidData->dKd * dDifference;

	return dPterm + dIterm + dDterm;
}


