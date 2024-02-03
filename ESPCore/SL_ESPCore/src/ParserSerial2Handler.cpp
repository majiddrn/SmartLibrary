#include "Handlers/ParserSerial2Handler.h"

ParserSerial2Handler::ParserSerial2Handler(/* args */)
{
}

ParserSerial2Handler::~ParserSerial2Handler()
{
}


void ParserSerial2Handler::parseStatus(const String& input) {
    if (input.indexOf("OK") >= 0) {
        Serial.println("Done\n");
    } else if (input.indexOf("INVALID") >= 0) {
        Serial.println("Invalid\n");
    } else if (input.indexOf("ERROR") >= 0) {
        Serial.println("Error\n");
    } else {
        Serial.println("Unknown\n");
    }
}

void ParserSerial2Handler::parseAvoid(const String& input, AvoidData& data) {
    if (input.startsWith("AVOID#")) {
        data.seatNum = input.substring(6).toInt();
        data.distance = input.substring(8).toInt();

        Serial.printf("Seat %d: %d\n", data.seatNum, data.distance);
    } else if (input.indexOf("INVALID") >= 0) {
        Serial.println("Invalid\n");
    } else if (input.indexOf("ERROR") >= 0) {
        Serial.println("Error\n");
    } else {
        Serial.println("Unknown\n");
    }
}

void ParserSerial2Handler::parsePir(const String& input, PirData& data) {
    if (input.startsWith("PIR#")) {
        data.seatNum = input.substring(4).toInt();
        data.lastTrigTime = input.substring(6).toInt();

        Serial.printf("PIR Seat %d: %d\n", data.seatNum, data.lastTrigTime);
    } else if (input.indexOf("INVALID") >= 0) {
        Serial.println("Invalid\n");
    } else if (input.indexOf("Error") >= 0) {
        Serial.println("Error\n");
    } else {
        Serial.println("Unknown\n");
    }
}