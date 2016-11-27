/* ***************************************************************** */
/* File name:        cmdmachine_hal.c                                */
/* File description: File containing the functions/methods    		 */
/*                   interfaces for protocol command machine		 */
/* Author name:      ddello		                                     */
/* Creation date:    27abr2016                                       */
/* Revision date:    13mai2016                                       */
/* ***************************************************************** */
#include <string.h>
#include <stdio.h>
#include "cmdmachine_hal.h"
#include "Util/util.h"
#include "LineControl/lineControl.h"
//#include "Main/es770.h"

#define ERR_STR "ERR\n"
#define ACK_STR "ACK\n"

#define STATE_IDLE 0
#define STATE_MOTOR_CMD 6
#define STATE_SC_CMD 1
#define STATE_LC_CMD 2
#define STATE_ERR 99

static int iState = STATE_IDLE;

//============================================================================
// IDLE STATE MACHINE
//============================================================================
/**
 * Handles parsing while in IDLE state and checks for transitions
 *
 * @param cpCmdBuffer The start of the command string to parse
 * @param uiSize The size of the command string
 * @param cpCmdRes Buffer for concatenating the commmand response
 *
 * @return The number of characters parsed while in the IDLE state
 */
unsigned int handleIdle(char *cpCmdBuffer, unsigned int uiSize, char* cpCmdRes){
	unsigned int uiCounter = 0;
	while(uiCounter < uiSize && iState == STATE_IDLE){
		switch(cpCmdBuffer[uiCounter++]){
			case ' ':
			case '\t':
			case '\r':
			case '\n':
			case '\0':
				break;
			case 'M':
				iState = STATE_MOTOR_CMD;
				break;
			case 'K':
				iState = STATE_SC_CMD;
				break;
			case 'L':
				iState = STATE_LC_CMD;
			default:
				iState = STATE_ERR;
		}
	}
	return uiCounter;
}

//============================================================================
// MOTOR STATE MACHINE
//============================================================================
/**
 * Handles parsing while in COOLER_CMD state and checks for transitions
 *
 * @param cpCmdBuffer The start of the command string to parse
 * @param uiSize The size of the command string
 * @param cpCmdRes Buffer for concatenating the command response
 *
 * @return The number of characters parsed while in the COOLER_COMMAND state
 */
int handleMotor(char *cpCmdBuffer, unsigned int uiSize, char* cpCmdRes){
	unsigned int uiCounter = 0;
	int iLinSpeed;
	int iAngSpeed;
	if(!sscanf(cpCmdBuffer, " %d %d", &iLinSpeed, &iAngSpeed)){
		iState = STATE_ERR;
		return 0;
	}
	strcat(cpCmdRes, ACK_STR);
	while(uiCounter < uiSize && ((cpCmdBuffer[uiCounter] >= '0' && cpCmdBuffer[uiCounter] <= '9') || (cpCmdBuffer[uiCounter] == ' '))){
				uiCounter++;
	}
//	main_setSpeeds(iLinSpeed, iAngSpeed);
	iState = STATE_IDLE;
	return uiCounter;
}

//============================================================================
// SPEED CONTROL STATE MACHINE
//============================================================================
/**
 * Handles parsing while in COOLER_CMD state and checks for transitions
 *
 * @param cpCmdBuffer The start of the command string to parse
 * @param uiSize The size of the command string
 * @param cpCmdRes Buffer for concatenating the command response
 *
 * @return The number of characters parsed while in the COOLER_COMMAND state
 */
int handleSC(char *cpCmdBuffer, unsigned int uiSize, char* cpCmdRes){
	unsigned int uiCounter = 0;
	unsigned int motor;
	int ikp;
	int iki;
	int ikd;
	if(!sscanf(cpCmdBuffer, " %u %d %d %d", &motor, &ikp, &iki, &ikd)){
		iState = STATE_ERR;
		return 0;
	}
	strcat(cpCmdRes, ACK_STR);
	while(uiCounter < uiSize && ((cpCmdBuffer[uiCounter] >= '0' && cpCmdBuffer[uiCounter] <= '9') || (cpCmdBuffer[uiCounter] == ' '))){
				uiCounter++;
	}
//	main_setControler(motor, ikp, iki, ikd);
	iState = STATE_IDLE;
	return uiCounter;
}

