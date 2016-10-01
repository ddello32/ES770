#include "KL25Z/es770_peripheral_board.h"
#include "Mcg/mcg_hal.h"
#include "PIT/pit_hal.h"
#include "Util/util.h"
#include "Serial/serial_hal.h"
#include "Protocolo/cmdmachine_hal.h"
#include "Util/tc_hal.h"
#include "RGBLed/rgbled_hal.h"
#include "PhotoSensor/photosensor_hal.h"
#include "Tacometro/tacometro_hal.h"
#include <string.h>
#include <stdio.h>

/* defines */
#define CYCLIC_EXECUTIVE_PERIOD         50*1000 /* in micro seconds */

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
	//TODO
}

/**
 * Main function
 */
int main(void) {
	char charBuff[100];
	main_boardInit();

    /* configure cyclic executive interruption */
    tc_installLptmr0(CYCLIC_EXECUTIVE_PERIOD, main_cyclicExecuteIsr);
    /* cooperative cyclic executive main loop */
	while(1){
		//TODO
		while(!uiFlagNextPeriod){
			for(unsigned short i = 0; i < 6; i++){
				sprintf(charBuff, "PH%d: %d", i, photoSensor_measure(i));
				serial_sendBuffer(charBuff, strlen(charBuff));
			}
		}
		uiFlagNextPeriod = 0;
	}
    /* Never leave main */
    return 0;
}
