/*
 * File name: photosensor_hal.c
 *
 *  Created on: 01/10/2016
 *      Author: ddello32
 */

#include "KL25Z/es770_peripheral_board.h"
#include <stdint.h>
#include "GPIO/gpio_hal.h"
#include <fsl_gpio_hal.h>
#include "PhotoSensor/photosensor_hal.h"
#include "Util/util.h"

static GPIO_Type* gtpLEDPORTS[6] = {LD0_GPIO_BASE_PNT, LD1_GPIO_BASE_PNT, LD2_GPIO_BASE_PNT, LD3_GPIO_BASE_PNT, LD4_GPIO_BASE_PNT, LD5_GPIO_BASE_PNT};
static unsigned short usaLEDPINS[6] = {LD0_PIN, LD1_PIN, LD2_PIN, LD3_PIN, LD4_PIN, LD5_PIN};
static unsigned short usaCHANNELS[6] = {PH0_CHANNEL_SEL, PH1_CHANNEL_SEL, PH2_CHANNEL_SEL, PH3_CHANNEL_SEL, PH4_CHANNEL_SEL, PH5_CHANNEL_SEL};
//DC Value offsets (Not adjusting ADC_OFS register to avoid having to handle integer overflow problems)
static uint16_t ui16aOFFSETS[6] = {0,0,0,0,0,0};

//Bus Clock should be 20MHz and Core clock 40Mhz

/**
 * Check if conversion is done
 * @return 1 if done, 0 otherwise
 */
#define photoSensor_isAdcDone() \
	ADC_BRD_SC1_COCO(ADC_NUM_BASE_PNT, ADC_MUX_IDX)

/**
 *  Configure ADC module
 */
void photoSensor_initADCModule(void) {
	/* un-gate port clock*/
	SIM_SCGC6 |= SIM_SCGC6_ADC0(CGC_CLOCK_ENABLED);	//Enable clock for ADC

    /* un-gate port clocks*/
    SIM_SCGC5 |= SIM_SCGC5_PORTE(CGC_CLOCK_ENABLED);
    SIM_SCGC5 |= SIM_SCGC5_PORTB(CGC_CLOCK_ENABLED);

    /* set pins as ADC In */
    PORT_PCR_REG(PH0_PORT_BASE_PNT, PH0_PIN) = PORT_PCR_MUX(PH0_PIN_MUX_ALT);
    PORT_PCR_REG(PH1_PORT_BASE_PNT, PH1_PIN) = PORT_PCR_MUX(PH1_PIN_MUX_ALT);
    PORT_PCR_REG(PH2_PORT_BASE_PNT, PH2_PIN) = PORT_PCR_MUX(PH2_PIN_MUX_ALT);
    PORT_PCR_REG(PH3_PORT_BASE_PNT, PH3_PIN) = PORT_PCR_MUX(PH3_PIN_MUX_ALT);
    PORT_PCR_REG(PH4_PORT_BASE_PNT, PH4_PIN) = PORT_PCR_MUX(PH4_PIN_MUX_ALT);
    PORT_PCR_REG(PH5_PORT_BASE_PNT, PH5_PIN) = PORT_PCR_MUX(PH5_PIN_MUX_ALT);

    ADC_WR_SC2(ADC_NUM_BASE_PNT,
    		(
    				ADC_SC2_REFSEL(0x0) 			//external pins
					| ADC_SC2_DMAEN(0x0) 			//dma disabled
					| ADC_SC2_ACFE(0x0) 			//compare disabled
					| ADC_SC2_ADTRG(0x0)			//sw trigger
			)
	);

    ADC_WR_CFG2(ADC_NUM_BASE_PNT,
    		(
    				ADC_CFG2_ADLSTS(0) 				// default time
    				| ADC_CFG2_ADHSC(0) 			// normal conversion sequence
					| ADC_CFG2_ADACKEN(0) 			// adack clock disabled
					| ADC_CFG2_MUXSEL(ADC_MUX_IDX)	// 'a' channels
			)
	);

    //Calibrate ADC MODULE
    //Change clock to lower than 4Mhz
    ADC_WR_CFG1(ADC_NUM_BASE_PNT,
    		(
    				ADC_CFG1_ADICLK(0x1)  		// bus/2 clock selection
    				| ADC_CFG1_MODE(0x3)  		// 16-bit Conversion mode selection
					| ADC_CFG1_ADLSMP(0x0) 		// Short sample time configuration
					| ADC_CFG1_ADIV(0x3) 		// Clock Divide Select (Divide by 8)
					| ADC_CFG1_ADLPC(0x1)		// Normal power Configuration
			)
	);

    //Enable maximum average mode and start calibration
    ADC_WR_SC3(ADC_NUM_BASE_PNT,
     		(
     				ADC_SC3_AVGE(0x1) 				//Enable average
 					| ADC_SC3_AVGS(0x3) 			//32 averages
					| ADC_SC3_CAL(0x1)
 			)
 	);

    while(!photoSensor_isAdcDone()); //Wait for conversion finish

    //Compute plus side gain value
    uint16_t ui16PSG = 0;
    ui16PSG += ADC_RD_CLP0(ADC_NUM_BASE_PNT);
    ui16PSG += ADC_RD_CLP1(ADC_NUM_BASE_PNT);
    ui16PSG += ADC_RD_CLP2(ADC_NUM_BASE_PNT);
    ui16PSG += ADC_RD_CLP3(ADC_NUM_BASE_PNT);
    ui16PSG += ADC_RD_CLP4(ADC_NUM_BASE_PNT);
    ui16PSG += ADC_RD_CLPS(ADC_NUM_BASE_PNT);
    ui16PSG >> 1;
	ui16PSG |= (1 << 15);
	// Save plus side gain
	ADC_WR_PG(ADC_NUM_BASE_PNT, ui16PSG);

    //Config ADC MODULE FOR USE
    ADC_WR_CFG1(ADC_NUM_BASE_PNT,
    		(
    				ADC_CFG1_ADICLK(0x1)  		// bus/2 clock selection
    				| ADC_CFG1_MODE(0x3)  		// 16-bit Conversion mode selection
					| ADC_CFG1_ADLSMP(0x0) 		// Short sample time configuration
					| ADC_CFG1_ADIV(0x0) 		// Clock Divide Select (Divide by 1)
					| ADC_CFG1_ADLPC(0x1)		// Normal power Configuration
			)
	);

    //TODO Check if measuring without hardware avg is precise enough and recalculate
    //measuring time to take into account multiple measures
//    ADC_WR_SC3(ADC_NUM_BASE_PNT,
//     		(
//     				ADC_SC3_AVGE(0x0) 				//Disable average
// 					| ADC_SC3_AVGS(0x0)
// 			)
// 	);
}

