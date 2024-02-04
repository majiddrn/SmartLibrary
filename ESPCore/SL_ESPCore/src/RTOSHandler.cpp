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

TableHandler tableHandler;

#define LAST_CHANGE_BUTTON_DEBOUNCE 250
unsigned long lastSitChangePressed = 0; // Debouncing of OLED related button

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          5          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);

// SeatStatus lastStatus = SEAT_NOT_USE;
//     bool change = false;
//     Seat change_to;
//     uint8_t sitNumberChanged;

typedef struct State {
    SeatStatus lastStatus;
    bool change;
    Seat change_to;
    uint8_t sitNumberChanged;
};

#define REST_TIME_OVERLOAD_THRESHOLD 300000

////////////////////////////// Prioriorities \\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

#define OLED_HANDLER_PRIORITY 2
#define RFID_HANDLER_PRIORITY 2
#define SEAT_SITTING_HANDLER_PRIORITY 2
#define SEAT_INDIVIDUALT_PRESENCE_HANDLER 3

////////////////////////////// Prioriorities \\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

//////////////////////////// Semaphore Queues \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

QueueHandle_t oledQueue = NULL;
QueueHandle_t seatSittingQueue = NULL;

SemaphoreHandle_t differLastChangeSemaphore = NULL;
SemaphoreHandle_t seatsSemaphore = NULL;
StaticSemaphore_t xSemaphoreBuffer;

//////////////////////////// Semaphore Queues \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

//////////////////////////////// Handlers \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

void stateHandler(State states[], uint8_t n) {
    for (uint8_t i = 0; i < n; i++) {
        
    }
}

//////////////////////////////// Handlers \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

////////////////////////////// RTOS Handlers \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

TaskHandle_t oledHandlerTask = NULL;
TaskHandle_t rfidHandlerTask = NULL;
TaskHandle_t seatSittingHandlerTask = NULL;
TaskHandle_t seatIndividualPresenceTask = NULL;

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

        uint8_t number = oledHandlerObj.getNumber();

        Serial.println("Number : " + (String) number);

        if (student.fname.indexOf("0") == 0) {
            oledHandlerObj.drawString("Student Not found", number);
            vTaskDelay(50 / portTICK_PERIOD_MS);
            continue;
        }

        String msg = student.fname + " " + student.lname;
        oledHandlerObj.drawString(msg, number);

        Serial.println("After drawing");


        Serial.println("NUMBER: " + (String)number);

        xSemaphoreTake(seatsSemaphore, portMAX_DELAY);

        bool inUse = tableHandler.sitInUse(number);
        
        xSemaphoreGive(seatsSemaphore);

        Serial.println("sit " + (String)number + " " + (String)inUse);

        if (!inUse) {    // sitting
            xSemaphoreTake(seatsSemaphore, portMAX_DELAY);
            tableHandler.sitOn(number, student.snum);
            xSemaphoreGive(seatsSemaphore);
            Serial.println("Sitting");
        }

        else {                                
            if (tableHandler.seats[number-1].studentNum.indexOf(student.snum) >= 0) { // leaving
            xSemaphoreTake(seatsSemaphore, portMAX_DELAY);
            tableHandler.emptySeat(number);
            xSemaphoreGive(seatsSemaphore);
            }
        }
        
        // xSemaphoreGive(seatsSemaphore);
        
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
    
}

void oledHandlerTask_(void * parameter) {
    // oledHandlerObj.setNumber(1);
    
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

        Seat seat {asciiBuffer, SEAT_IN_USE_REST};

        xQueueSend(seatSittingQueue, &seat, portMAX_DELAY);

        vTaskDelay(1000 / portTICK_PERIOD_MS);

         mfrc522.PICC_HaltA();
         mfrc522.PCD_StopCrypto1();

        vTaskDelay(10000 / portTICK_PERIOD_MS);

    }
    
}

