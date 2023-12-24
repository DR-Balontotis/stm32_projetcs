/*
 * hcsr04_driver.h
 *
 *  Created on: Dec 22, 2023
 *      Author: YEDITEPE ROVER TEAM 23/24 ~ ELECTRONIC DEPARTMENT
 */

#ifndef SRC_HCSR04_DRIVER_H_
#define SRC_HCSR04_DRIVER_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

#define TIMER_MAX_VALUE					0xffff			// max value of the use timer
#define READ_DELAY_TIME_IN_MS			100				// delay value of waiting after sending request

#define DIFF_CALC(FIRST, LAST)			((LAST > FIRST) ? (LAST - FIRST) : ((TIMER_MAX_VALUE - FIRST) + LAST))		// calculate the difference between two times
#define DIST_CALC(DIFF)					(DIFF * 0.034/2)															// calculate the distance from difference
#define DIST_CHECK(DIST)				((DIST < 0) ? 0 : ((DIST > 400) ? 0 : DIST))								// check distance and rearrange

// struct for HCSR04 driver
typedef struct sHCSR04
{
	TIM_HandleTypeDef 				*htim;				// user timer
	GPIO_TypeDef*					TrigPort;			// port of hcsr04 module's trigger pin
	uint16_t						TrigPin;			// pin of hcsr04 module's trigger pin
	HAL_TIM_ActiveChannel			TimerChannel;		// Echo pin and timer channel
	uint32_t						CurrentDistance;	// current distance
} hcsr04;

/*
 *		start the hcsr04 driver.
 *		IN:
 *		 	_hcsr04 		-> 		user driver
 *		 	_htim 			-> 		timer for hcsr04
 *		 	TrigPort 		-> 		port value of hcsr04's trigger pin
 *		 	TrigPin 		-> 		pin value of hcsr04's trigger pin
 *		 	_TimerChannel	-> 		connecting echo pin of hcsr04 module to the timer channel
 *		 OUT:
 *		 	(int) 0 is success, else fail
 */
int hcsr04_initialize(hcsr04 *_hcsr04, TIM_HandleTypeDef *_htim, GPIO_TypeDef* TrigPort, uint16_t TrigPin, HAL_TIM_ActiveChannel _TimerChannel);

/*
 * 		read instantaneous distance
 *		IN:
 *		 	_hcsr04 		-> 		user driver
 *		OUT:
 *			(int) distance value (in cm)
 *		Not:
 *			this function includes a delay READ_DELAY_TIME_IN_MS
 *
 */
int hcsr04_readDistance(hcsr04 *_hcsr04);

/*
 * 		timer interrupt
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);

#endif /* SRC_HCSR04_DRIVER_H_ */
