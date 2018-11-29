/* ***************************************************************** */
/* File name:        main.c                                          */
/* File description: File dedicated to the ES670 prototype projects  */
/*                   involving the FRDM-KL25Z board together with is */
/*                   daughter board containing more peripherals      */
/*                                                                   */
/*                   Processor MKL25Z128VLK4 characteristics         */
/*                   48 MHz ARM Cortex-M0+ core                      */
/*                   128 KB program flash memory                     */
/*                   16 KB SRAM                                      */
/*                   Voltage range: 1.71 to 3.6 V                    */
/*                                                                   */
/* Author name:      fcpinheiro,lopfrigerio                          */
/* Creation date:    02may2018                                       */
/* Revision date:    20jun2018                                       */
/* ***************************************************************** */

/* system includes */
#include "fsl_device_registers.h"

/* our includes */
#include "Util/debugUart.h"
#include "Util/tc_hal.h"
#include "Util/timer_counter.h"
#include "Util/adc.h"
#include "Util/pid.h"
#include "mcg_hal.h"
#include "KL25Z/es670_peripheral_board.h"

/* defines */

#define PORT_PCR_MUX_CLR_MASK	!(PORT_PCR_MUX_MASK)
#define BLACK_VALUE 			5					// valor normalizado do sensor abaixo do qual se considera como leitura do fundo preto
#define MOTOR1_STRAIGHT 		30  				// PWM de reta do motor esquerdo
#define MOTOR2_STRAIGHT 		60 					// PWM de reta do motor direito
#define SENSOR_MAX 				0xd3				// maior valor dos sensores da pre-calibracao
#define NUMBER_OF_CROSSINGS		2					// numero de cruzamentos da linha de chegada a contar
#define MAX_WHITE_TIME			20					// tempo de captacao da linha de chegada / tempo do ECC

 /* in micro seconds = mili seconds * 1000 */
#define CYCLIC_EXECUTIVE_PERIOD    5*1000
#define START_DELAY					150

/* globals */

/* cyclic executive flag */
volatile unsigned int uiFlagNextPeriod = 0;

/* ************************************************ */
/* Method name:        main_cyclicExecuteIsr        */
/* Method description: cyclic executive interrupt   */
/*                     service routine              */
/* Input params:       n/a                          */
/* Output params:      n/a                          */
/* ************************************************ */
void main_cyclicExecuteIsr(void)
{
    /* set the cyclic executive flag */
    uiFlagNextPeriod = 1;
}

/* funcao para desabilitar o pino de interrupcao nao mascaravel padrao */
void main_disablePinNmi(void)
{
	SIM_SCGC5 |= SIM_SCGC5_PORTA(CGC_CLOCK_ENABLED);

	PORTA_PCR4 &= PORT_PCR_MUX_CLR_MASK;
}

