#ifndef TABLEHANDELR_H
#define TABLEHANDLER_H

#include <Arduino.h>

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
    
public:
    TableHandler(/* args */);
    ~TableHandler();
    bool sitOn(uint8_t, String);
    Seat seats[2];
};


#endif