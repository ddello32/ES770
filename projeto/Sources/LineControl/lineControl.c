/*
 * File name: lineController.c
 *
 *  Created on: 19/11/2016
 *      Author: ddello32
 */

#include "lineControl.h"
#include "LineSensor/linesensor.h"
#include "SpeedControl/speedController.h"
#include <stdlib.h>

#define OVERFLOW_AVOIDMENT_THRESSHOLD 250

static unsigned int kp;
static unsigned int ki;
static unsigned int kd;
static unsigned int dt;
static int err = 0;
static int sum = 0;
static int der = 0;
static unsigned int uiCounter = 0;
/**
 *  Configure LineController module
 *
 *  @param uiKp - Proportional Gain
 *  @param uiKi - Integral Gain
 *  @param uiKd - Derivative Gain
 *  @param uiSampleTime - Time between runs
 */
void lineControl_init(unsigned int uiKp, unsigned int uiKi, unsigned int uiKd, unsigned int sampleTime){
	kp = uiKp;
	ki = uiKi;
	kd = uiKd;
	dt = sampleTime;
	sum = 0;
}


/**
 * Runs speed controller adjusting motor speed
 */
/**
 * Runs line controller calling motor controller
 *
 * @param - desired linear speed (in %)
 */
void lineControl_execute(int iLinSpeed){
	int measure = lineSensor_measure();
	der = measure - err;
	sum += measure;
	err = measure;
	//the dt/2 factor is already included in ki
	int iOut = kp * err + (ki * sum) + (kd * der /dt);
	speedControl_execute(iLinSpeed, iOut);

}
