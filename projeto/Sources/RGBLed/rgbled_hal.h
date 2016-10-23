/* ***************************************************************** */
/* File name:        rgbled_hal.h                                    */
/* File description: Header file containing the functions/methods    */
/*                   interfaces for rgbled control	  			     */
/* Author name:      ddello32	                                     */
/* Creation date:    01out2016                                       */
/* Revision date:    01out2016                                       */
/* ***************************************************************** */

#ifndef SOURCES_RGBLED_HAL_H_
#define SOURCES_RGBLED_HAL_H_

//#define USERGBPWM

#include <stdint.h>

/**
 * Initialize the rgbled module
 */
void rgbled_init(void);


/**
 * Set the rgb led color
 * @param uiR red intensity from 0 (black) to 0xFFFF (red)
 * @param uiG green intensity from 0 (black) to 0xFFFF (green)
 * @param uiB blue intensity from 0 (black) to 0xFFFF (blue)
 */
void rgbled_setColor(uint16_t uiR, uint16_t uiG, uint16_t uiB);

#endif /* SOURCES_RGBLED_HAL_H_ */
