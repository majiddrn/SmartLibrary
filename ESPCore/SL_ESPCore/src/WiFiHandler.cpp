#include "Handlers/WiFiHandler.h"

WiFiHandler::WiFiHandler()
{
}

WiFiHandler::~WiFiHandler()
{
}

void WiFiHandler::connectTCP() {
    client.connect(localServerIp.c_str(), localServerPort);

    vTaskDelay(5000);

    if (firstInit) {
        client.println("set a1 1 fum");
        client.println("set a2 1 fum");
        firstInit = false;
    }
}

void WiFiHandler::begin(String ssid, String password) {
    WiFi.begin(ssid.c_str(), password.c_str());

    Serial.println("Connecting to WiFi");

    while (!WiFi.isConnected()) {
        Serial.print("..");
        vTaskDelay(1000);
    }
    
    Serial.println();

    connectTCP();
    // client.connect()
}