//============================================================================
// LINE CONTROL STATE MACHINE
//============================================================================
/**
 * Handles parsing while in COOLER_CMD state and checks for transitions
 *
 * @param cpCmdBuffer The start of the command string to parse
 * @param uiSize The size of the command string
 * @param cpCmdRes Buffer for concatenating the command response
 *
 * @return The number of characters parsed while in the COOLER_COMMAND state
 */
int handleLC(char *cpCmdBuffer, unsigned int uiSize, char* cpCmdRes){
	unsigned int uiCounter = 0;
	int ikp;
	int iki;
	int ikd;
	if(!sscanf(cpCmdBuffer, " %d %d %d", &ikp, &iki, &ikd)){
		iState = STATE_ERR;
		return 0;
	}
	strcat(cpCmdRes, ACK_STR);
	while(uiCounter < uiSize && ((cpCmdBuffer[uiCounter] >= '0' && cpCmdBuffer[uiCounter] <= '9') || (cpCmdBuffer[uiCounter] == ' '))){
				uiCounter++;
	}
	lineControl_init(ikp, iki, ikd, 100*1000);
	iState = STATE_IDLE;
	return uiCounter;
}
//============================================================================
// ERROR STATE MACHINE
//============================================================================
/**
 * Handles parsing while in ERR state and checks for transitions
 *
 * @param cpCmdBuffer The start of the command string to parse
 * @param uiSize The size of the command string
 * @param cpCmdRes Buffer for concatenating the command response
 *
 * @return The number of characters parsed while in the ERR state
 */
int handleError(char *cpCmdBuffer, unsigned int uiSize, char* cpCmdRes){
	int uiCounter = 0;
	while(uiCounter < uiSize && iState == STATE_ERR){
		switch(cpCmdBuffer[uiCounter++]){
			case ' ':
			case '\t':
			case '\r':
			case '\n':
			case '\0':
				iState = STATE_IDLE;
				break;
			default:
				break;
		}
	}
	strcat(cpCmdRes, ERR_STR);
	return uiCounter;
}


/**
 * Interpret all commands in the given command buffer.
 * Will trigger the command execution an format an output string for printing
 * Ignores blanks (\r,\n, ,\t,\0) between commands.
 *
 * For each valid command, the string ACK will be concatenated to the response string
 * followed by a line-break and the command response (if any).
 *
 * If invalid command syntax is found ERR will be concatenated to the response string
 * and all the characters before the next blank (\r,\n, ,\t,\0) will be ignored.
 *
 * @param cpCmdBuffer Pointer to command buffer
 * @param uiSize Size of the command buffer
 * @param cpCmdRes Pointer for response string.
 *
 * @after cpCmdRes with the response string for this cmdBuffer,
 * 				this will contain a ACK for each valid commmand in the cmdBuffer
 * 				followed by a line break and the command output (if any).
 * 				If an invalid command syntax is found the output string will contain
 * 				an ERR\n for that command and all characters before the next blank
 * 				(\r,\n, ,\t,\0) will be ignored.
 */
void cmdmachine_interpretCmdBuffer(char *cpCmdBuffer, unsigned int uiSize, char* cpCmdRes){
	iState = STATE_IDLE;
	*cpCmdRes = '\0';	//Start response as an empty string.
	unsigned int uiCounter = 0;
	while(uiCounter < uiSize){
		switch(iState){
			case STATE_IDLE:
				uiCounter += handleIdle(&cpCmdBuffer[uiCounter], uiSize - uiCounter, cpCmdRes);
				break;
			case STATE_MOTOR_CMD:
				uiCounter += handleMotor(&cpCmdBuffer[uiCounter], uiSize - uiCounter, cpCmdRes);
				break;
			case STATE_SC_CMD:
				uiCounter += handleSC(&cpCmdBuffer[uiCounter], uiSize - uiCounter, cpCmdRes);
				break;
			case STATE_LC_CMD:
				uiCounter += handleLC(&cpCmdBuffer[uiCounter], uiSize - uiCounter, cpCmdRes);
				break;
			case STATE_ERR:
				uiCounter += handleError(&cpCmdBuffer[uiCounter], uiSize - uiCounter, cpCmdRes);
				break;
		}
	}
}
