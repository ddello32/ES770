/*
 * File name: stateMachine.h
 *
 *  Created on: 27/11/2016
 *      Author: ddello32
 */

#ifndef SOURCES_STATE_MACHINE_H_
#define SOURCES_STATE_MACHINE_H_
/**
 *  Should be called by line sensor at each execution
 *
 * @param - usFoundCommand if a command was found in the line or not
 *
 */
void stateMachine_foundCommand(unsigned short usFoundCommand);

/**
 * Runs protocol controller changing it's state as necessary
 *
 * @param - iMeasuredDistance distance measured since last call (in mm)
 * @param - uiTime Time ellapsed since last call (in us)
 */
void stateMachine_execute(int iMeasuredDistance, unsigned int uiTime);
#endif /* SOURCES_STATE_MACHINE_H_ */
