/*
 * File name: photoSensor.h
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
 * Measure light level for the given sensor
 * @param usSensorNumber - from 0 to 5
 * @return The conversion result
 */
unsigned int photoSensor_measure(unsigned short usSensorNumber);

/**
 * Save photo sensor calibration
 * @param usSensorNumber - from 0 to 5
 * @param uiLightVal - value when measuring white
 * @param uiDarkVal - value when measuring black
 */
//void photoSensor_calibrate(unsigned short usSensorNumber, unsigned int uiLightVal, unsigned int darkVal);
#endif /* SOURCES_PHOTOSENSOR_HAL_H_ */
