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
 * Triggers a measurement
 */
void encoder_measure();


/**
 * @param usEncoderNumber encoder identifier
 * @param uiPeriod Time ellapsed between measure calls in us
 *
 * @return current encoder speed in rps for given encoder number
 */
unsigned int encoder_getCurrentSpeed(unsigned short usEncoderNumber, unsigned int uiPeriod);

/**
 * @return linear distance walked by the CG (from last measure)
 */
unsigned int encoder_getLinDistance();

/**
 * @param usEncoderNumber encoder identifier
 * @param uiPeriod Time ellapsed between measure calls in us
 *
 * @return mean encoder speed (for last MEASURE_BUFFER_SIZE measures) in mm/sec for given encoder number
 */
unsigned int encoder_getMeanSpeed(unsigned short usEncoderNumber, unsigned int uiPeriod);

#endif /* SOURCES_ENCODER_HAL_H_ */
