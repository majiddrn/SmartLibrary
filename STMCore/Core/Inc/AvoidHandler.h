#ifndef AVOIDHANDLER_H
#define AVOIDHANDLER_H

#include "main.h"
#include "CommonHandlers.h"
#include "cmsis_os.h"

extern uint32_t rawAVOID[2];
extern uint8_t maxSeats;

extern osMutexId_t AvoidDistance1Handle;
extern osMutexId_t AvoidDistance2Handle;

int doAVOIDFunc(Message avoidMessage);
uint32_t getAvoidDistance(uint8_t avoidNum);

#endif