/**
 * Init conversion from A to D
 * @param usSensorNumber - from 0 to 5
 */
void photoSensor_initAdcConversion(unsigned short usSensorNumber) {
	ADC_WR_SC1(ADC_NUM_BASE_PNT, ADC_MUX_IDX,
		(
			ADC_SC1_ADCH(usaCHANNELS[usSensorNumber])
			| ADC_SC1_DIFF(0) 						//single ended
			| ADC_SC1_AIEN(0)						//interrupt disabled
		)
	);
}

/**
 * Retrieve converted value
 * @return The conversion result
 */
uint16_t photoSensor_getAdcConversionValue(void) {
	return ADC_RD_R(ADC_NUM_BASE_PNT, ADC_MUX_IDX); // return the register value that keeps the result of convertion
}

/**
 * Measure DC values for all sensors
 */
void photoSensor_getDCValues() {
	for(unsigned int i = 0; i < 6; i++){
		photoSensor_initAdcConversion(i);
		while(!photoSensor_isAdcDone());
		ui16aOFFSETS[i] += photoSensor_getAdcConversionValue();
	}
}

/**
 * Configure LED GPIOs
 */
void photoSensor_initLEDS() {
	GPIO_UNGATE_PORT(LD0_PORT_ID);
	GPIO_INIT_PIN(LD0_PORT_ID, LD0_PIN, GPIO_OUTPUT);
	GPIO_WRITE_PIN(LD0_PORT_ID, LD0_PIN, GPIO_HIGH);
	GPIO_UNGATE_PORT(LD1_PORT_ID);
	GPIO_INIT_PIN(LD1_PORT_ID, LD1_PIN, GPIO_OUTPUT);
	GPIO_WRITE_PIN(LD1_PORT_ID, LD1_PIN, GPIO_HIGH);
	GPIO_UNGATE_PORT(LD2_PORT_ID);
	GPIO_INIT_PIN(LD2_PORT_ID, LD2_PIN, GPIO_OUTPUT);
	GPIO_WRITE_PIN(LD2_PORT_ID, LD2_PIN, GPIO_HIGH);
	GPIO_UNGATE_PORT(LD3_PORT_ID);
	GPIO_INIT_PIN(LD3_PORT_ID, LD3_PIN, GPIO_OUTPUT);
	GPIO_WRITE_PIN(LD3_PORT_ID, LD3_PIN, GPIO_HIGH);
	GPIO_UNGATE_PORT(LD4_PORT_ID);
	GPIO_INIT_PIN(LD4_PORT_ID, LD4_PIN, GPIO_OUTPUT);
	GPIO_WRITE_PIN(LD4_PORT_ID, LD4_PIN, GPIO_HIGH);
	GPIO_UNGATE_PORT(LD5_PORT_ID);
	GPIO_INIT_PIN(LD5_PORT_ID, LD5_PIN, GPIO_OUTPUT);
	GPIO_WRITE_PIN(LD5_PORT_ID, LD5_PIN, GPIO_HIGH);
}

/**
 * Configure photoSensor module
 */
void photoSensor_init(void){
	photoSensor_initLEDS();
	photoSensor_initADCModule();
	photoSensor_getDCValues();
}

/**
 * Measure light level for the given sensor (already without DC offset)
 * @param usSensorNumber - from 0 to 5
 * @return The conversion result
 */
int photoSensor_measure(unsigned short usSensorNumber) {
	GPIO_HAL_ClearPinOutput(gtpLEDPORTS[usSensorNumber], usaLEDPINS[usSensorNumber]);
	photoSensor_initAdcConversion(usSensorNumber);
	while(!photoSensor_isAdcDone());
	int measure = (int) photoSensor_getAdcConversionValue();
	measure -= ui16aOFFSETS[usSensorNumber];
	GPIO_HAL_SetPinOutput(gtpLEDPORTS[usSensorNumber], usaLEDPINS[usSensorNumber]);
	return measure;
}
