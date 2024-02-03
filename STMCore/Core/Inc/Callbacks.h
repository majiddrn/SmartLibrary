#ifndef CALLBACKS_H
#define CALLBACKS_H

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif
