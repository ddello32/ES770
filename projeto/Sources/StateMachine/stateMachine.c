/*
 * File name: protocol.c
 *
 *  Created on: 19/11/2016
 *      Author: ddello32
 */

#include "RGBLed/rgbled_hal.h"
#include "LineControl/lineControl.h"
#include "Motor/motor_hal.h"
#include <stdlib.h>
#include <StateMachine/stateMachine.h>

#define STATE_NORMAL 0
#define STATE_LOOKING 1
#define STATE_CMD_WALKANDSTOP 2
#define STATE_CMD_MAXSPEED 3

#define SPEED_WHEN_NORMAL 50
#define SPEED_WHEN_MAX 90

#define DISTANCE_THRESSHOLD 60
#define DISTANCE_WALK 80
#define TIME_STOP 5000*1000

int iDistanceSinceLastCommand;
unsigned int uiTimeStopped = 0;
unsigned short iCmdCount = 0;
int iCurrState = STATE_NORMAL;

/**
 *  Should be called by line sensor at each execution
 *
 * @param - usFoundCommand if a command was found in the line or not
 *
 */
void stateMachine_foundCommand(unsigned short usFoundCommand){
	if(usFoundCommand){
		iCmdCount++;
	} else{
		if(iCmdCount > 2){
			switch(iCurrState){
				case STATE_NORMAL:
					iCurrState = STATE_LOOKING;
					rgbled_setColor(0, 0, 0);
					break;
				case STATE_LOOKING:
					iCurrState = STATE_CMD_WALKANDSTOP;
					rgbled_setColor(0x0FF, 0, 0x0FF);
					break;
				case STATE_CMD_MAXSPEED:
					iCurrState = STATE_NORMAL;
					rgbled_setColor(0, 0, 0);
					break;
				case STATE_CMD_WALKANDSTOP:
					iCurrState = STATE_CMD_MAXSPEED;
					rgbled_setColor(0, 0x0FF, 0x0FF);
					break;
			}
			iDistanceSinceLastCommand = 0;
		}
		iCmdCount = 0;
	}
}

/**
 * Runs protocol controller changing it's state as necessary
 *
 * @param - iMeasuredDistance distance measured since last call (in mm)
 * @param - uiTime Time ellapsed since last call (in us)
 */
void stateMachine_execute(int iMeasuredDistance, unsigned int uiTime){
	iDistanceSinceLastCommand += iMeasuredDistance;
	switch(iCurrState){
		case STATE_NORMAL:
			lineControl_execute(SPEED_WHEN_NORMAL);
			break;
		case STATE_LOOKING:
			lineControl_execute(SPEED_WHEN_NORMAL);
			if(iDistanceSinceLastCommand > DISTANCE_THRESSHOLD){
				iCurrState = STATE_CMD_MAXSPEED;
				iDistanceSinceLastCommand = 0;
				rgbled_setColor(0, 0x0FF, 0x0FF);
			}
			break;
		case STATE_CMD_MAXSPEED:
			lineControl_execute(SPEED_WHEN_MAX);
			rgbled_setColor(0, 0x0FF, 0x0FF);
			break;
		case STATE_CMD_WALKANDSTOP:
			rgbled_setColor(0x0FF, 0, 0x0FF);
			iCurrState = STATE_CMD_WALKANDSTOP;
			if(iDistanceSinceLastCommand >= DISTANCE_WALK){
				uiTimeStopped += uiTime;
				motor_setSpeed(0, 0);
				motor_setSpeed(1, 0);
				if(uiTimeStopped >= TIME_STOP){
					iCurrState = STATE_NORMAL;
					uiTimeStopped = 0;
					iDistanceSinceLastCommand = 0;
					rgbled_setColor(0, 0, 0);
					lineControl_execute(SPEED_WHEN_NORMAL);
				}
			}else {
				lineControl_execute(SPEED_WHEN_NORMAL);
			}
			break;
	}
}
