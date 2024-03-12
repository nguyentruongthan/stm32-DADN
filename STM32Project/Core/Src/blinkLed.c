/*
 * blink_led.c
 *
 *  Created on: Mar 12, 2024
 *      Author: truong_than
 */
#include <blinkLed.h>

void blinkLedEvery500ms(){
	HAL_GPIO_TogglePin(BLINK_LED_GPIO_Port, BLINK_LED_Pin);
}
