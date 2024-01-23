#ifndef HANDLER_H
#define HANDLER_H

#include "main.h"
#include <string.h>

typedef struct {
    int ledNumber;
    char color[10];
} LedMessage;

enum ACK_TYPE{
	ACK_OK,
	ACK_INVALID,
	ACK_ERR
};

enum LED_COLOR {
	LED_RED,
	LED_GREEN,
	LED_YELLOW
};

int parseMessage(char* message, LedMessage* ledMessage);
void addToHandler(char data[]);
void sendACK(enum ACK_TYPE ack);

#endif
