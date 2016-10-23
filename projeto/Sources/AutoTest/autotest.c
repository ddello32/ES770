/*
 * File name: autotest.c
 *
 *  Created on: 04/10/2016
 *      Author: ddello32
 */

#include "RGBLed/rgbled_hal.h"
#include "PhotoSensor/photosensor_hal.h"
#include "Serial/serial_hal.h"
#include "Encoder/encoder_hal.h"
#include "Motor/motor_hal.h"
#include "Util/util.h"
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
					rgbled_setColor(0,0,0x0FF);
					break;
				case 1:
					rgbled_setColor(0x0FF,0,0x0FF);
					break;
				case 2:
					rgbled_setColor(0,0x0FF,0);
					break;
				case 3:
					rgbled_setColor(0x0FF,0x02A,0);
					break;
				case 4:
					rgbled_setColor(0x0,0x0FF,0x0FF);
					break;
				case 5:
					rgbled_setColor(0x0FF,0x0FF,0x0FF);
					break;
			}
		}else {
			sprintf(buff, "LED %d SUCCESS\n", i);
			serial_sendBuffer(buff, strlen(buff));
		}
	}

}

void autotest_testMotors(){
	char buff[100];
	if(encoder_getSpeed(0,  1000000)){
		sprintf(buff, "GOT READING FROM ENCODER 0 WITH MOTOR 0 OFF\n");
		serial_sendBuffer(buff, strlen(buff));
	}
	if(encoder_getSpeed(1,  1000000)){
		sprintf(buff, "GOT READING FROM ENCODER 1 WITH MOTOR 1 OFF\n");
		serial_sendBuffer(buff, strlen(buff));
	}
	motor_setSpeed(0, 0x7777);
	motor_setSpeed(1, 0);
	util_genDelay1s();
	util_genDelay1s();
	util_genDelay1s();
	int reading = encoder_getSpeed(0, 3000000);
	if(!reading){
		sprintf(buff, "GOT NO READING ENCODER 0 WITH MOTOR 0 ON\n");
		serial_sendBuffer(buff, strlen(buff));
		rgbled_setColor(0x0FF, 0, 0);
	}else {
		sprintf(buff, "READ %d PULSES FROM ENCODER 0\n", reading);
		serial_sendBuffer(buff, strlen(buff));
	}
	motor_setSpeed(0, 0);
	motor_setSpeed(1, 0x7777);
	util_genDelay1s();
	util_genDelay1s();
	util_genDelay1s();
	reading = encoder_getSpeed(1, 3000000);
	if(!reading){
		sprintf(buff, "GOT NO READING ENCODER 1 WITH MOTOR 1 ON\n");
		serial_sendBuffer(buff, strlen(buff));
		rgbled_setColor(0, 0x0FF, 0);
	}else {
		sprintf(buff, "READ %d PULSES FROM ENCODER 1\n", reading);
		serial_sendBuffer(buff, strlen(buff));
	}
	motor_setSpeed(0, -0x7777);
	motor_setSpeed(1, 0);
	util_genDelay1s();
	util_genDelay1s();
	util_genDelay1s();
	reading = encoder_getSpeed(0, 3000000);
	if(!reading){
		sprintf(buff, "GOT NO READING ENCODER 0 WITH MOTOR 0 ON\n");
		serial_sendBuffer(buff, strlen(buff));
		rgbled_setColor(0x0FF, 0, 0);
	}else {
		sprintf(buff, "READ %d PULSES FROM ENCODER 0\n", reading);
		serial_sendBuffer(buff, strlen(buff));
	}
	motor_setSpeed(0, 0);
	motor_setSpeed(1, -0x7777);
	util_genDelay1s();
	util_genDelay1s();
	util_genDelay1s();
	reading = encoder_getSpeed(1, 3000000);
	if(!reading){
		sprintf(buff, "GOT NO READING ENCODER 1 WITH MOTOR 1 ON\n");
		serial_sendBuffer(buff, strlen(buff));
		rgbled_setColor(0, 0x0FF, 0);
	}else {
		sprintf(buff, "READ %d PULSES FROM ENCODER 1\n", reading);
		serial_sendBuffer(buff, strlen(buff));
	}
	motor_setSpeed(0, 0);
	motor_setSpeed(1, 0);
}

/**
 *  Test and calibrate all modules
 */
void autotest_testAndCalibrate(void) {
	autotest_testPhotoSensors();
	autotest_testMotors();
}
