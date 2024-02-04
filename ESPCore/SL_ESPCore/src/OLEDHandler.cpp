#include "Handlers/OLEDHandler.h"

static SSD1306Wire display(0x3c, SDA, SCL); 

OLEDHandler::OLEDHandler(bool isBegin)
{
    if (isBegin)
        beginOLEDSeat();
}

void OLEDHandler::drawScreen(uint8_t num, bool clear) {

    if (clear)
        display.clear();

    display.setFont(ArialMT_Plain_10);
    display.drawString(35, 10, "Sit number");
    display.display();
    // vTaskDelay(2000);

    // display.setTextSize(3);             
    // display.setTextColor(WHITE);   

    // display.setCursor(0, 20);
    display.setFont(ArialMT_Plain_24);
    display.drawString(0, 20, "---------------------");

    // display.setCursor(54, 40);
    // display.print("1");
    display.drawString(54, 40, String(num));

    display.display();
}

void OLEDHandler::beginOLEDSeat() {

    display.init();

    display.clear();

    display.flipScreenVertically();
    
    // display.setCursor(35, 10);
    // display.setTextColor(WHITE);
    drawScreen(1);

    // vTaskDelay(2000);
}

void OLEDHandler::setNumber(uint8_t num) {
    number = num;

    // display.setCursor(54, 40);
    // display.print(" ");
    // display.drawString(48, 40, "     ");

    // display.display();
    drawScreen(num, true);
    // vTaskDelay(100);

    // display.setCursor(54, 40);
    // display.print(String(num));
    // display.drawString(54, 40, String(num));

    // display.display();
}

uint8_t OLEDHandler::increaseNumber() {
    number = (number + 1) % maxNumber;

    // display.setCursor(54, 40);
    // display.print("   ");
    // display.setFont(ArialMT_Plain_24);
    // display.drawString(48, 40, "------");

    // display.display();

    // vTaskDelay(100);
    drawScreen(number + 1, true);
    Serial.println("NUMBER INCREASED -> " + (String)number);
    // display.setCursor(54, 40);
    // display.print(String(number));
    // display.drawString(50, 40, String(number));

    // display.display();

    // Serial.println("Must have been changed");

    return number + 1;
}

uint8_t OLEDHandler::getNumber() {
    Serial.println("GOT NUMBER " + (String) number);
    return number + 1;
}

void OLEDHandler::drawString(String str, uint8_t number) {
    display.clear();

    display.setFont(ArialMT_Plain_16);
    display.drawString(2, 20, str);
    display.display();

    vTaskDelay(5000);

    drawScreen(number, true);

}

OLEDHandler::~OLEDHandler()
{
}