/*
 * File name: speedController.h
 *
 *  Created on: 08/10/2016
 *      Author: ddello32
 */

#ifndef SOURCES_SPEEDCONTROL_H_
#define SOURCES_SPEEDCONTROL_H_
/**
 *  Configure SpeedController module
 *
 *  @param uiKp - Proportional Gain
 *  @param uiKi - Integral Gain
 *  @param uiKd - Derivative Gain
 *  @param uiSampleTime - Time between runs
 */
void speedControl_init(unsigned int uiKp[2], unsigned int uiKi[2], unsigned int uiKd[2], unsigned int sampleTime);

/**
 * Saves the motor calibration params
 * @param usMotorNumber motor identifier (0, 1)
 * @param iMin the minimum speed ref to get the motor running in direct speed
 * @param uiMax the speed measured when the motor was in maximum direct speed (in pulses by sec)
 * @param iInvMin the minimum speed ref to get the motor running in inverse speed
 * @param uiInvMax the speed measured when the motor was in maximum inverse speed (in pulses by sec)
 */
void speedControl_calibrate(unsigned short usMotorNumb, int iMin, unsigned int uiMax, int iInvMin, unsigned int uiInvMax);

/**
 * Runs speed controller adjusting motor speed
 */
void speedControl_execute(int iLinSpeed, int iAngSpeed);
#endif /* SOURCES_SPEEDCONTROL_H_ */
