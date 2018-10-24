/*
 * fan_hal.c
 *
 *  Created on: 02/05/2018
 *      Author: aluno
 */
#include "fan_hal.h"
#include "../KL25Z/es670_peripheral_board.h"

void fan_init()
{
    /* un-gate port clock*/
    SIM_SCGC5 |= SIM_SCGC5_PORTA(CGC_CLOCK_ENABLED);

    /* define pin as gpio */
    PORTA_PCR13 = PORT_PCR_MUX(FAN_GPIO_ALT);

    /* define pin as output */
    GPIOA_PDDR |= GPIO_PDDR_PDD(FAN_DIR);

    /* fan initially off */
    GPIOA_PCOR = GPIO_PCOR_PTCO( (0x01U << FAN_PIN) );
}

void fan_setFanOn()
{
	GPIOA_PSOR = GPIO_PSOR_PTSO( (0x01U << FAN_PIN) );
}

void fan_setFanOff()
{
	GPIOA_PCOR = GPIO_PCOR_PTCO( (0x01U << FAN_PIN) );
}



