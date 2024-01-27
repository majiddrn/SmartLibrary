#ifndef HANDLER_H
#define HANDLER_H

#include "main.h"
#include <string.h>
#include "RGBHandler.h"
#include "CommonHandlers.h"

int parseMessage(char* message, Message* ledMessage);
void addToHandler(char data[]);
void sendACK(enum ACK_TYPE ack);

#endif
