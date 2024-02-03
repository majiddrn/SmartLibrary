#include "Handlers/TableHandler.h"



TableHandler::TableHandler(/* args */)
{
    seats[0].status = SEAT_NOT_USE;
    seats[1].studentNum = "0";
}

TableHandler::~TableHandler()
{
}

bool TableHandler::sitOn(uint8_t num, String sNum) {
    if (seats[num-1].status != SEAT_NOT_USE)
        return false;

    seats[num-1].status = SEAT_IN_USE_REST;
    seats[num-1].studentNum = sNum;
    seats[num-1].number = num;

    return true;
}