#include "Handler.h"

extern UART_HandleTypeDef huart2;

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

RGBTimer rgbLeds[] = {
		{1, &htim2},
		{2, &htim3}
};

uint8_t rgbLedsCount = 2;

int parseMessage(char* message, LedMessage* ledMessage) {
	// Check if the message starts with "LED#"
	if (strncmp(message, "LED#", 4) != 0)
		return -1; // Invalid message

	// Extract the LED number
	char* token = strtok(message + 4, ":");
	if (token == NULL)
		return -1; // Invalid message

	ledMessage->ledNumber = atoi(token);

	// Extract the color
	token = strtok(NULL, "#");
	if (token == NULL)
		return -1; // Invalid message

	strcpy(ledMessage->color, token);

	return 0; // Valid message
}

int doLEDFunc(LedMessage ledMessage) {
	enum LED_COLOR color;
	if (strstr(ledMessage.color, "GREEN"))
		color = LED_GREEN;

	else if (strstr(ledMessage.color, "YELLOW"))
		color = LED_YELLOW;

	else if (strstr(ledMessage.color, "RED"))
		color = LED_RED;

	else
		return -1;

	TIM_HandleTypeDef* htim_;

	int found = 0;

	for (uint8_t i = 0; i < rgbLedsCount; i++) {
		if (rgbLeds[i].ledNumber == ledMessage.ledNumber) {
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

void addToHandler(char data[]) {
	LedMessage ledMessage;
	int parse = parseMessage(data, &ledMessage);

	if (parse == -1) {
		sendACK(ACK_INVALID);
		return;
	}

	int funcStat = doLEDFunc(ledMessage);

	if (funcStat == -1) {
		sendACK(ACK_ERR);
		return;
	}

	sendACK(ACK_OK);
}


void sendACK(enum ACK_TYPE ack) {
	char tmp[20];
	int len;
	HAL_StatusTypeDef status;

	switch (ack) {
		case ACK_INVALID:
			len = sprintf(tmp, "INVALID\r");
			status = HAL_UART_Transmit(&huart2, tmp, len, 250);

			break;
		case ACK_OK:
			len = sprintf(tmp, "OK\r");
			status = HAL_UART_Transmit(&huart2, tmp, len, 250);

			break;
		case ACK_ERR:
			len = sprintf(tmp, "ERROR\r");
			status = HAL_UART_Transmit(&huart2, tmp, len, 250);

			break;
		default:
			break;
	}
}
