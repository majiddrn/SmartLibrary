#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "main.h"
#include "cmsis_os.h"
#include "Callbacks.h"

extern UART_HandleTypeDef huart2;
extern osSemaphoreId_t diffRxDataHandle;
extern char data_c;
extern char data[150];
extern uint8_t data_c_idx;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//	HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_10);
	if (huart->Instance == huart2.Instance) {
	        data[data_c_idx] = data_c;
	        if (data_c == '\r') {
	            data[data_c_idx] = '\0';
	            data_c_idx = 0;
	            osSemaphoreRelease(diffRxDataHandle);
	        } else
	        	data_c_idx++;
	        HAL_UART_Receive_IT(&huart2, &data_c, 1);
	}
}

#endif
