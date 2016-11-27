/*
 * File name: linesensor_hal.c
 *
 *  Created on: 02/11/2016
 *      Author: ddello32
 */
#include "PhotoSensor/photosensor_hal.h"
#include "Serial/serial_hal.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <StateMachine/stateMachine.h>
#include "RGBLed/rgbled_hal.h"

#define PHOTOSENSORDISTANCE 10
#define DARK_THRESSHOLD 50
#define LINE_MIN 2
#define COMMAND_MIN 5
static unsigned short usaPoints[8] = {100, 0, 0, 0, 0, 0, 0, 100};

float linesensor_spline(unsigned short i, float u){
	//assuming s = 0.5 adjust if changed
	return u*u*u*(-0.5*usaPoints[i-1] + 1.5*usaPoints[i] - 1.5*usaPoints[i+1] + 0.5*usaPoints[i+2])
			+ u*u*(usaPoints[i-1] - 2.5*usaPoints[i] + 2*usaPoints[i+1] - 0.5*usaPoints[i+2])
			+ u*(-0.5*usaPoints[i-1] + 0.5*usaPoints[i+1])
			+ usaPoints[i];
}

float linesensor_findMin(){
	float min = 1000;
	float minPos = 0;
	float aux, a, b, c, sqrtD, r1, r2;
	//For all intervals
	for(unsigned short i = 1; i < 6; i++){
		aux = linesensor_spline(i, 0);
		if(aux < min){
			min = aux;
			minPos = i-1;
		}
		//Find zero derivatives (assuming s = 0.5 adjust if changed)
		a = -1.5*usaPoints[i-1] + 4.5*usaPoints[i] -4.5*usaPoints[i+1] + 1.5*usaPoints[i+2];
		b = 2*usaPoints[i-1] - 5*usaPoints[i] + 4*usaPoints[i+1] - 1*usaPoints[i+2];
		c = -0.5*usaPoints[i-1] + 0.5*usaPoints[i+1];
		sqrtD = sqrtf(b*b - 4*a*c);
		r1 = (-b + sqrtD)/2/a;
		r2 = (-b - sqrtD)/2/a;
		//Check if in interval
		if(r1 >=0  && r1 <= 1) {
			aux = linesensor_spline(i, r1);
			if(aux < min){
			  min = aux;
			  minPos = i - 1 + r1;
			}
		}
		if(r2 >=0  && r2 <= 1){
			aux = linesensor_spline(i, r2);
			if(aux < min){
			  min = aux;
			  minPos = i - 1 +r2;
			}
		}
		aux = linesensor_spline(i, 1);
		if(aux < min){
			min =  aux;
			minPos = i;
		}
	}
	return minPos;
}

/**
 *  Configure LineSensor module
 */
void lineSensor_init(void){
	photoSensor_init();
}

/**
 * Measure distance between car and line
 * @return The distance between car center and line (in mm)
 */
int lineSensor_measure(){
	static int lastMeasure = -25;
	char buff[100];
	unsigned short darkCount = 0;
	for(unsigned short usSensorNumb = 0; usSensorNumb < 6; usSensorNumb++){
		usaPoints[usSensorNumb+1] = photoSensor_measure(usSensorNumb);
		if(usaPoints[usSensorNumb+1] < DARK_THRESSHOLD){
			darkCount++;
		}
	}
	usaPoints[0] = usaPoints[1];
	usaPoints[7] = usaPoints[6];
	if(darkCount < LINE_MIN){
		rgbled_setColor(0x0FF,0,0);
		stateMachine_foundCommand(0);
//		sprintf(buff, "LOST LINE\n");
//		serial_sendBuffer(buff, strlen(buff));
	}else{
		rgbled_setColor(0,0,0);
		if(darkCount >= COMMAND_MIN){
			rgbled_setColor(0x0, 0x0FF, 0);
			stateMachine_foundCommand(1);
//			sprintf(buff, "FOUND COMMAND\n");
//			serial_sendBuffer(buff, strlen(buff));
		}else {
			stateMachine_foundCommand(0);
		}
		lastMeasure = (linesensor_findMin() - 2.5)*PHOTOSENSORDISTANCE;
	}
	return lastMeasure;
}
