/*
 * motores.h
 *
 *  Created on: 06/11/2018
 *      Author: aluno
 */

#ifndef SOURCES_UTIL_MOTORES_H_
#define SOURCES_UTIL_MOTORES_H_

void pwm_init(double idutyCycle1,double idutyCycle2);
void seta_pwm(int motor, double idutyCycle);
void motor_init();
void seta_motor(int motor, int direcao, int idutyCycle);




#endif /* SOURCES_UTIL_MOTORES_H_ */
