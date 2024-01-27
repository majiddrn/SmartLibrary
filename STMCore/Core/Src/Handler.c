#include "Handler.h"

extern UART_HandleTypeDef huart2;

MessageType getMessageType(const char* message) {
    if (strncmp(message, "LED#", 4) == 0)
        return MESSAGE_TYPE_LED;
    else if (strncmp(message, "PIR#", 4) == 0)
        return MESSAGE_TYPE_PIR;
    else if (strncmp(message, "AVOID#", 6) == 0 || strncmp(message, "AVOIDE#", 7) == 0)
        return MESSAGE_TYPE_AVOID;
    else
        return MESSAGE_TYPE_INVALID;
}

int parseLedMessage(char* message, Message* ledMessage) {
	if (strncmp(message, "LED#", 4) != 0)
		return -1; // Invalid message

	ledMessage->type = MESSAGE_TYPE_LED;

	// Extract the LED number
	char* token = strtok(message + 4, ":");
	if (token == NULL)
		return -1; // Invalid message

	ledMessage->mNumber = atoi(token);

	// Extract the color
	token = strtok(NULL, "#");
	if (token == NULL)
		return -1; // Invalid message

	strcpy(ledMessage->message, token);

	return 0; // Valid message
}

int parsePirMessage(char* message, Message* pirMessage) {
	if (strncmp(message, "PIR#", 4) != 0)
		return -1; // Invalid message

	pirMessage->type = MESSAGE_TYPE_PIR;

	// Extract the PIR number
	char* token = strtok(message + 4, ":");
	if (token == NULL)
		return -1; // Invalid message

	pirMessage->mNumber = atoi(token);

	// Extract the action
	token = strtok(NULL, "#");
	if (token == NULL)
		return -1; // Invalid message

	strcpy(pirMessage->message, token);

	return 0; // Valid message
}

int parseAvoidMessage(char* message, Message* avoidMessage) {
	if ((strncmp(message, "AVOID#", 6) != 0) && (strncmp(message, "AVOIDE#", 7) != 0))
		return -1; // Invalid message

	avoidMessage->type = MESSAGE_TYPE_AVOID;

	// Extract the AVOID number
	char* token = strtok(message + 6, ":");
	if (token == NULL)
		return -1; // Invalid message

	avoidMessage->mNumber = atoi(token);

	// Extract the action
	token = strtok(NULL, "#");
	if (token == NULL)
		return -1; // Invalid message

	strcpy(avoidMessage->message, token);

	return 0; // Valid message
}

int parseMessage(char* message, Message* messageStruct) {
    MessageType messageType = getMessageType(message);
    switch (messageType) {
        case MESSAGE_TYPE_LED:
            return parseLedMessage(message, messageStruct);
        case MESSAGE_TYPE_PIR:
            return parsePirMessage(message, messageStruct);
        case MESSAGE_TYPE_AVOID:
            return parseAvoidMessage(message, messageStruct);
        default:
            return -1; // Invalid message
    }
}

//int parseMessage(char* message, LedMessage* ledMessage) {
//	// Check if the message starts with "LED#"
//	if (strncmp(message, "LED#", 4) != 0)
//		return -1; // Invalid message
//
//	// Extract the LED number
//	char* token = strtok(message + 4, ":");
//	if (token == NULL)
//		return -1; // Invalid message
//
//	ledMessage->ledNumber = atoi(token);
//
//	// Extract the color
//	token = strtok(NULL, "#");
//	if (token == NULL)
//		return -1; // Invalid message
//
//	strcpy(ledMessage->color, token);
//
//	return 0; // Valid message
//}

void addToHandler(char data[]) {
	Message message;
	int parse = parseMessage(data, &message);

	if (parse == -1) {
		sendACK(ACK_INVALID);
		return;
	}

	int funcStat;

	switch (message.type) {
		case MESSAGE_TYPE_LED:
			funcStat = doLEDFunc(message);
			break;
		case MESSAGE_TYPE_AVOID:
			break;
		case MESSAGE_TYPE_PIR:
			break;
		default:
			break;
	}

	if (funcStat == -1) {
		sendACK(ACK_ERR);
		return;
	}

	if (message.type == MESSAGE_TYPE_LED)
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
//			else
//				HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 1);
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
