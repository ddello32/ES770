/* ***************************************************************** */
/* File name:        motor_hal.h                                    */
/* File description: Header file containing the functions/methods    */
/*                   interfaces for motor control	  			     */
/* Author name:      ddello32	                                     */
/* Creation date:    01out2016                                       */
/* Revision date:    01out2016                                       */
/* ***************************************************************** */

#ifndef SOURCES_MOTOR_HAL_H_
#define SOURCES_MOTOR_HAL_H_

/**
 * Initialize the motor module
 */
void motor_init(void);


/**
 * Set the motor speed
 * @param usMotorNumber motor identifier (0, 1)
 * @param iSpeed the desired speed for this motor (from -0xFFFF to 0xFFFF)
 */
void motor_setSpeed(unsigned short usMotorNumber, int iSpeed);

#endif /* SOURCES_MOTOR_HAL_H_ */
