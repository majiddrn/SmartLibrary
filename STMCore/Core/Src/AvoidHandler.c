#include "AvoidHandler.h"

extern UART_HandleTypeDef huart2;
extern osMutexId_t uartTransmitHandle;

int doAVOIDFunc(Message avoidMessage) {
	if (avoidMessage.mNumber > maxSeats)
		return -1;

	char tmp[20];

	switch (avoidMessage.mNumber) {
		case 1:
			osMutexAcquire(AvoidDistance1Handle, portMAX_DELAY);
			break;
		case 2:
			osMutexAcquire(AvoidDistance2Handle, portMAX_DELAY);
			break;
		default:
			break;
	}

	sprintf(tmp, "AVOID#%d:%d\r", avoidMessage.mNumber, rawAVOID[avoidMessage.mNumber-1]);

	osMutexAcquire(uartTransmitHandle, portMAX_DELAY);
	HAL_UART_Transmit(&huart2, tmp, strlen(tmp), 250);
	osMutexRelease(uartTransmitHandle);

	switch (avoidMessage.mNumber) {
		case 1:
			osMutexRelease(AvoidDistance1Handle);
			break;
		case 2:
			osMutexRelease(AvoidDistance2Handle);
			break;
		default:
			break;
	}

	return 1;
}

uint32_t getAvoidDistance(uint8_t avoidNum) {
	TIM_TypeDef* timer_ = NULL;

	GPIO_TypeDef* gpioType = NULL;
	uint16_t pinNumEcho, pinNumTrig;

	uint8_t timeNeeded = 10;

	switch (avoidNum) {
		case 1:
			timer_ = TIM4;

			gpioType = GPIOB;
			pinNumEcho = GPIO_PIN_4;
			pinNumTrig = GPIO_PIN_5;
			break;
		case 2:
			timer_ = TIM4;

			gpioType = GPIOD;
			pinNumEcho = GPIO_PIN_11;
			pinNumTrig = GPIO_PIN_10;
			break;
		default:
			break;
	}

	HAL_GPIO_WritePin(gpioType, pinNumTrig, 1);
	timer_->CNT = 0;
	while(timer_->CNT <= timeNeeded);
	HAL_GPIO_WritePin(gpioType, pinNumTrig, 0);

	while (HAL_GPIO_ReadPin(gpioType, pinNumEcho) == 0 && timer_->CNT <= 20000);

	timer_->CNT = 0;
	while(HAL_GPIO_ReadPin(gpioType, pinNumEcho) == 1 && timer_->CNT <= 10000);

	uint32_t tof = timer_->CNT;
	uint32_t distance = tof / 58;

	return distance;

	if (timer_ == NULL)
		return 0;



}
