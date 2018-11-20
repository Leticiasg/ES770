#include "KL25Z/es670_peripheral_board.h"
#include "timer_counter.h"
#include "MKL25Z4.h"
#include "motores.h"

#define DA_DIR_OUTPUT 0x1 << 0
#define DB_DIR_OUTPUT 0x1 << 1
#define DC_DIR_OUTPUT 0x1 << 2
#define DD_DIR_OUTPUT 0x1 << 3


/* A função inicializa o TPM_1 para ser utilizado como pwm para os dois motores, o valor de idutyCycle recebe o
 *valor inicial do pwm.
 *Essa função utiliza as portas PTA12 e PTA13 para os 2 pwm */
void pwm_init(double idutyCycle1,double idutyCycle2)
{
	/* un-gate port clock*/
	SIM_SCGC6 |= SIM_SCGC6_TPM1(CGC_CLOCK_ENABLED); //Enable clock for TPM1
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(0x2);

	//up counting mode
	TPM1_SC |= TPM_SC_CPWMS(0x0u);
	//LPTPM Counter increments on every LPTPM counter clock
	TPM1_SC |= TPM_SC_CMOD(0x01u);
	//Prescale 1:1
	TPM1_SC |= TPM_SC_PS(0x00u);

	/*Usar edged aligned*/
	TPM1_C1SC |= TPM_CnSC_MSB(0x01u)| TPM_CnSC_MSA(0x00u) | TPM_CnSC_ELSB(0x01u)| TPM_CnSC_ELSA(0x00u);

	/*Zera o contador do LPTPM*/
	TPM1_CNT = TPM_CNT_COUNT(0x0u);

	/*Duty cycle*/
	TPM1_MOD = TPM_MOD_MOD(4*9999+3);
	//TPM1_C1V = TPM_CnV_VAL(idutyCycle); //PWM 75%

	/* Channel 0 Config - Heater */
	TPM1_C0SC |= (TPM_CnSC_MSB(0b1) | TPM_CnSC_MSA(0b0) | TPM_CnSC_ELSB(0b1) | TPM_CnSC_ELSA(0b0));

	/*Seta o pwm inicial*/
	idutyCycle1 = ((idutyCycle1)*(TPM1_MOD+1))/100;
	idutyCycle2 = ((idutyCycle2)*(TPM1_MOD+1))/100;
	TPM1_C0V = (int)idutyCycle1*(idutyCycle1>0);
	TPM1_C1V = (int)idutyCycle2*(idutyCycle2>0);

//	TPM1_C0V = (int)0x10;
//	TPM1_C1V = (int)0x10;

	/* un-gate port clock*/
	SIM_SCGC5 |= SIM_SCGC5_PORTA(CGC_CLOCK_ENABLED);

	/*Pino PTA13 se torna pwm*/
	PORTA_PCR13 |= PORT_PCR_MUX(0x03);

	/*Pino PTA12 se torna pwm*/
	PORTA_PCR12 |= PORT_PCR_MUX(0x03);
}


/*Troca o valor do pwm de cada lado*/
void seta_pwm(int motor, double idutyCycle)
{
	idutyCycle = ((idutyCycle)*(TPM1_MOD+1))/100;

	if(motor == 0){
		TPM1_C1V = (int)idutyCycle;
	}else{
		TPM1_C0V = (int)idutyCycle;
	}
}
//
//
///*Inicializa saidas de controle de direção para os motores*/
void motor_init()
{
	/*inicializa os pwm sempre em 75%*/
	pwm_init(20,30);

	/*Liberar o clock para o PORT C*/
	SIM_SCGC5 |= SIM_SCGC5_PORTC(CGC_CLOCK_ENABLED);

	/*Vamos usar PTC0, PTC1, PTC2 e PTC3*/
	PORTC_PCR0 = PORT_PCR_MUX(0x1);
	PORTC_PCR1 = PORT_PCR_MUX(0x1);
	PORTC_PCR2 = PORT_PCR_MUX(0x1);
	PORTC_PCR3 = PORT_PCR_MUX(0x1);

	/*Escolhea a direção dos pinos*/
	GPIOC_PDDR |= GPIO_PDDR_PDD(DA_DIR_OUTPUT | DB_DIR_OUTPUT | DC_DIR_OUTPUT | DD_DIR_OUTPUT);

	/*Todos os sinais baixos*/
	GPIOC_PCOR |= GPIO_PCOR_PTCO( DA_DIR_OUTPUT );
	GPIOC_PCOR |= GPIO_PCOR_PTCO( DB_DIR_OUTPUT );
	GPIOC_PCOR |= GPIO_PCOR_PTCO( DC_DIR_OUTPUT );
	GPIOC_PCOR |= GPIO_PCOR_PTCO( DD_DIR_OUTPUT );
}

/*Se motor = 1 é o motor direito, se motor = 0 é o motor esquerdo
 *Se direção = 1 vai para a frente, se o motor = 0 ele vai para trás */
void seta_motor(int motor, int direcao, int idutyCycle)
{

	/*Caso a entrada não seja válida ele retorna sem fazer nada*/
	if((motor != 0 && motor != 1)||(direcao != 0 && direcao != 1)){
		return;
	}

	/*Arruma direção*/
	if(idutyCycle < 0){
		idutyCycle = -idutyCycle;
		if(direcao == 1){
			direcao = 0;
		}else{
			direcao= 1;
		}
	}

	if(motor == 1){
		/*Motor esquerdo*/
		if(direcao == 0){
			/*Gira para trás*/
			GPIOC_PCOR |= GPIO_PCOR_PTCO( DB_DIR_OUTPUT );
			GPIOC_PSOR |= GPIO_PSOR_PTSO( DA_DIR_OUTPUT );
		}else{
			/*Vai para frente*/
			GPIOC_PCOR |= GPIO_PCOR_PTCO( DA_DIR_OUTPUT );
			GPIOC_PSOR |= GPIO_PSOR_PTSO( DB_DIR_OUTPUT );
		}
	}else{
		/*Motor direito*/
		if(direcao == 0){
			/*Gira para trás*/
			GPIOC_PCOR |= GPIO_PCOR_PTCO( DD_DIR_OUTPUT );
			GPIOC_PSOR |= GPIO_PSOR_PTSO( DC_DIR_OUTPUT );
		}else{
			/*Vai para frente*/

			GPIOC_PCOR |= GPIO_PCOR_PTCO( DC_DIR_OUTPUT );
			GPIOC_PSOR |= GPIO_PSOR_PTSO( DD_DIR_OUTPUT );
		}
	}

	seta_pwm(motor, idutyCycle);
}
//
//

