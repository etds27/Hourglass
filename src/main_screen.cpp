#include <Arduino.h>
#include <EEPROM.h>
#include "device_manager.h"
#include "lcd_ring.h"
#include "fast_led_light.h"
#include "lcd_timer.h"
#include "hg_display_interface.h"

#define ILI9341_DRIVER

// #define TFT_MISO  PIN_D6  // Automatically assigned with ESP8266 if not defined
/*
#define TFT_MOSI  27  // Automatically assigned with ESP8266 if not defined
#define TFT_SCLK  26  // Automatically assigned with ESP8266 if not defined

#define TFT_CS    12  // Chip select control pin D8
#define TFT_DC    14  // Data Command control pin
#define TFT_RST   13  // Reset pin (could connect to NodeMCU RST, see next line)
*/
#define TFT_SDA_READ

#define SPI_FREQUENCY  27000000
// #define SPI_FREQUENCY  40000000
// #define SPI_FREQUENCY  55000000 // STM32 SPI1 only (SPI2 maximum is 27MHz)
// #define SPI_FREQUENCY  80000000

// Optional reduced SPI frequency for reading TFT
#define SPI_READ_FREQUENCY  20000000

// The XPT2046 requires a lower SPI clock rate of 2.5MHz so we define that here:
#define SPI_TOUCH_FREQUENCY  2500000


TFT_eSPI tft = TFT_eSPI();
LCDRing *lRing = new LCDRing(16, &tft);
// LCDTimer *lTimer = new LCDTimer(&tft);
FastLEDLight *fRing = new FastLEDLight(16, 12);
TimerData *data = new TimerData;

DeviceManager *deviceManager;
HourglassDisplayManager *displayManager;
void setup() {

    Serial.begin(115200);
 
    data->totalTime = 70000;
    data->elapsedTime = 0;
    data->isTurnTimeEnforced = true;

    displayManager = new HourglassDisplayManager();
    displayManager->addDisplayInterface(lRing);
    //displayManager->addDisplayInterface(fRing);
    // displayManager->addDisplayInterface(lTimer);

    deviceManager = new DeviceManager(displayManager);
    tft.init();
    tft.setRotation(0); // Adjust rotation (0-3)

    tft.fillScreen(TFT_BLACK);

    displayManager->updateTimerData(*data);
    displayManager->setDisplayMode(DeviceState::State::BuzzerAwaitingTurnStart);
}

void loop() {
    uint32_t elapsedTime = millis() % 70000;
    data->elapsedTime = elapsedTime;
    displayManager->updateTimerData(*data);
    displayManager->update();
}