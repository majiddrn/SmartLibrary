#include "Handlers/WiFiHandler.h"

WiFiHandler::WiFiHandler()
{
}

WiFiHandler::~WiFiHandler()
{
}

void WiFiHandler::connectTCP() {
    // client.connect(host, 111);
}

void WiFiHandler::begin(String ssid, String password) {
    WiFi.begin(ssid.c_str(), password.c_str());

    connectTCP();
    // client.connect()
}