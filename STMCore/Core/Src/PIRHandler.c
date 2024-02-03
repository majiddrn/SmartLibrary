#include "PIRHandler.h"

extern UART_HandleTypeDef huart2;

extern TIM_HandleTypeDef htim8;

extern osMutexId_t pirTime1Handle;
extern osMutexId_t pirTime2Handle;

extern uint32_t pirTime[2];

extern uint8_t maxSeats;

int doPIRFunc(Message pirMessage) {
	if (pirMessage.mNumber > maxSeats)
		return -1;

	char tmp[30];

	switch (pirMessage.mNumber	) {
		case 1:
			osMutexAcquire(pirTime1Handle, portMAX_DELAY);
			break;
		case 2:
			osMutexAcquire(pirTime2Handle, portMAX_DELAY);
			break;
		default:
			break;
	}

	sprintf(tmp, "PIR#%d:%d\r", pirMessage.mNumber, pirTime[pirMessage.mNumber-1]);
	HAL_UART_Transmit(&huart2, tmp, strlen(tmp), 250);

	switch (pirMessage.mNumber) {
		case 1:
			osMutexRelease(pirTime1Handle);
			break;
		case 2:
			osMutexRelease(pirTime2Handle);
			break;
		default:
			break;
	}

	return 1;

}