int main(void)
{
	/* variavel para contagem da ordem dos sensores */
	int i = 0;

	/* variavel para contagem de ciclos rodados */
	int j = 0;

	int k = 0;

	/* variavel para contagem de tempo sobre linha de chegada */
	int iWhiteTime = 0;

	/* vetor de leitura bruta dos sensores */
	int iSensor[5] = {0,0,0,0,0};

	/* vetor de maximo dos sensores */
	int iSensorMax[5] = {SENSOR_MAX,SENSOR_MAX,SENSOR_MAX,SENSOR_MAX,SENSOR_MAX};

	/* vetor de leitura normalizada (pelo maximo proprio) dos sensores */
	int iNormSensor[5] = {0,0,0,0,0};

	/* vetor de leitura binaria (1=branco,0=preto) dos sensores */
	int iBoolSensor[5] = {0,0,0,0,0};

	/* flag que indica se a primeira calibracao dos sensores esta em curso */
	int iGetMaxVal = 1;

	/* valor ponderado para definicao da posicao na linha (inteiro) */
	int iLineVal = 0;

	/* valor ponderado da linha a esquerda */
	int iLeftVal = 0;

	/* valor ponderado da linha a direita */
	int iRightVal = 0;

	/* pwm a ser imposto ao motor esquerdo */
	int iPwm1 = MOTOR1_STRAIGHT;

	/* pwm a ser imposto ao motor direito */
	int iPwm2 = MOTOR2_STRAIGHT;


	/* flag que indica se o estado anterior dos sensores era todos lendo branco */
	int iWasAllWhite = 1;

	/* variavel para contagem de cruzamentos da linha de chegada */
	int iCrossingsCounter = 0;

	/* flag para indicar se o carrinho deve estar continuar andando (=1) ou parar (=0) */
	int iRun = 1;

	/* valor ponderado para definicao da posicao na linha (inteiro) */
	double dLineVal = 0.0;

	/* variavel para saida do controlador PID */
	double dControl = 0.0;

	/* variavel para salvar ultimo valor de saida do controlador */
	double dLastControl = 0.0;

	/* struct que contem a configuracao e estado do controlador */
	PID_DATA pidStr;

	/* initialization functions */
	mcg_clockInit();
	main_disablePinNmi();
	adc_initADCModule();
	adc_initConvertion2(i);
	pid_PidInitialize(&pidStr);
	timer_initMotor();

	/* functions before loop */
	tc_installLptmr0(CYCLIC_EXECUTIVE_PERIOD, main_cyclicExecuteIsr);

	/* cooperative cyclic executive main loop */
    for (;;)
	{

		/* Se a leitura do sensor atual foi concluida*/
    	if(adc_isAdcDone())
		{
			/* salva o valor lido no vetor */
    		iSensor[i] = adc_read2();

			/*, normaliza seu valor e atualiza o valor de maximo desse caso seja maior que o salvo */
			if(iSensor[i] > iSensorMax[i])
				iSensorMax[i] = iSensor[i];
			iNormSensor[i] = adc_normalizeReadValue10(iSensor[i],iSensorMax[i]);
			iBoolSensor[i] = (iNormSensor[i]>BLACK_VALUE);

			i++;
			if(i>4)
			{
				i=0;

				/* apos ler uma vez todos os sensores, a flag e zerada */
				iGetMaxVal = 0;
			}

			/* inicia a conversao do proximo sensor */
			adc_initConvertion2(i);
		}


    	/* Faz a ponderacao para definicao do valor da linha, que deve ser zero se estiver centralizado e aumentar
    	 * gradativamente conforme a leitura for feita nos sensores mais externos */
		iLeftVal = 3*(iBoolSensor[4] & !iBoolSensor[3]) + (iBoolSensor[4] & iBoolSensor[3])
				+ 2*(!iBoolSensor[4] & iBoolSensor[3]);
		iRightVal = 3*(iBoolSensor[0] & !iBoolSensor[1]) + (iBoolSensor[0] & iBoolSensor[1])
						+ 2*(!iBoolSensor[0] & iBoolSensor[1]);
		iLineVal = iLeftVal - iRightVal;
		dLineVal = (double)iLineVal;

		/* gera a saida de controle, tomando como referencia o valor da linha como 0 */
		dControl = pid_PidUpdate(&pidStr,dLineVal,0.0);

		/* se o carrinho deve prosseguir */
		if(iRun)
		{
			if(iBoolSensor[2])
			{

				/* Se uma linha perpendicular branca for lida por tempo sufuciente (indicando a faixa de chegada),
				 * conta uma volta */
				if(iBoolSensor[0] && iBoolSensor[1] && iBoolSensor[2] &&
						iBoolSensor[3] && iBoolSensor[4])
				{
					iWhiteTime++;
					if(!iWasAllWhite && (iWhiteTime>MAX_WHITE_TIME))
					{
						iWasAllWhite = 1;
						iCrossingsCounter++;
					}
				}
				else
				{
					iWasAllWhite = 0;
				}

				/* Se duas voltas forem contadas, o carrinho deve parar no ciclo seguinte */
				if(iCrossingsCounter>=NUMBER_OF_CROSSINGS)
				{
					timer_initTPM1AsPWM(0,0);
					iRun = 0;
				}

				/* Se o sensor do meio indicar a linha e nao estiver parado, deve seguir reto */
				else
				{
					timer_initTPM1AsPWM(MOTOR1_STRAIGHT,MOTOR2_STRAIGHT);
					iPwm1 = 0;
					iPwm2 = 0;
				}
			}

			/* Se o carrinho nao estiver centralizado, o controle deve operar para corrigir sua posicao.
			 * E dado um tempo inicial de leitura para nao haver problemas com leitura incompleta do vetor de sensores */
			else if(j>=START_DELAY && !iGetMaxVal && (iBoolSensor[0]) ||
					(iBoolSensor[1]) ||
					(iBoolSensor[3]) ||
					(iBoolSensor[4]))
			{
				iWasAllWhite = 0;
				iWhiteTime = 0;
				if(dControl<0)
				{
					iPwm1 = (int)(MOTOR1_STRAIGHT+dControl);
					iPwm2 = (int)(MOTOR2_STRAIGHT);
					dLastControl = dControl;
				}
				else
				{
					iPwm1 = (int)(MOTOR1_STRAIGHT+dControl);
					iPwm2 = (int)(MOTOR2_STRAIGHT-2*dControl);
					dLastControl = dControl;
				}
				timer_initTPM1AsPWM(iPwm1,iPwm2);
			}
			else
			{
				iWasAllWhite = 0;
				iWhiteTime = 0;
				if(dLastControl<0)
				{
					iPwm1 = (int)(MOTOR1_STRAIGHT+dLastControl);
					iPwm2 = (int)(MOTOR2_STRAIGHT);
				}
				else if(dLastControl>0)
				{
					iPwm1 = (int)(MOTOR1_STRAIGHT+dLastControl);
					iPwm2 = (int)(MOTOR2_STRAIGHT-2*dLastControl);
				}
				else
				{
					timer_initTPM1AsPWM(0,0);
				}
				timer_initTPM1AsPWM(iPwm1,iPwm2);
			}
		}

		/* Se as voltas foram completadas, o carrinho deve parar */
		else
		{
			timer_initTPM1AsPWM(0,0);
			return 0;
		}


    	/* WAIT FOR CYCLIC EXECUTIVE PERIOD */
		while(!uiFlagNextPeriod);
		uiFlagNextPeriod = 0;
		j++;

    }
    /* Never leave main */
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
