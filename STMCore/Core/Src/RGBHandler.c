#include "RGBHandler.h"

uint8_t rgbLedsCount = 2;

RGBTimer rgbLeds[] = {
		{1, &htim2},
		{2, &htim3}
};

int doLEDFunc(Message ledMessage) {
	enum LED_COLOR color;
	if (strstr(ledMessage.message, "GREEN"))
		color = LED_GREEN;

	else if (strstr(ledMessage.message, "YELLOW"))
		color = LED_YELLOW;

	else if (strstr(ledMessage.message, "RED"))
		color = LED_RED;

	else
		return -1;

	TIM_HandleTypeDef* htim_;

	int found = 0;

	for (uint8_t i = 0; i < rgbLedsCount; i++) {
		if (rgbLeds[i].ledNumber == ledMessage.mNumber) {
			found = 1;
			htim_ = rgbLeds[i].htim_;
		}
	}

	if (found == 0)
		return -1;

	/*
	 * TIM_CHANNEL_1 represents Red
	 * TIM_CHANNEL_2 represents Green
	 * TIM_CHANNEL_2 represents Blue
	 * */

	switch (color) {
		case LED_GREEN:
			__HAL_TIM_SET_COMPARE(htim_, TIM_CHANNEL_1, 0);
			__HAL_TIM_SET_COMPARE(htim_, TIM_CHANNEL_2, 25);
			__HAL_TIM_SET_COMPARE(htim_, TIM_CHANNEL_3, 0);
			break;

		case LED_YELLOW:
			__HAL_TIM_SET_COMPARE(htim_, TIM_CHANNEL_1, 12);
			__HAL_TIM_SET_COMPARE(htim_, TIM_CHANNEL_2, 12);
			__HAL_TIM_SET_COMPARE(htim_, TIM_CHANNEL_3, 0);
			break;

		case LED_RED:
			__HAL_TIM_SET_COMPARE(htim_, TIM_CHANNEL_1, 25);
			__HAL_TIM_SET_COMPARE(htim_, TIM_CHANNEL_2, 0);
			__HAL_TIM_SET_COMPARE(htim_, TIM_CHANNEL_3, 0);
			break;
		default:
			break;
	}

	return 1;
}
