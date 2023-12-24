/*
 * hcsr04_driver.c
 *
 *  Created on: Dec 22, 2023
 *      Author: YEDITEPE ROVER TEAM 23/24 ~ ELECTRONIC DEPARTMENT
 */

#include "hcsr04_driver.h"

#include <stdint.h>
#include "stm32f4xx_hal.h"

#include "stdio.h"
/*
 * for access from timer interrupt
 */
static hcsr04 *HCSR04_n;

static uint32_t val1 = 0;
static uint32_t val2 = 0;
static uint8_t Is_First_Captured = 0;

/*
 *		start the hcsr04 driver.
 *		IN:
 *		 	_hcsr04 		-> 		user driver
 *		 	_htim 			-> 		timer for hcsr04
 *		 	TrigPort 		-> 		port value of hcsr04's trigger pin
 *		 	TrigPin 		-> 		pin value of hcsr04's trigger pin
 *		 	_TimerChannel 	-> 		connecting echo pin of hcsr04 module to the timer channel
 *		 OUT:
 *		 	(int) 0 is success, else fail
 */
int hcsr04_initialize(hcsr04 *_hcsr04, TIM_HandleTypeDef *_htim, GPIO_TypeDef* _TrigPort, uint16_t _TrigPin, HAL_TIM_ActiveChannel _TimerChannel)
{
	_hcsr04->htim = _htim;
	_hcsr04->TrigPort = _TrigPort;
	_hcsr04->TrigPin = _TrigPin;
	_hcsr04->TimerChannel = _TimerChannel;

	HCSR04_n = _hcsr04;

	return (0);
}

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
int hcsr04_readDistance(hcsr04 *_hcsr04)
{

	HAL_GPIO_WritePin(_hcsr04->TrigPort, _hcsr04->TrigPin, GPIO_PIN_SET);

	// wait for 10 us
	__HAL_TIM_SET_COUNTER(_hcsr04->htim, 0);
	while (__HAL_TIM_GET_COUNTER(_hcsr04->htim) < 10);

	HAL_GPIO_WritePin(_hcsr04->TrigPort, _hcsr04->TrigPin, GPIO_PIN_RESET);

	__HAL_TIM_ENABLE_IT(_hcsr04->htim, TIM_IT_CC1);

	// wait for response
	HAL_Delay(READ_DELAY_TIME_IN_MS);

	return _hcsr04->CurrentDistance;
}

/*
 * 		timer interrupt
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	uint32_t difference = 0, distance = 0;
	if (htim->Channel == HCSR04_n->TimerChannel)
	{
		if (Is_First_Captured == 0)
		{
			val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
			Is_First_Captured = 1;
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
		}
		else if (Is_First_Captured == 1)
		{
			val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
			__HAL_TIM_SET_COUNTER(htim, 0);

			difference 					= (uint32_t)(DIFF_CALC(val1, val2));
			distance					= (uint32_t)(DIST_CALC(difference));
			HCSR04_n->CurrentDistance 	= (uint32_t)(DIST_CHECK(distance));

			Is_First_Captured = 0;

			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(HCSR04_n->htim, TIM_IT_CC1);
		}
	}
}



















