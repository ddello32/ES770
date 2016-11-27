/*
 * File name: lineController.h
 *
 *  Created on: 19/11/2016
 *      Author: ddello32
 */

#ifndef SOURCES_LINECONTROL_H_
#define SOURCES_LINECONTROL_H_
/**
 *  Configure LineController module
 *
 *  @param uiKp - Proportional Gain
 *  @param uiKi - Integral Gain
 *  @param uiKd - Derivative Gain
 *  @param uiSampleTime - Time between runs
 */
void lineControl_init(unsigned int uiKp, unsigned int uiKi, unsigned int uiKd, unsigned int sampleTime);

/**
 * Runs line controller calling motor controller
 *
 * @param - desired linear speed (in %)
 */
void lineControl_execute(int iLinSpeed);
#endif /* SOURCES_LINECONTROL_H_ */
