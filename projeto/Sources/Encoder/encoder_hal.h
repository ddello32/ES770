/* ***************************************************************** */
/* File name:        tacometro_hal.h                                 */
/* File description: Header file containing the functions/methods    */
/*                   interfaces for tacometer 						 */
/* Author name:      ddello		                                     */
/* Creation date:    13maio2016                                      */
/* Revision date:    13maio2016                                      */
/* ***************************************************************** */

#ifndef SOURCES_ENCODER_HAL_H_
#define SOURCES_ENCODER_HAL_H_

/**
 * Initialize the encoder module
 */
void encoder_init(void);


/**
 * @param usEncoderNumber encoder identifier
 * @param uiPeriod Time ellapsed since last getSpeedCall in ms
 *
 * @return current encoder speed in rps for given encoder number
 */
unsigned int encoder_getSpeed(unsigned short usEncoderNumber, unsigned int uiPeriod);

#endif /* SOURCES_ENCODER_HAL_H_ */
