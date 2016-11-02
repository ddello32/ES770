/*
 * File name: photosensor_hal.h
 *
 *  Created on: 01/10/2016
 *      Author: ddello32
 */

#ifndef SOURCES_PHOTOSENSOR_HAL_H_
#define SOURCES_PHOTOSENSOR_HAL_H_
/**
 *  Configure PhotoSensor module
 */
void photoSensor_init(void);

/**
 * Measure raw light level for the given sensor (already without DC offset)
 * @param usSensorNumber - from 0 to 5
 * @return The conversion result
 */
int photoSensor_measure_raw(unsigned short usSensorNumber);

/**
 * Measure light level for the given sensor in a scale from 0 to 100.
 * Should only be called after calibration
 * @param usSensorNumber - from 0 to 5
 * @return The conversion result
 */
unsigned short photoSensor_measure(unsigned short usSensorNumber);

/**
 * Save photo sensor calibration
 * @param usSensorNumber - from 0 to 5
 * @param iLightVal - value when measuring white
 * @param iDarkVal - value when measuring black
 */
void photoSensor_calibrate(unsigned short usSensorNumber, int iLightVal, int iDarkVal);

#endif /* SOURCES_PHOTOSENSOR_HAL_H_ */
