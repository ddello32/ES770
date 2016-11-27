/*
 * File name: speedController.c
 *
 *  Created on: 08/10/2016
 *      Author: ddello32
 */

#include "speedController.h"
#include "Encoder/encoder_hal.h"
#include "Motor/motor_hal.h"
#include <stdlib.h>

#define OVERFLOW_AVOIDMENT_THRESSHOLD 1000
#define CAR_RADIUS 71

#define MIN(A, B) A<B?A:B

static unsigned int kp[2];
static unsigned int ki[2];
static unsigned int kd[2];
static unsigned int dt;
static int err[2] = {0, 0};
static int sum[2] = {0, 0};
static int der[2] = {0, 0};
static short lastSign[2] = {1, 1};
static int refSpeed[2] = {0, 0};
static int iaMin[2] = {0, 0};
static int iaInvMin[2] = {0, 0};
static unsigned int uiMaxSpeed =  0;
static unsigned int uiCounter = 0;
/**
 *  Configure SpeedController module
 *
 *  @param uiKp - Proportional Gain
 *  @param uiKi - Integral Gain
 *  @param uiKd - Derivative Gain
 *  @param uiSampleTime - Time between runs
 */
void speedControl_init(unsigned int uiKp[2], unsigned int uiKi[2], unsigned int uiKd[2], unsigned int sampleTime){
	kp[0] = uiKp[0];
	kp[1] = uiKp[1];
	ki[0] = uiKi[0];
	ki[1] = uiKi[1];
	kd[0] = uiKd[0];
	kd[1] = uiKd[1];
	dt = sampleTime;
	sum[0] = 0;
	sum[1] = 0;
}

/**
 * Saves the motor calibration params
 * @param usMotorNumber motor identifier (0, 1)
 * @param iMin the minimum speed ref to get the motor running in direct speed
 * @param uiMax the speed measured when the motor was in maximum direct speed (in pulses by sec)
 * @param iInvMin the minimum speed ref to get the motor running in inverse speed
 * @param uiInvMax the speed measured when the motor was in maximum inverse speed (in pulses by sec)
 */
void speedControl_calibrate(unsigned short usMotorNumb, int iMin, unsigned int uiMax, int iInvMin, unsigned int uiInvMax){
	iaMin[usMotorNumb] = iMin;
	iaInvMin[usMotorNumb] = iInvMin;
	uiMaxSpeed = MIN(uiMaxSpeed, MIN(uiMax, uiInvMax));
}

/**
 * Runs speed controller adjusting motor speed
 */
void speedControl_execute(int iLinSpeed, int iAngSpeed){
	int halfSpeed = (iAngSpeed*((int)uiMaxSpeed))/200;
	refSpeed[0] = (iLinSpeed*((int)uiMaxSpeed))/100 - halfSpeed;
	refSpeed[1] = (iLinSpeed*((int)uiMaxSpeed))/100 + halfSpeed;
	uiCounter++;
	if(uiCounter > OVERFLOW_AVOIDMENT_THRESSHOLD){
		uiCounter = 0;
		sum[0] /= 10;
		sum[1] /= 10;
	}
	for(unsigned short usMotorNum = 0; usMotorNum < 2; usMotorNum++){
		int iRef = abs(refSpeed[usMotorNum]);
		short sSign = refSpeed[usMotorNum] < 0? -1:1;
		if(iRef > uiMaxSpeed){
			iRef = uiMaxSpeed;
		}
		//Error (we keep it in an auxiliary variable in order not to lose last measure before calculating der)
		int iAux = iRef - encoder_getMeanSpeed(usMotorNum, dt);
		der[usMotorNum] = iAux - err[usMotorNum];
		sum[usMotorNum] += iAux;
		err[usMotorNum] = iAux;
		//the dt/2 factor is already included in ki
		int iOut = kp[usMotorNum] * err[usMotorNum] + (ki[usMotorNum] * sum[usMotorNum]) + (kd[usMotorNum] * der[usMotorNum] /dt);
		//NORMALIZE OUT TO LIN REGION OF MOTOR
		iOut += (sSign<0?iaInvMin[usMotorNum]:iaMin[usMotorNum]);
		motor_setSpeed(usMotorNum, sSign*iOut);
	}
}
