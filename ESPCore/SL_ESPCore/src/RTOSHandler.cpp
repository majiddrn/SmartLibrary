#include "Handlers/RTOSHandler.h"
#include "Handlers/OLEDHandler.h"
#include "Handlers/WiFiHandler.h"

#define DEAFAULT_CORE 1
#define KB 1024

#define SIT_CHANGE_BUTTON_PIN 14 

#define WIFI_SSID "mohammad"
#define WIFI_PASSWORD "md1234567997MD"

OLEDHandler oledHandlerObj(false);
WiFiHandler wifiHandlerObj;

#define LAST_CHANGE_BUTTON_DEBOUNCE 250
unsigned long lastSitChangePressed = 0;

////////////////////////////// Prioriorities \\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

#define OLED_HANDLER_PRIORITY 2

////////////////////////////// Prioriorities \\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

//////////////////////////// Semaphore Queues \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

QueueHandle_t oledQueue = NULL;
SemaphoreHandle_t differLastChangeSemaphore = NULL;

//////////////////////////// Semaphore Queues \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

//////////////////////////////// Handlers \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

//////////////////////////////// Handlers \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

////////////////////////////// RTOS Handlers \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

TaskHandle_t oledHandlerTask = NULL;

////////////////////////////// RTOS Handlers \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

/////////////////////////////// Interrupts \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

void IRAM_ATTR sitChangeISR() {
    if (millis() - lastSitChangePressed >= LAST_CHANGE_BUTTON_DEBOUNCE) {
        OLEDMessage oledMessage {
            INCREASE_NUMBER_MESSAGE,
            true,
            0,
            "NULL"
        };
        BaseType_t xType = pdFALSE;
        xQueueSendFromISR(oledQueue, &oledMessage, &xType);

        lastSitChangePressed = millis();
    }
}

/////////////////////////////// Interrupts \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

////////////////////////////// Task Functions \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

void oledHandlerTask_(void * parameter) {
    oledHandlerObj.setNumber(1);
    
    while (true)
    {
        OLEDMessage message;
        xQueueReceive(oledQueue, &message, portMAX_DELAY);

        Serial.println("Pressed" + (String)message.increase);

        if (message.increase) {
            Serial.println("Increase");
            oledHandlerObj.increaseNumber();
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    
}

////////////////////////////// Task Functions \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

void initQueueSemaphores() {
    oledQueue = xQueueCreate(1, sizeof(OLEDMessage));
    differLastChangeSemaphore = xSemaphoreCreateBinary();
    xSemaphoreTake(differLastChangeSemaphore, 250);
}

void initWiFi() {
    wifiHandlerObj.begin(WIFI_SSID, WIFI_PASSWORD);


}

void initInterrupts() {
    attachInterrupt(SIT_CHANGE_BUTTON_PIN, sitChangeISR, HIGH);
}

void initTasks() {
    xTaskCreatePinnedToCore(oledHandlerTask_,
        "oledHandlerTask",
        10 * KB,
        NULL,
        OLED_HANDLER_PRIORITY, 
        &oledHandlerTask,
        DEAFAULT_CORE
    );
}

void initModules() {
    oledHandlerObj.beginOLEDSeat();
}

void init() {

    // initWiFi();

    initModules();

    initQueueSemaphores();

    Serial.println("init1");

    initInterrupts();

    Serial.println("init2");

    initTasks();

    Serial.println("init3");

    

    // xTaskCreatePinnedToCore()
}