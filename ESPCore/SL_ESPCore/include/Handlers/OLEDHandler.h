#ifndef OLED_HANDLER_H
#define OLED_HANDLER_H

#include "SensorsModulesCommon.h"

enum OLEDMessageType {
    NUMBER_MESSAGE,
    INCREASE_NUMBER_MESSAGE,
    STRING_MESSAGE
};

typedef struct OLEDMessage {
    OLEDMessageType messageType;
    bool increase;
    uint8_t number;
    String message;
};


class OLEDHandler
{
private:
    uint8_t number{1};
    uint8_t maxNumber{2};
public:
    OLEDHandler(bool isBegin = false);
    ~OLEDHandler();
    void beginOLEDSeat();
    void setNumber(uint8_t num);
    void drawScreen(uint8_t num, bool clear = false);
    void drawString(String);
    uint8_t increaseNumber();
    uint8_t getNumber();
};


#endif