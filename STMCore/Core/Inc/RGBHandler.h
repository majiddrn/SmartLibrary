#ifndef RGBHANDLER_H
#define RGBHANDLER_H

#include "main.h"
#include <string.h>
#include "CommonHandlers.h"
//#include "Handler.h"

typedef struct {
	uint8_t ledNumber;
	TIM_HandleTypeDef* htim_;
} RGBTimer;

enum LED_COLOR {
	LED_RED,
	LED_GREEN,
	LED_YELLOW
};

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

int doLEDFunc(Message message);

#endif
