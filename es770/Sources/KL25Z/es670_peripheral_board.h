/* ***************************************************************** */
/* File name:        es670_peripheral_board.h                        */
/* File description: Header file containing the peripherals mapping  */
/*                     of the peripheral board for the ES670 hardware*/
/* Author name:      dloubach                                        */
/* Creation date:    16out2015                                       */
/* Revision date:    20jun2018 by fcpinheiro,lopfrigerio             */
/* ***************************************************************** */

#ifndef SOURCES_ES670_PERIPHERAL_BOARD_H_
#define SOURCES_ES670_PERIPHERAL_BOARD_H_

/* system includes */
#include <MKL25Z4.h>

/*                 General uC definitions                 */

/* Clock gate control */
#define CGC_CLOCK_DISABLED          0x00U
#define CGC_CLOCK_ENABLED           0x01U

/* GPIO input / output */
#define GPIO_INPUT                  0x00U
#define GPIO_OUTPUT                 0x01U

#define ALT0                        0x00u
#define ALT1                        0X01U
#define ALT3                        0X03U
#define ALT4                        0X04U
#define FTM_CLKIN0                  ALT4


/*                 END OF General uC definitions         */

/*                 Fan definitions              */

#define FAN_PORT_BASE_PNT           PORTA
#define FAN_GPIO_BASE_PNT           PTA

#define FAN_PIN                     13U
#define FAN_GPIO_ALT                ALT1
#define FAN_PWM_ALT                 ALT3
#define FAN_DIR                     (GPIO_OUTPUT << FAN_PIN)

/*                 END OF Fan definitions              */

/*                 Heater definitions                  */

#define HEATER_PORT_BASE_PNT           PORTA
#define HEATER_GPIO_BASE_PNT           PTA

#define HEATER_PIN                     12U
#define HEATER_PWM_ALT                 ALT3
#define HEATER_DIR                     (GPIO_OUTPUT << HEATER_PIN)

/*                 END OF Heater definitions            */

/*                 Temperature sensor definitions       */
#define TEMPSENSOR_PORT_BASE_PNT       PORTE

#define TEMPSENSOR_PIN                 21U
#define VOLTSENSOR_PIN                 20U
#define TEMPSENSOR_ADC_ALT             ALT0
#define VOLTSENSOR_ADC_ALT             ALT0

/* ADC Definitions */
#define ADC_CFG1_ADICLK_BUS		       0u
#define ADC_CFG1_ADICLK_BUS2		   1u
#define ADC_CFG1_MODE_8BIT             0u
#define ADC_CFG1_ADLSMP_SHORT          0u
#define ADC_CFG1_ADIV_1                0u
#define ADC_CFG1_ADLPC_NORMAL          0u

#define ADC_SC2_REFSEL_EXT             0u
#define ADC_SC2_DMAEN_DISABLE          0u
#define ADC_SC2_ACFE_DISABLE           0u
#define ADC_SC2_ADTRG_SOFT             0u
#define ADC_SC2_ADACT_INPROGRESS       1u
#define ADC_SC2_ADACT_NOTINPROGRESS    0u

#define ADC_CFG2_ADLSTS_DEFAULT        0U
#define ADC_CFG2_ADHSC_NORMAL          0u
#define ADC_CFG2_ADACKEN_DISABLE       0u
#define ADC_CFG2_MUXSEL_ACHS           0u

#define ADC_SC1_DIFF_SINGLEENDED       0u
#define ADC_SC1_AIEN_DISABLE           0U
#define ADC_SC1_ADCH_AD4               4u
#define ADC_SC1_COCO_COMPLETE          1u

/*                 TPM definitions                     */

#define TPM_SC_PS_1                     0u
#define TPM_SC_CMOD_RSNGEDGE            2u
#define TPM_SC_CMOD_EVRCNTCLK           1u
#define TPM_SC_CPWMS_UPCNT              0u
#define TPM_CNSC_MSB_PWM                1u
#define TPM_CNSC_MSA_PWM                0u
#define TPM_CNSC_ELSB_PWM               1u
#define TPM_CNSC_ELSA_PWM               0u
#define SIM_SOPT2_TPMSRC_OSCERCCLK      2u
#define SIM_SOPT2_CLKOUTSEL_OSCERCCLK   6u
#define SIM_SOPT4_TPMCLKSEL_TPM_CLKIN0  0u

/* definitions for input capture mode*/
#define TPM_SC_CPWMS_INPUT_CAPTURE      0u
#define TPM_CnSC_MSA_INPUT_CAPTURE      0u
#define TPM_CnSC_MSB_INPUT_CAPTURE      0u
#define TPM_CnSC_ELSA_INPUT_CAPTURE     1u
#define TPM_CnSC_ELSB_INPUT_CAPTURE     1u

/*                 END OF TPM0 definitions             */

/*                 Tacometer1 definitions               */

#define TACOMETER1_PORT_BASE_PNT     PORTE
#define TACOMETER1_GPIO_BASE_PNT     PTE

#define TACOMETER1_PIN               29U
#define TACOMETER1_ALT               FTM_CLKIN0
#define TACOMETER1_DIR               GPIO_INPUT

#define TACOMETER1_TPM_BASE			TPM0

/*                 END OF Tacometer1 definitions       */


/*                 Tacometer2 definitions               */

#define TACOMETER2_PORT_BASE_PNT     PORTE
#define TACOMETER2_GPIO_BASE_PNT     PTE

#define TACOMETER2_PIN               30U
#define TACOMETER2_ALT               FTM_CLKIN1
#define TACOMETER2_DIR               GPIO_INPUT

#define TACOMETER2_TPM_BASE			TPM0

/*                 END OF Tacometer2 definitions       */

#define OUTPUT_LED_BASE14			PTE
#define	OUTPUT_LED_BASE5			PTB

#define OUTPUT_LED_PIN1					5U
#define OUTPUT_LED_PIN1_DIR             (GPIO_OUTPUT << OUTPUT_LED_PIN1)

#define OUTPUT_LED_PIN2					4U
#define OUTPUT_LED_PIN1_DIR             (GPIO_OUTPUT << OUTPUT_LED_PIN2)

#define OUTPUT_LED_PIN3					3U
#define OUTPUT_LED_PIN1_DIR             (GPIO_OUTPUT << OUTPUT_LED_PIN3)

#define OUTPUT_LED_PIN4					2U
#define OUTPUT_LED_PIN1_DIR             (GPIO_OUTPUT << OUTPUT_LED_PIN4)

#define OUTPUT_LED_PIN5					11U
#define OUTPUT_LED_PIN1_DIR             (GPIO_OUTPUT << OUTPUT_LED_PIN5)

/*                 LED and SWITCH Definitions                    */
//#define LS_PORT_BASE_PNT            PORTA                                   /* peripheral port base pointer */
//#define LS_GPIO_BASE_PNT            PTA                                     /* peripheral gpio base pointer */

/* THIS PIN CONFLICTS WITH PTA1 USED AS UART0_RX IN THE OPENSDA SERIAL DEBUG PORT */
//#define LS1_PIN                     1U                                      /* led/switch #1 pin */
//#define LS1_DIR_OUTPUT              (GPIO_OUTPUT << LS1_PIN)
//#define LS1_DIR_INPUT               (GPIO_OUTPUT << LS1_PIN)
//#define LS1_ALT                     0x01u                                   /* GPIO alternative */



/*                 END OF LED and SWITCH definitions             */


#endif /* SOURCES_ES670_PERIPHERAL_BOARD_H_ */
