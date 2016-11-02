/* ***************************************************************** */
/* File name:        motor_hal.c                                     */
/* File description: File containing the functions/methods   		 */
/*                   for motor control	  						     */
/* Author name:      ddello		                                     */
/* Creation date:    13maio2016                                      */
/* Revision date:    13maio2016                                      */
/* ***************************************************************** */

#include "motor_hal.h"
#include "KL25Z/es770_peripheral_board.h"
#include "GPIO/gpio_hal.h"
#include <stdint.h>

/**
 * Convert given speed value into DC speed value
 * @param iDesiredSpeed desired speed (from -0xFFFF to 0xFFFF)
 */
uint16_t motor_speedToDc(int iDesiredSpeed) {
	uint16_t uiOutSpeed = 0;
	if(iDesiredSpeed < -0xFFFF) {
		uiOutSpeed = 0xFFFF;
	}else if(iDesiredSpeed > 0xFFFF) {
		uiOutSpeed = 0xFFFF;
	}else if(iDesiredSpeed < 0){
		uiOutSpeed = (uint16_t) ((-1*iDesiredSpeed) & 0xFFFF);
	}else{
		uiOutSpeed = (uint16_t) (iDesiredSpeed & 0xFFFF);
	}
	return uiOutSpeed;
}

/**
 * Initialize motor tpm module
 */
void motor_initTpm(void){
	/* Init tpm module */
	SIM_SCGC5 |= SIM_SCGC5_PORTA(1u);
	PORT_PCR_REG(MOTOR_0_PORT_BASE_PNT , MOTOR_0_PIN) = PORT_PCR_MUX(MOTOR_0_PIN_MUX_ALT);
	PORT_PCR_REG(MOTOR_1_PORT_BASE_PNT , MOTOR_1_PIN) = PORT_PCR_MUX(MOTOR_1_PIN_MUX_ALT);

	/* INIT TPM */
	//Init TPM Clock source as OSCERCLOCK
	SIM_WR_SOPT2_TPMSRC(SIM,0x2);
	//Enable clock to TPM1
	SIM_SCGC6 |= SIM_SCGC6_TPM1(0x1);
	//PWM mode
	TPM_WR_SC_CPWMS(MOTOR_TPM_BASE_PNT, 0x0);	//Edge Aligned

	// Config motor 0 Channel mode
	/* Channel mode configuration should be done with the channel disabled and need acknowledge*/
	TPM_WR_CnSC(MOTOR_TPM_BASE_PNT, MOTOR_0_TPM_CHANNEL_INDEX, TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
    /* Wait till mode change is acknowledged */
    while (!
    		(
				TPM_RD_CnSC(MOTOR_TPM_BASE_PNT, MOTOR_0_TPM_CHANNEL_INDEX)
				& (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1))
			)
		  );
	//motor 1 Channel mode
	TPM_WR_CnSC(MOTOR_TPM_BASE_PNT, MOTOR_1_TPM_CHANNEL_INDEX, TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));
    /* Wait till mode change is acknowledged */
    while (!
    		(
				TPM_RD_CnSC(MOTOR_TPM_BASE_PNT, MOTOR_1_TPM_CHANNEL_INDEX)
				& (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1))
			)
		  );
	//Same period as OSCRCLK
	TPM_WR_MOD(MOTOR_TPM_BASE_PNT, 0xFFFE);
	//Duty cycle
	TPM_WR_CnV_VAL(MOTOR_TPM_BASE_PNT, MOTOR_0_TPM_CHANNEL_INDEX, 0x0);
	TPM_WR_CnV_VAL(MOTOR_TPM_BASE_PNT, MOTOR_1_TPM_CHANNEL_INDEX, 0x0);
	//Prescale 1:1
	TPM_WR_SC_PS(MOTOR_TPM_BASE_PNT, 0x0);
	//Select LTPM Clock Source for TPM module
	TPM_WR_SC_CMOD(MOTOR_TPM_BASE_PNT, 0x1);
}

/**
 * Initialize the HBridge direction control module
 */
