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
#define WHEEL_RADIUS 34 //in mm
#define ENC_PRECISION 10 //pulses per turn
#define MEASURE_BUFFER_SIZE 4
#define PI 3.141598
static unsigned int uiaMEASURES[2] = {0,0};
static unsigned int uiaMeasureBuffer[MEASURE_BUFFER_SIZE][2];


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
 * Triggers a measurement
 */
void encoder_measure(){
	uiaMEASURES[0] = TPM_RD_CNT(ENCODER_0_TPM_BASE);
	TPM_WR_CNT(ENCODER_0_TPM_BASE, 0x0);	/*Reset counter */
	uiaMEASURES[1] = TPM_RD_CNT(ENCODER_1_TPM_BASE);;
	TPM_WR_CNT(ENCODER_1_TPM_BASE, 0x0);
	for(int i = 0; i < MEASURE_BUFFER_SIZE -1; i++){
		uiaMeasureBuffer[i+1][0] = uiaMeasureBuffer[i][0];
		uiaMeasureBuffer[i+1][1] = uiaMeasureBuffer[i][1];
	}
	uiaMeasureBuffer[0][0] = uiaMEASURES[0];
	uiaMeasureBuffer[0][1] = uiaMEASURES[1];
}

/**
 * @param usEncoderNumber encoder identifier
 * @param uiPeriod Time ellapsed between measure calls in us
 *
 * @return current encoder speed in mm/sec for given encoder number (from last measure)
 */
unsigned int encoder_getCurrentSpeed(unsigned short usEncoderNumber, unsigned int uiPeriod){
	return (uiaMEASURES[usEncoderNumber]*2*PI*WHEEL_RADIUS/ENC_PRECISION)*1000000/uiPeriod;
}

/**
 * @return linear distance walked by the CG (from last measure)
 */
unsigned int encoder_getLinDistance(){
	return (uiaMEASURES[0] + uiaMEASURES[1])*PI*WHEEL_RADIUS/ENC_PRECISION;
}


/**
 * @param usEncoderNumber encoder identifier
 * @param uiPeriod Time ellapsed between measure calls in us
 *
 * @return mean encoder speed (for last MEASURE_BUFFER_SIZE measures) in mm/sec for given encoder number
 */
unsigned int encoder_getMeanSpeed(unsigned short usEncoderNumber, unsigned int uiPeriod){
	unsigned int uiMean = 0;
	for(int i = 0; i < MEASURE_BUFFER_SIZE; i++){
		uiMean += uiaMeasureBuffer[i][usEncoderNumber];
	}
	return ((uiMean*2*PI*WHEEL_RADIUS/ENC_PRECISION)/MEASURE_BUFFER_SIZE)*1000000/uiPeriod;
}
