#ifndef TABLEHANDLER_H
#define TABLEHANDLER_H

#include <Arduino.h>
#include "SensorsModulesCommon.h"
#include "Handlers/ParserSerial2Handler.h"

typedef enum SeatStatus {
    SEAT_IN_USE_REST,
    SEAT_IN_USE,
    SEAT_NOT_USE
};

typedef struct Seat {
    String studentNum;
    SeatStatus status;
};

class TableHandler
{
private:
    uint16_t seatDistanceThr {40};
    uint16_t lastPIRSeatTime[2];
    uint8_t seatNumbers = 2;
    SeatStatus distance2SeatStatus(AvoidData distance);
public:
    TableHandler(/* args */);
    ~TableHandler();
    bool sitOn(uint8_t, String);
    bool emptySeat(uint8_t);
    bool getSeatPresence(uint8_t seatNum);
    bool isMovedSinceLast(uint8_t seaNum);              // will only be called when 
    bool sitInUse(uint8_t seatNum);
    Seat seats[2];
    uint8_t getSeatNumbers() {
        return seatNumbers;
    }
};


#endif