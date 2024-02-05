#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H

#include "SensorsModulesCommon.h"
#include <WiFi.h>

class WiFiHandler
{
private:
    String localServerIp = "192.168.43.244";
    uint16_t localServerPort = 9898;
    bool firstInit = true;
public:
    WiFiHandler();
    ~WiFiHandler();
    void begin(String ssid, String password);
    void connectTCP();
    WiFiClient client;
};


#endif