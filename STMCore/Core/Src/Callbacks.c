#include "main.h"
#include "cmsis_os.h"
#include "Callbacks.h"

extern UART_HandleTypeDef huart2;
extern osSemaphoreId_t diffRxDataHandle;
extern char data_c;
extern char data[150];
extern uint8_t data_c_idx;

extern TIM_HandleTypeDef htim8;

extern uint32_t pirTime[2];

extern osMutexId_t pirTime1Handle;
extern osMutexId_t pirTime2Handle;

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

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == PIR_MOVE_1_Pin) {
		osMutexAcquire(pirTime1Handle, portMAX_DELAY);
		pirTime[0] = TIM8->CNT;
		osMutexRelease(pirTime1Handle);
//		HAL_UART_Transmit(&huart2, "Seat 1 moved\n", 13, 250);
	}

	if (GPIO_Pin == PIR_MOVE_2_Pin) {
		osMutexAcquire(pirTime2Handle, portMAX_DELAY);
		pirTime[1] = TIM8->CNT;
		osMutexRelease(pirTime2Handle);
//		HAL_UART_Transmit(&huart2, "Seat 2 moved\n", 13, 250);
	}
}
