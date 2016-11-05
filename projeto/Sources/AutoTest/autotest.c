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
#include <limits.h>

#define MIN(A,B)	A<B?A:B
#define MAX(A,B)	A>B?A:B

/**
 * Test all photoSensors
 * Checks one by one if their readings differ from the DC reading value
 * In case of failure turns on LED with the failed sensor wire color.
 */
void autotest_testPhotoSensors(void) {
	for (unsigned short i = 0; i < 6; i++) {
		char buff[100];
		unsigned int measure = photoSensor_measure_raw(i);
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

/**
 * Tests motor modules.
 * Checks if stopped
 * Turns motor 1 at half direct speed for 3 seconds
 * Checks if encoder 1 read any value (turns on red light in case of failure)
 * Turns motor 2 at half direct speed for 3 seconds
 * Checks if encoder 2 read any value (turns on green light in case of failure)
 * Turns motor 1 at half inverse speed for 3 seconds
 * Checks if encoder 1 read any value (turns on red light in case of failure)
 * Turns motor 2 at half inverse speed for 3 seconds
 * Checks if encoder 2 read any value (turns on green light in case of failure)
 */
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
	int reading = encoder_getSpeed(0, 1000000);
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
	reading = encoder_getSpeed(1, 1000000);
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
	reading = encoder_getSpeed(0, 1000000);
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
	reading = encoder_getSpeed(1, 1000000);
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
 * Calibrates motor module
 * Gradually increases PWM value sent to motor until we are able to find the minimum
 * speed necessary for the motor to turn on.
 * Then sets the motor to full speed and reads the maximum motor speed.
 */
void autotest_calibrateMotors(){
	char buff[100];
	for(unsigned short usMotorNumb = 0; usMotorNumb < 2; usMotorNumb++){
		int iMinDSpeed = 0;
		unsigned int uiMaxDValue = 0;
		int iMinISpeed = 0;
		unsigned int uiMaxIValue = 0;
		//Direct Speed
		for(int iSpeed = 0; iSpeed <= 0xFFFF; iSpeed += 0x0400){
			motor_setSpeed(usMotorNumb, iSpeed);
			encoder_getSpeed(usMotorNumb, 100000); 	//Clear any residual measures
			util_genDelay100ms();
			util_genDelay100ms();
			util_genDelay100ms();
			if(encoder_getSpeed(usMotorNumb,  300000)){
					sprintf(buff, "MIN DIRECT SPEED FOR MOTOR %u IS %d\n", usMotorNumb, iSpeed);
					serial_sendBuffer(buff, strlen(buff));
					iMinDSpeed = iSpeed;
					break;
			}
		}
		motor_setSpeed(usMotorNumb, 0xFFFF);
		encoder_getSpeed(usMotorNumb, 100000);	//Clear residual measures
		util_genDelay1s();
		uiMaxDValue = encoder_getSpeed(usMotorNumb, 1000000);
		sprintf(buff, "MAX DIRECT SPEED VALUE FOR MOTOR %u IS %u PULSED BY SEC\n", usMotorNumb, uiMaxDValue);
		serial_sendBuffer(buff, strlen(buff));
		motor_setSpeed(usMotorNumb, 0);
		util_genDelay1s();
		//Inverse Speed
		for(int iSpeed = 0; iSpeed >= -0xFFFF; iSpeed -= 0x0400){
			motor_setSpeed(usMotorNumb, iSpeed);
			encoder_getSpeed(usMotorNumb, 100000);
			util_genDelay100ms();
			util_genDelay100ms();
			util_genDelay100ms();
			if(encoder_getSpeed(usMotorNumb,  300000)){
					sprintf(buff, "MIN INVERSE SPEED FOR MOTOR %u IS %d\n", usMotorNumb, iSpeed);
					serial_sendBuffer(buff, strlen(buff));
					iMinISpeed = iSpeed;
					break;
			}
		}
		motor_setSpeed(usMotorNumb, -0xFFFF);
		encoder_getSpeed(usMotorNumb, 100000);
		util_genDelay1s();
		uiMaxIValue = encoder_getSpeed(usMotorNumb,  1000000);
		sprintf(buff, "MAX INVERSE SPEED VALUE FOR MOTOR %u IS %u PULSED BY SEC\n", usMotorNumb, uiMaxIValue);
		serial_sendBuffer(buff, strlen(buff));
		motor_setSpeed(usMotorNumb, 0);
		motor_calibrate(usMotorNumb, iMinDSpeed, uiMaxDValue, iMinISpeed, uiMaxIValue);
	}
}

/**
 * Calibrates photo sensor module
 *
 */
void autotest_calibratePhotoSensors(){
	int iaMINS[6] = {INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX};
	int iaMAXS[6] = {INT_MIN, INT_MIN, INT_MIN, INT_MIN, INT_MIN, INT_MIN};
	motor_setSpeed(0, 0x7777);
	motor_setSpeed(1, -0x8000);
	for(unsigned int i = 0; i < 10000; i++){
		for(unsigned short usSensorNumb = 0; usSensorNumb < 6; usSensorNumb++){
			int iRawMeasure = photoSensor_measure_raw(usSensorNumb);
			iaMINS[usSensorNumb] = MIN(iRawMeasure, iaMINS[usSensorNumb]);
			iaMAXS[usSensorNumb] = MAX(iRawMeasure, iaMAXS[usSensorNumb]);
		}
	}
	motor_setSpeed(0, 0);
	motor_setSpeed(1, 0);
	for(unsigned short usSensorNumb = 0; usSensorNumb < 6; usSensorNumb++){
		photoSensor_calibrate(usSensorNumb, iaMAXS[usSensorNumb], iaMINS[usSensorNumb]);
		if(iaMAXS[usSensorNumb] - iaMINS[usSensorNumb] < 2000){
			switch (usSensorNumb) {
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
		}
	}
}



/**
 *  Test and calibrate all modules
 */
void autotest_testAndCalibrate(void) {
	autotest_testPhotoSensors();
//	autotest_testMotors();
//	autotest_calibrateMotors();
	autotest_calibratePhotoSensors();
}
