/* ***************************************************************** */
/* File name:        es770_peripheral_board.h                        */
/* File description: Header file containing the peripherals mapping  */
/*                     of the peripheral board for the ES770 hardware*/
/* Author name:      ddello32                                        */
/* Creation date:    13set2016                                       */
/* Revision date:    25fev2016                                       */
/* ***************************************************************** */

#ifndef SOURCES_ES770_PERIPHERAL_BOARD_H_
#define SOURCES_ES770_PERIPHERAL_BOARD_H_

/* system includes */
#include <MKL25Z4.h>
#include "MKL25Z4_extension.h"

/*                 General uC definitions                 */

/* Clock gate control */
#define  CGC_CLOCK_DISABLED         0x00U
#define  CGC_CLOCK_ENABLED          0x01U

/* GPIO input / output */
#define GPIO_INPUT                  0x00U
#define GPIO_OUTPUT                 0x01U

#define GPIO_MUX_ALT                0x01u

#define GPIO_HIGH					1
#define GPIO_LOW					0

/*	Workaround for PORT_ID macro expansion to stop at port level*/
typedef int A;
typedef int B;
typedef int C;
typedef int D;
typedef int E;

/*                 END OF General uC definitions         */

/*					MOTOR DEFINITIONS 					  */
//TODO
#define SERVO_PORT_ID				D
#define SERVO_PORT_BASE_PNT			PORTD
#define SERVO_PIN					5
#define SERVO_PIN_MUX_ALT			0x4

#define SERVO_TPM_BASE_PNT			TPM0
#define SERVO_TPM_CHANNEL_INDEX		5


/*					END OF SERVO DEFINITIONS 					  */


/*					TACOMETER DEFINITIONS				*/
//TODO
#define TACOMETRO_PORT_ID			E
#define TACOMETRO_PORT_BASE_PNT		PORTE
#define TACOMETRO_PIN				29
#define TACOMETRO_PIN_ALT			0x4		/*ALT4 FTM_CLOCKIN0*/

#define TACOMETRO_TPM_BASE			TPM0

/*					END OF TACOMETER DEFINITIONS		*/

/*					PH0TO SENSOR DEFINITIONS 					  */
#define ADC_NUM_BASE_PNT			ADC0_BASE_PTR
#define ADC_MUX_IDX					0		//MUX A

#define PH0_PORT_ID					B
#define PH0_PORT_BASE_PNT			PORTB
#define PH0_CHANNEL_SEL				0x8
#define PH0_PIN						0
#define PH0_PIN_MUX_ALT				0x0

#define PH1_PORT_ID					B
#define PH1_PORT_BASE_PNT			PORTB
#define PH1_CHANNEL_SEL				0x9
#define PH1_PIN						1
#define PH1_PIN_MUX_ALT				0x0

#define PH2_PORT_ID					B
#define PH2_PORT_BASE_PNT			PORTB
#define PH2_CHANNEL_SEL				0x12
#define PH2_PIN						2
#define PH2_PIN_MUX_ALT				0x0

#define PH3_PORT_ID					C
#define PH3_PORT_BASE_PNT			PORTC
#define PH3_CHANNEL_SEL				0x3
#define PH3_PIN						22
#define PH3_PIN_MUX_ALT				0x0

#define PH4_PORT_ID					C
#define PH4_PORT_BASE_PNT			PORTC
#define PH4_CHANNEL_SEL				0x4
#define PH4_PIN						21
#define PH4_PIN_MUX_ALT				0x0

#define PH5_PORT_ID					C
#define PH5_PORT_BASE_PNT			PORTC
#define PH5_CHANNEL_SEL				0x0
#define PH5_PIN						20
#define PH5_PIN_MUX_ALT				0x0

#define LD0_PORT_ID					E
#define LD0_PORT_BASE_PNT      		PORTE
#define LD0_GPIO_BASE_PNT      		PTE
#define LD0_PIN                     31
#define LD0_DIR		               (GPIO_OUTPUT << LD0_PIN)
#define LD0_ALT                     GPIO_OUTPUT

#define LD1_PORT_ID					A
#define LD1_PORT_BASE_PNT      		PORTA
#define LD1_GPIO_BASE_PNT      		PTA
#define LD1_PIN                     17
#define LD1_DIR		                (GPIO_OUTPUT << LD1_PIN)
#define LD1_ALT                     GPIO_OUTPUT

#define LD2_PORT_ID					A
#define LD2_PORT_BASE_PNT      		PORTA
#define LD2_GPIO_BASE_PNT      		PTA
#define LD2_PIN                     16
#define LD2_DIR		                (GPIO_OUTPUT << LD2_PIN)
#define LD2_ALT                     GPIO_OUTPUT

#define LD3_PORT_ID					C
#define LD3_PORT_BASE_PNT      		PORTC
#define LD3_GPIO_BASE_PNT      		PTC
#define LD3_PIN                     17
#define LD3_DIR		                (GPIO_OUTPUT << LD3_PIN)
#define LD3_ALT                     GPIO_OUTPUT

#define LD4_PORT_ID					C
#define LD4_PORT_BASE_PNT      		PORTC
#define LD4_GPIO_BASE_PNT      		PTC
#define LD4_PIN                     16
#define LD4_DIR			            (GPIO_OUTPUT << LD4_PIN)
#define LD4_ALT                     GPIO_OUTPUT

#define LD5_PORT_ID					C
#define LD5_PORT_BASE_PNT      		PORTC
#define LD5_GPIO_BASE_PNT      		PTC
#define LD5_PIN                     13
#define LD5_DIR		                (GPIO_OUTPUT << LD5_PIN)
#define LD5_ALT                     GPIO_OUTPUT
/*					END OF PH0TO SENSOR DEFINITIONS 					  */

/*					RGBLED DEFINITIONS 					  */
#define RGBLED_R_PORT_ID				B
#define RGBLED_R_PORT_BASE_PNT			PORTB
#define RGBLED_R_PIN					18
#define RGBLED_R_PIN_MUX_ALT			0x3

#define RGBLED_G_PORT_ID				B
#define RGBLED_G_PORT_BASE_PNT			PORTB
#define RGBLED_G_PIN					19
#define RGBLED_G_PIN_MUX_ALT			0x3

#define RGBLED_B_PORT_ID				D
#define RGBLED_B_PORT_BASE_PNT			PORTD
#define RGBLED_B_PIN					1
#define RGBLED_B_PIN_MUX_ALT			0x4

#define RGBLED_R_TPM_BASE_PNT			TPM2
#define RGBLED_R_TPM_CHANNEL_INDEX		0

#define RGBLED_G_TPM_BASE_PNT			TPM2
#define RGBLED_G_TPM_CHANNEL_INDEX		1

#define RGBLED_B_TPM_BASE_PNT			TPM0
#define RGBLED_B_TPM_CHANNEL_INDEX		1
/*					END OF RGBLED DEFINITIONS 					  */

#endif /* SOURCES_ES770_PERIPHERAL_BOARD_H_ */
