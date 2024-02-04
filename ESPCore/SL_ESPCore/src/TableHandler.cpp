#include "Handlers/TableHandler.h"


TableHandler::TableHandler(/* args */)
{
    for (uint8_t i = 0; i < 2; i++) {
        seats[i].status = SEAT_NOT_USE;
        seats[i].studentNum = "0";
    }
}

TableHandler::~TableHandler()
{
}

bool TableHandler::sitOn(uint8_t num, String sNum) {
    // if (seats[num-1].status != SEAT_NOT_USE)
    //     return false;

    seats[num-1].status = SEAT_IN_USE_REST;
    seats[num-1].studentNum = sNum;

    Serial.println(seats[num-1].studentNum + "Seat: " + (String)(num-1));

    return true;
}

bool TableHandler::emptySeat(uint8_t num) {
    seats[num-1].status = SEAT_NOT_USE;
    seats[num-1].studentNum = "0";

    return true;
}

SeatStatus TableHandler::distance2SeatStatus(AvoidData distance) {
    bool distanceCondition = distance.distance <= this->seatDistanceThr;
    bool inUseCondition = seats[distance.seatNum-1].studentNum.indexOf("0") == 0 ? false : true;

    Serial.println("index condition: " + (String)seats[distance.seatNum-1].studentNum.indexOf("0") == 0);

    if (!inUseCondition)
        return SEAT_NOT_USE;

    if (distanceCondition && inUseCondition)
        return SEAT_IN_USE;

    if (inUseCondition && !distanceCondition)
        return SEAT_IN_USE_REST;

}

bool TableHandler::getSeatPresence(uint8_t seatNum) {
    AvoidData distance = ParserSerial2Handler::getDistance(seatNum);

    if (seatNum == 0) {
        Serial.println("Failed in getting distance");
        return seats[seatNum-1].status;
    }

    // Serial.printf("PRESENCE: %d - thr: %d -- %d\n", distance.distance, seatDistanceThr, distance.distance <= seatDistanceThr);

    return distance.distance <= seatDistanceThr;

    // return distance2SeatStatus(distance);

}

bool TableHandler::isMovedSinceLast(uint8_t seatNum) {
    PirData lastTrigger = ParserSerial2Handler::getLastTrigger(seatNum);

    if (seatNum == 0) {
        Serial.println("Failed in getting last trigger");
        return seats[seatNum-1].status;
    }

    Serial.printf("last: %d, lastPIRSeatTime: %d\n", lastTrigger.lastTrigTime, lastPIRSeatTime[seatNum-1]);

    if (lastTrigger.lastTrigTime != lastPIRSeatTime[seatNum-1]) {
        lastPIRSeatTime[seatNum-1] = lastTrigger.lastTrigTime;
        return true;
    }
    
    lastPIRSeatTime[seatNum-1] = lastTrigger.lastTrigTime;

    return false;
}

bool TableHandler::sitInUse(uint8_t seatNum) {
    Serial.println("Checking seat " + (String)seatNum);

    if (seats[seatNum-1].status == SEAT_NOT_USE) {
        Serial.println("Checking seat " + (String)seatNum);
        return false;
        
    }

    return true;
}