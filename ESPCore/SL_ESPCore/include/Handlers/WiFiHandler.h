#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H

#include "SensorsModulesCommon.h"
#include <WiFi.h>

class WiFiHandler
{
private:
    WiFiClient client;
public:
    WiFiHandler();
    ~WiFiHandler();
    void begin(String ssid, String password);
    void connectTCP();
};


#endif