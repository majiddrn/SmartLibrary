#ifndef PARSERHANDLER_H
#define PARSERHANDLER_H

#include <Arduino.h>
#include "SensorsModulesCommon.h"

class ParserSerial2Handler
{
private:
    static void __wait();
    static String __readSerial();
    static String buffer;
    static String color2String(LEDColor);
public:
    ParserSerial2Handler(/* args */);
    ~ParserSerial2Handler();

    static void parseStatus(const String& input);
    static AvoidData parseAvoid(const String& input);
    static PirData parsePir(const String& input);

    static AvoidData getDistance(uint8_t seatNum);
    static PirData getLastTrigger(uint8_t seatNum);
    static bool setLED(uint8_t seatNum, LEDColor);
};

#endif