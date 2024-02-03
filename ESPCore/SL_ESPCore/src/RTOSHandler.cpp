#include "Handlers/RTOSHandler.h"
#include "Handlers/OLEDHandler.h"
#include "Handlers/WiFiHandler.h"
#include "Handlers/TableHandler.h"
#include "Students.h"

#define DEFAULT_CORE 1
#define KB 1024

#define SIT_CHANGE_BUTTON_PIN 14 

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

OLEDHandler oledHandlerObj(false);
WiFiHandler wifiHandlerObj;

#define LAST_CHANGE_BUTTON_DEBOUNCE 250
unsigned long lastSitChangePressed = 0;

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          5          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);

////////////////////////////// Prioriorities \\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

#define OLED_HANDLER_PRIORITY 2
#define RFID_HANDLER_PRIORITY 2
#define SEAT_SITTING_HANDLER_PRIORITY 2

////////////////////////////// Prioriorities \\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

//////////////////////////// Semaphore Queues \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

QueueHandle_t oledQueue = NULL;
QueueHandle_t seatSittingQueue = NULL;
SemaphoreHandle_t differLastChangeSemaphore = NULL;

//////////////////////////// Semaphore Queues \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

//////////////////////////////// Handlers \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

//////////////////////////////// Handlers \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

////////////////////////////// RTOS Handlers \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

TaskHandle_t oledHandlerTask = NULL;
TaskHandle_t rfidHandlerTask = NULL;
TaskHandle_t seatSittingHandlerTask = NULL;

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

void seatSittingHandlerTask_(void* parameter) {

    while (true) {
        Seat seat;

        xQueueReceive(seatSittingQueue, &seat, portMAX_DELAY);

        Student student = Students::isStudentValid(seat.studentNum);

        if (student.fname.indexOf("0") == 0) {
            oledHandlerObj.drawString("Student Not found");
            vTaskDelay(50 / portTICK_PERIOD_MS);
            continue;
        }

        String msg = student.fname + " " + student.lname;
        oledHandlerObj.drawString(msg);
        // Serial.printf("%s found\n", student.snum);
        
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
    
}

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

void rfidHandlerTask_(void* parameter) {
    MFRC522::MIFARE_Key key;

    for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

    byte block;
    byte len;
    MFRC522::StatusCode status;

    while (true)
    {

         mfrc522.PCD_StopCrypto1();
         vTaskDelay(50 / portTICK_PERIOD_MS);


         if ( ! mfrc522.PICC_IsNewCardPresent()) {
            Serial.print(F("Authentication failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));

            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
         }

        if (!mfrc522.PICC_ReadCardSerial()) {
            Serial.print(F("Reading failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));

            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue;
        }

        Serial.println(F("**Card Detected:**"));

        byte buffer[32];

        block = 16;
        len = 32;

        status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
        if (status != MFRC522::STATUS_OK) {
            vTaskDelay(250 / portTICK_PERIOD_MS);
            continue;
        }

        status = mfrc522.MIFARE_Read(block, buffer, &len);
        if (status != MFRC522::STATUS_OK) {
            vTaskDelay(250 / portTICK_PERIOD_MS);
            continue;
        }

        char asciiBuffer[11];

        for (uint8_t i = 0; i < 10; i++) {
            asciiBuffer[i] = (char) buffer[i];
        }

        Serial.println(asciiBuffer);

        Seat seat {asciiBuffer, SEAT_IN_USE_REST, 1};

        xQueueSend(seatSittingQueue, &seat, portMAX_DELAY);

        vTaskDelay(1000 / portTICK_PERIOD_MS);

         mfrc522.PICC_HaltA();
         mfrc522.PCD_StopCrypto1();

        vTaskDelay(10000 / portTICK_PERIOD_MS);

    }
    
}

////////////////////////////// Task Functions \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

void initQueueSemaphores() {
    oledQueue = xQueueCreate(1, sizeof(OLEDMessage));
    seatSittingQueue = xQueueCreate(1, sizeof(Seat));
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
    xTaskCreatePinnedToCore(oledHandlerTask_, "oledHandlerTask", 10 * KB, NULL, OLED_HANDLER_PRIORITY, &oledHandlerTask, DEFAULT_CORE);
    xTaskCreatePinnedToCore(rfidHandlerTask_, "rfidHandlerTask", 50 * KB, NULL, RFID_HANDLER_PRIORITY, &rfidHandlerTask, DEFAULT_CORE);
    xTaskCreatePinnedToCore(seatSittingHandlerTask_, "seatSittingHandlerTask", 20 * KB, NULL, SEAT_SITTING_HANDLER_PRIORITY, &seatSittingHandlerTask, DEFAULT_CORE);
}

void sampleStudents() {
    Student s1 {
        "Majid",
        "Dareini",
        "9822762040"
    };

    Student s2 {
        "Amir",
        "Hesami",
        "9812762241"
    };

    Students::addStudent(s1);
    Students::addStudent(s2);
}

void initModules() {

    {
        {
            sampleStudents();
        }
    }

    oledHandlerObj.beginOLEDSeat();
    SPI.begin();
    mfrc522.PCD_Init();
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