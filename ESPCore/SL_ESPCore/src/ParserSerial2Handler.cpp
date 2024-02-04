#include "Handlers/ParserSerial2Handler.h"

String ParserSerial2Handler::buffer = "";

#define BUFFER_TIME_OUT 10000

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

AvoidData ParserSerial2Handler::parseAvoid(const String& input) {
    AvoidData data {0, 0};
    if (input.startsWith("AVOID#")) {
        data.seatNum = input.substring(6).toInt();
        data.distance = input.substring(8).toInt();

        // Serial.printf("Seat %d: %d\n", data.seatNum, data.distance);
    } else if (input.indexOf("INVALID") >= 0) {
        Serial.println("Invalid\n");
    } else if (input.indexOf("ERROR") >= 0) {
        Serial.println("Error\n");
    } else {
        Serial.println("Unknown\n");
    }

    return data;
}

PirData ParserSerial2Handler::parsePir(const String& input) {
    PirData data {0, 0};
    if (input.startsWith("PIR#")) {
        data.seatNum = input.substring(4).toInt();
        data.lastTrigTime = input.substring(6).toInt();

        // Serial.printf("PIR Seat %d: %d\n", data.seatNum, data.lastTrigTime);
    } else if (input.indexOf("INVALID") >= 0) {
        Serial.println("Invalid\n");

    } else if (input.indexOf("Error") >= 0) {
        Serial.println("Error\n");
    } else {
        Serial.println("Unknown\n");
    }

    return data;
}

void ParserSerial2Handler::__wait() {
    buffer = "";
    unsigned long t = millis();

    while(!Serial2.available() && t - millis() <= BUFFER_TIME_OUT) vTaskDelay(10 / portTICK_PERIOD_MS);

    while(Serial2.available()) {
        buffer += Serial2.readStringUntil('\r');
    }
}

String ParserSerial2Handler::__readSerial() {
    __wait();
    return buffer;
}

String ParserSerial2Handler::color2String(LEDColor color) {
    switch (color)
    {
    case LED_COLOR_GREEN:
        return "GREEN";
        break;
    case LED_COLOR_RED:
        return "RED";
        break;
    case LED_COLOR_YELLOW:
        return "YELLOW";
        break;
    default:
        break;
    }
}

bool ParserSerial2Handler::setLED(uint8_t seatNum, LEDColor ledColor) {
    String response = "";
    unsigned long t = millis();
    do{
        // vTaskDelay(20 / portTICK_PERIOD_MS);

        Serial2.printf("LED#%d:%s\r", seatNum, color2String(ledColor).c_str());
        
        Serial.printf("LED#%d:%s\n", seatNum, color2String(ledColor).c_str());

        // vTaskDelay(200 / portTICK_PERIOD_MS);
        response = __readSerial();
        response.trim();
        // Serial.println("LED: " + response + (String)response.equals("OK"));


        if (response.equals("OK"))
            break;
    } while(millis() - t <= BUFFER_TIME_OUT);

    if (response.indexOf("OK") >= 0)
        return true;

    Serial.println("Failed\n");

    return false;
}

PirData ParserSerial2Handler::getLastTrigger(uint8_t seatNum) {
    String response = "";
    unsigned long t = millis();
    do {
        // vTaskDelay(50 / portTICK_PERIOD_MS);

        Serial2.printf("PIR#%d:ASK_LAST\r", seatNum);

        Serial.printf("PIR#%d:ASK_LAST\r", seatNum);

        // vTaskDelay(200 / portTICK_PERIOD_MS);
        response = __readSerial();
    } while(response.indexOf("PIR") < 0 && millis() - t <= BUFFER_TIME_OUT);

    if (response.indexOf("PIR") < 0)
        return {0, 0};
    
    Serial.println("Failed\n");

    PirData lastTriger = parsePir(response);

    return lastTriger;
}

AvoidData ParserSerial2Handler::getDistance(uint8_t seatNum) {
    String response = "";
    unsigned long t = millis();
    do {
        // vTaskDelay(20 / portTICK_PERIOD_MS);

        Serial2.printf("AVOID#%u:ASK_AVOIDANCE\r", seatNum);

        Serial.printf("AVOID#%u:ASK_AVOIDANCE\n", seatNum);

        // vTaskDelay(50 / portTICK_PERIOD_MS);
        response = __readSerial();
        Serial.println(response);
    } while(response.indexOf("AVOID") < 0 && millis() - t <= BUFFER_TIME_OUT);

    if (response.indexOf("AVOID") < 0)
        return {0, 0};

    Serial.println("Failed\n");

    AvoidData distance = parseAvoid(response);

    // Serial.println("DISTANCE: " + (String)distance.distance + " " + (String)distance.seatNum);

    return distance;
}