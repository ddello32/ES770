/* ***************************************************************** */
/* File name:        tacometro_hal.c                                 */
/* File description: File containing the functions/methods           */
/*                   for tacometer				     */
/* Author name:      ddello		                             */
/* Creation date:    13maio2016                                      */
/* Revision date:    13maio2016                                      */
/* ***************************************************************** */
#include <Encoder/encoder_hal.h>
#include "KL25Z/es770_peripheral_board.h"

#define ENCODER_SCALER 4

/**
 * Initialize the encoder module
 */
void encoder_init(void){
	/* Init encoder pin */
	SIM_SCGC5 |= SIM_SCGC5_PORTC(1u);
	PORT_PCR_REG(ENCODER_PORT_BASE_PNT , ENCODER_0_PIN) = PORT_PCR_MUX(ENCODER_0_PIN_ALT);
	PORT_PCR_REG(ENCODER_PORT_BASE_PNT , ENCODER_1_PIN) = PORT_PCR_MUX(ENCODER_1_PIN_ALT);

	/* INIT TPM */
	//Init TPM Clock source as OSCERCLOCK
	SIM_WR_SOPT2_TPMSRC(SIM,0x2);
	//TPM0 ClockIn0
	SIM_WR_SOPT4_TPM0CLKSEL(SIM, 0x0);
	//TPM2 ClockIn1
	SIM_WR_SOPT4_TPM2CLKSEL(SIM, 0x1);
	//Enable clock to TPM
	SIM_SCGC6 |= SIM_SCGC6_TPM0(0x1);
	SIM_SCGC6 |= SIM_SCGC6_TPM2(0x1);
	//Select External Clock Source for TPM module
	TPM_WR_SC_CMOD(ENCODER_0_TPM_BASE, 0x2);
	TPM_WR_SC_CMOD(ENCODER_1_TPM_BASE, 0x2);
	//Prescale 1:1
	TPM_WR_SC_PS(ENCODER_0_TPM_BASE, 0x0);
	TPM_WR_SC_PS(ENCODER_1_TPM_BASE, 0x0);
	//Init mod as maximum value
	TPM_WR_MOD(ENCODER_0_TPM_BASE, 0xFFFF);
	TPM_WR_MOD(ENCODER_1_TPM_BASE, 0xFFFF);
}


/**
 * @param usEncoderNumber encoder identifier
 * @param uiPeriod Time ellapsed since last getSpeedCall in ms
 *
 * @return current encoder speed in rps for given encoder number
 */
unsigned int encoder_getSpeed(unsigned short usEncoderNumber, unsigned int uiPeriod){
	unsigned int ret;
	if(usEncoderNumber == 0){
		ret = TPM_RD_CNT(ENCODER_0_TPM_BASE)/ENCODER_SCALER*1000/uiPeriod;
		TPM_WR_CNT(ENCODER_0_TPM_BASE, 0x0);	/*Reset counter */
	}else {
		ret = TPM_RD_CNT(ENCODER_1_TPM_BASE)/ENCODER_SCALER*1000/uiPeriod;
		TPM_WR_CNT(ENCODER_1_TPM_BASE, 0x0);	/*Reset counter */
	}
	return ret;
}
