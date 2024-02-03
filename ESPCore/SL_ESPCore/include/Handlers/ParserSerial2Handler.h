#ifndef PARSERHANDLER_H
#define PARSERHANDLER_H

#include <Arduino.h>

struct AvoidData {
    int seatNum;
    int distance;
};

struct PirData {
    int seatNum;
    int lastTrigTime;
};

class ParserSerial2Handler
{
private:
    
public:
    ParserSerial2Handler(/* args */);
    ~ParserSerial2Handler();

    void parseStatus(const String& input);
    void parseAvoid(const String& input, AvoidData& data);
    void parsePir(const String& input, PirData& data);
    
};

#endif