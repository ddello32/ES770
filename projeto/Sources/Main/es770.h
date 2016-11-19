/* ***************************************************************** */
/* File name:        cmdmachine_hal.h                                */
/* File description: Header file containing the functions/methods    */
/*                   interfaces for protocol command machine		 */
/* Author name:      ddello		                                     */
/* Creation date:    27abr2016                                       */
/* Revision date:    27abr2016                                       */
/* ***************************************************************** */

#ifndef SOURCES_MAIN_H_
#define SOURCES_MAIN_H_

void main_setSpeeds(int iLinSpeed, int iAngSpeed);

void main_setControler(unsigned short motor, int ikp, int iki, int ikd);

#endif /* SOURCES_MAIN_H_ */
