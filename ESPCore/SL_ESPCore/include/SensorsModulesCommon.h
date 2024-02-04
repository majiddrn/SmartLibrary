#ifndef SENSORS_MODULES_COMMON_H
#define SENSORS_MODULES_COMMON_H

#include "SSD1306Wire.h"

typedef enum LEDColor {
    LED_COLOR_RED,
    LED_COLOR_GREEN,
    LED_COLOR_YELLOW
};

struct AvoidData {
    uint8_t seatNum;
    uint32_t distance;
};

struct PirData {
    uint8_t seatNum;
    uint32_t lastTrigTime;
};

#endif