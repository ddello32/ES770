/*
 * File name: autotest.c
 *
 *  Created on: 04/10/2016
 *      Author: ddello32
 */

#include "RGBLed/rgbled_hal.h"
#include "PhotoSensor/photosensor_hal.h"
#include "Serial/serial_hal.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * Test all photoSensors
 */
void autotest_testPhotoSensors(void) {
	for (unsigned short i = 0; i < 6; i++) {
		char buff[100];
		unsigned int measure = photoSensor_measure(i);
		if(abs(measure) < 1000){
			sprintf(buff, "LED %d FAILED\n", i);
			serial_sendBuffer(buff, strlen(buff));
			switch (i) {
				case 0:
					rgbled_setColor(0,0,0xFF);
					break;
				case 1:
					rgbled_setColor(0xFF,0,0xFF);
					break;
				case 2:
					rgbled_setColor(0,0xFF,0);
					break;
				case 3:
					rgbled_setColor(0,0xFF,0);
					break;
				case 4:
					rgbled_setColor(0x5A,0x27,0x29);
					break;
				case 5:
					rgbled_setColor(0xFF,0xFF,0xFF);
					break;
			}
		}else {
			sprintf(buff, "LED %d SUCCESS\n", i);
			serial_sendBuffer(buff, strlen(buff));
		}
	}

}

/**
 *  Test and calibrate all modules
 */
void autotest_testAndCalibrate(void) {
	autotest_testPhotoSensors();
}
