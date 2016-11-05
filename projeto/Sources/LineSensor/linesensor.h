/*
 * File name: linesensor_hal.h
 *
 *  Created on: 02/11/2016
 *      Author: ddello32
 */

#ifndef SOURCES_LINESENSOR_H_
#define SOURCES_LINESENSOR_H_
/**
 *  Configure LineSensor module
 */
void lineSensor_init(void);

/**
 * Measure distance between car and line
 * @return The distance between car center and line
 */
int lineSensor_measure();
#endif /* SOURCES_LINESENSOR_H_ */