void seatIndividualPresenceTask_(void* parameter) {
    static uint8_t cnt = 0;

    uint8_t seatNumber = tableHandler.getSeatNumbers();

    bool presence[seatNumber];

    bool timeOverloaded[seatNumber];
    unsigned long firstRestTime = 0;

    bool moving[seatNumber];
    bool first = true;

    // SeatStatus lastStatus = SEAT_NOT_USE;
    // bool change = false;
    // Seat change_to;
    // uint8_t sitNumberChanged;
    State state[seatNumber];
    state[0].change = false;
    state[1].change = false;

    // unsigned long last

    while (true)
    {
        Serial.println("PROCESS");
        xSemaphoreTake(seatsSemaphore, portMAX_DELAY);
        for (uint8_t i = 1; i <= seatNumber; i++) {
            
            presence[i-1] = tableHandler.getSeatPresence(i);
            
        }
        xSemaphoreGive(seatsSemaphore);
            
        cnt++;
        if (cnt >= 50) {
            xSemaphoreTake(seatsSemaphore, portMAX_DELAY);
            for (uint8_t i = 1; i <= seatNumber; i++){
                
                moving[i-1] = tableHandler.isMovedSinceLast(i);
                
            }
            xSemaphoreGive(seatsSemaphore);
            first = false;
            cnt = 0;
        }

        // Serial.println("Presence2: " + (String)tableHandler.getSeatPresence(2) + ", in use:" + (String)tableHandler.sitInUse(2) + " Move 2: " + (String)tableHandler.isMovedSinceLast(2));

        
        for (uint8_t i = 1; i <= seatNumber; i++) {
            uint8_t idx = i-1;

            xSemaphoreTake(seatsSemaphore, portMAX_DELAY);
            Seat currentSeat = tableHandler.seats[idx];
            xSemaphoreGive(seatsSemaphore);

            Serial.println("idx: " + (String)idx);

            xSemaphoreTake(seatsSemaphore, portMAX_DELAY);
            bool inuse = tableHandler.sitInUse(i);
            Serial.printf("Seat %d : %d\n", i, (int)inuse);
            xSemaphoreGive(seatsSemaphore);
            
            check_again:

            if (inuse) {

                if (!presence[idx]) {
                    xSemaphoreTake(seatsSemaphore, portMAX_DELAY);
                    tableHandler.seats[idx].status = SEAT_IN_USE_REST;
                    xSemaphoreGive(seatsSemaphore);

                    ParserSerial2Handler::setLED(i, LED_COLOR_YELLOW);
                    if (state[idx].lastStatus != SEAT_IN_USE_REST) {
                        firstRestTime = millis();

                        state[idx].change_to = currentSeat;

                        state[idx].change_to.status = SEAT_IN_USE_REST;

                        state[idx].lastStatus = SEAT_IN_USE_REST;
                        state[idx].sitNumberChanged = i;
                    } else {
                        
                        if (millis() - firstRestTime >= REST_TIME_OVERLOAD_THRESHOLD) {
                            state[idx].change = true;
                            state[idx].change_to = currentSeat;
                            currentSeat.status = SEAT_NOT_USE;
                            state[idx].lastStatus = SEAT_NOT_USE;
                            state[idx].sitNumberChanged = i;

                            xSemaphoreTake(seatsSemaphore, portMAX_DELAY);
                            tableHandler.seats[idx].status = SEAT_NOT_USE;
                            tableHandler.seats[idx].studentNum = "0";
                            xSemaphoreGive(seatsSemaphore);
                        } else 
                            state[idx].change = false;
                    }
        //             /*
                
        //             do server things
                
        //             */
                } else {
                    if (!moving[idx] && !first) {
                        xSemaphoreTake(seatsSemaphore, portMAX_DELAY);
                        tableHandler.seats[idx].status = SEAT_IN_USE_REST;
                        xSemaphoreGive(seatsSemaphore);
                        
                        ParserSerial2Handler::setLED(i, LED_COLOR_YELLOW);
                        if (state[idx].lastStatus != SEAT_IN_USE_REST) {
                            firstRestTime = millis();

                            state[idx].change_to = currentSeat;
                            state[idx].change_to.status = SEAT_IN_USE_REST;

                            state[idx].lastStatus = SEAT_IN_USE_REST;
                            state[idx].sitNumberChanged = i;
                        } else {
                            if (millis() - firstRestTime >= REST_TIME_OVERLOAD_THRESHOLD) {
                                state[idx].change = true;
                                state[idx].change_to = currentSeat;
                                currentSeat.status = SEAT_NOT_USE;
                                state[idx].lastStatus = SEAT_NOT_USE;
                                state[idx].sitNumberChanged = i;

                                xSemaphoreTake(seatsSemaphore, portMAX_DELAY);
                                tableHandler.seats[idx].status = SEAT_NOT_USE;
                                tableHandler.seats[idx].studentNum = "0";
                                xSemaphoreGive(seatsSemaphore);
                            } else 
                                state[idx].change = false;
                        }
                        /*
                
                        do server things
                
                        */
                    } else if (moving[idx]) {
                        xSemaphoreTake(seatsSemaphore, portMAX_DELAY);
                        tableHandler.seats[idx].status = SEAT_IN_USE;
                        xSemaphoreGive(seatsSemaphore);

                        ParserSerial2Handler::setLED(i, LED_COLOR_RED);
                        if (state[idx].lastStatus != SEAT_IN_USE) {
                            
                            state[idx].change_to = currentSeat;

                            state[idx].change_to.status = SEAT_IN_USE;

                            state[idx].lastStatus = SEAT_IN_USE;
                            state[idx].sitNumberChanged = i;
                        } else 
                            state[idx].change = false;
        //                 /*
                
        //                 do server things
                
        //                 */
                    }
                }

            } else {
                xSemaphoreTake(seatsSemaphore, portMAX_DELAY);
                tableHandler.seats[idx].status = SEAT_NOT_USE;
                xSemaphoreGive(seatsSemaphore);

                ParserSerial2Handler::setLED(i, LED_COLOR_GREEN);
                if (state[idx].lastStatus != SEAT_NOT_USE) {
                    state[idx].change_to = currentSeat;

                    state[idx].change_to.status = SEAT_NOT_USE;

                    state[idx].lastStatus = SEAT_NOT_USE;
                    state[idx].sitNumberChanged = i;
                } else 
                    state[idx].change = false;
        //         /*
                
        //         do server things
                
        //         */

            }
        }
        // xSemaphoreGive(seatsSemaphore);

        vTaskDelay(1500 / portTICK_PERIOD_MS);    
    }
    
}

////////////////////////////// Task Functions \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/

void initQueueSemaphores() {
    oledQueue = xQueueCreate(1, sizeof(OLEDMessage));
    seatSittingQueue = xQueueCreate(1, sizeof(Seat));

    seatsSemaphore = xSemaphoreCreateBinary();
    xSemaphoreGive(seatsSemaphore);

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
    xTaskCreatePinnedToCore(seatIndividualPresenceTask_, "seatInduvidualPresenceTask", 20 * KB, NULL, SEAT_INDIVIDUALT_PRESENCE_HANDLER, &seatIndividualPresenceTask, DEFAULT_CORE);
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