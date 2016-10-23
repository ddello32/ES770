#include "KL25Z/es770_peripheral_board.h"
#include "Mcg/mcg_hal.h"
#include "PIT/pit_hal.h"
#include "Util/util.h"
#include "Serial/serial_hal.h"
#include "Protocolo/cmdmachine_hal.h"
#include "Util/tc_hal.h"
#include "RGBLed/rgbled_hal.h"
#include "PhotoSensor/photosensor_hal.h"
#include "AutoTest/autotest.h"
#include "Motor/motor_hal.h"
#include "Encoder/encoder_hal.h"
#include <string.h>
#include <stdio.h>

/* defines */
#define CYCLIC_EXECUTIVE_PERIOD         3000*1000 /* in micro seconds */

/* globals */
volatile unsigned int uiFlagNextPeriod = 0;         /* cyclic executive flag */

/**
 * cyclic executive interrupt service routine
 */
void main_cyclicExecuteIsr(void)
{
    /* set the cyclic executive flag */
    uiFlagNextPeriod = 1;
}


/**
 * Initialize board and periferics
 */
void main_boardInit(){
	mcg_clockInit();
	serial_initUart();
	rgbled_init();
	photoSensor_init();
	motor_init();
	encoder_init();
	//TODO
}

/**
 * Main function
 */
int main(void) {
	int count = 0;
	char charBuff[100];
	main_boardInit();
	autotest_testAndCalibrate();

    /* configure cyclic executive interruption */
    tc_installLptmr0(CYCLIC_EXECUTIVE_PERIOD, main_cyclicExecuteIsr);
    /* cooperative cyclic executive main loop */
	while(1){
		switch(count){
		case 0:
			motor_setSpeed(0, 0x7777);
			motor_setSpeed(1, 0x0);
			count++;
			break;
		case 1:
			motor_setSpeed(0, 0x0);
			motor_setSpeed(1, 0x7777);
			count++;
			break;
		case 2:
			motor_setSpeed(0, -0x7777);
			motor_setSpeed(1, 0x0);
			count++;
			break;
		case 3:
			motor_setSpeed(0, 0);
			motor_setSpeed(1, -0x7777);
			count = 0;
			break;
		default:
			count = 0;
		}

		for(unsigned short i = 0; i < 2; i++){
			sprintf(charBuff, "MOTOR%d: %d\n", i, encoder_getSpeed(i, CYCLIC_EXECUTIVE_PERIOD));
			serial_sendBuffer(charBuff, strlen(charBuff));
		}
		while(!uiFlagNextPeriod){
			//Export data for calibration
			//TODO Clear this and put actual control loop
		}
		uiFlagNextPeriod = 0;
	}
    /* Never leave main */
    return 0;
}