void motor_initHBridge(void){
	GPIO_UNGATE_PORT(MOTOR_HBRIDGE_PORT_ID);
	GPIO_INIT_PIN(MOTOR_HBRIDGE_PORT_ID, MOTOR_0_A_PIN, GPIO_OUTPUT);
	GPIO_WRITE_PIN(MOTOR_HBRIDGE_PORT_ID, MOTOR_0_A_PIN, GPIO_HIGH);
	GPIO_INIT_PIN(MOTOR_HBRIDGE_PORT_ID, MOTOR_0_B_PIN, GPIO_OUTPUT);
	GPIO_WRITE_PIN(MOTOR_HBRIDGE_PORT_ID, MOTOR_0_B_PIN, GPIO_LOW);
	GPIO_INIT_PIN(MOTOR_HBRIDGE_PORT_ID, MOTOR_1_A_PIN, GPIO_OUTPUT);
	GPIO_WRITE_PIN(MOTOR_HBRIDGE_PORT_ID, MOTOR_1_A_PIN, GPIO_HIGH);
	GPIO_INIT_PIN(MOTOR_HBRIDGE_PORT_ID, MOTOR_1_B_PIN, GPIO_OUTPUT);
	GPIO_WRITE_PIN(MOTOR_HBRIDGE_PORT_ID, MOTOR_1_B_PIN, GPIO_LOW);
}

/**
 * Initialize the motor module
 */
void motor_init(void){
	motor_initHBridge();
	motor_initTpm();
}


/**
 * Sets the direction for the given motor
 * @param usMotorNumber motor identifier (0, 1)
 * @paramm usDir the direction (0 for reverse, 1 for direct)
 */
void motor_setDir(unsigned short usMotorNumber, unsigned short usDir){
	if(usMotorNumber == 0){
		GPIO_WRITE_PIN(MOTOR_HBRIDGE_PORT_ID, MOTOR_0_A_PIN, usDir ? GPIO_HIGH : GPIO_LOW);
		GPIO_WRITE_PIN(MOTOR_HBRIDGE_PORT_ID, MOTOR_0_B_PIN, usDir ? GPIO_LOW : GPIO_HIGH);
	}else {
		GPIO_WRITE_PIN(MOTOR_HBRIDGE_PORT_ID, MOTOR_1_A_PIN, usDir ? GPIO_HIGH : GPIO_LOW);
		GPIO_WRITE_PIN(MOTOR_HBRIDGE_PORT_ID, MOTOR_1_B_PIN, usDir ? GPIO_LOW : GPIO_HIGH);
	}
}

/**
 * Set the motor speed
 * @param usMotorNumber motor identifier (0, 1)
 * @param iSpeed the desired speed for this motor (from -0xFFFF to 0xFFFF)
 */
void motor_setSpeed(unsigned short usMotorNumber, int iSpeed){
	motor_setDir(usMotorNumber, iSpeed < 0 ? 0:1);
	if(usMotorNumber == 0) {
		TPM_WR_CnV_VAL(MOTOR_TPM_BASE_PNT, MOTOR_0_TPM_CHANNEL_INDEX, motor_speedToDc(iSpeed));
	}else {
		TPM_WR_CnV_VAL(MOTOR_TPM_BASE_PNT, MOTOR_1_TPM_CHANNEL_INDEX, motor_speedToDc(iSpeed));
	}
}

/**
 * Saves the motor calibration params
 * @param usMotorNumber motor identifier (0, 1)
 * @param iMinDirectSpeedRef the minimum speed ref to get the motor running in direct speed
 * @param uiMaxDirectMeasuredSpeed the speed measured when the motor was in maximum direct speed (in pulses by sec)
 * @param iMinInverseSpeedRef the minimum speed ref to get the motor running in inverse speed
 * @param uiMaxInverseMeasuredSpeed the speed measured when the motor was in maximum inverse speed (in pulses by sec)
 */
void motor_calibrate(unsigned short usMotorNumber, int iMinDirectSpeedRef, unsigned int uiMaxDirectMeasuredSpeed, int iMinInverseSpeedRef, unsigned int uiMaxInverseMeasuredSpeed){

}
