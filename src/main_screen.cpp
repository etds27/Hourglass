#include <Arduino.h>
#include <EEPROM.h>
#include "device_manager.h"
#include "lcd_ring.h"

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



TFT_eSPI *tft = new TFT_eSPI();
LCDRing *ring = new LCDRing(16, tft);
void setup() {
    Serial.begin(115200);
    tft->init();
    tft->setRotation(1); // Adjust rotation (0-3)

    tft->fillScreen(TFT_BLACK);

    ring->begin();
    ring->setDisplayMode(DeviceState::AwaitingConnecion);
}

void loop() {
    ring->update();
}