#include <Arduino.h>
#include <EEPROM.h>
#include "fast_led_light.h"
#include "ble_interface.h"
#include "button_input_interface.h"

#include "colors.h"
#include "constants.h"
#include "logger.h"
#include "device_manager.h"
#include "device_state.h"
#include "fast_led_light.h"
#include "hg_display_manager.h"
#include "lcd_ring.h"
#include "lcd_timer.h"
#include "device_config.h"
#include <TFT_eSPI.h>

unsigned long lastMemoryUpdate = millis();

namespace
{
  const LogString loggerTag = "Main";
}

TFT_eSPI tft = TFT_eSPI();
FastLEDLight *fastLEDLight;
LCDRing *lRing;
LCDTimer *lTimer;
DeviceManager *deviceManager;
HourglassDisplayManager *displayManager;
InputInterface *inputInterface;
HGCentralInterface *interface;


bool isPrintableString(const char *buf, size_t len) {
  bool allPrintable = true;
  for (size_t i = 0; i < len; i++) {
    uint8_t c = buf[i];
    if (c == 0xFF) return false;  // uninitialized EEPROM
    if (c == 0x00) break;         // reached end of string
    if (c < 0x20 || c > 0x7E) return false; // invalid char
    allPrintable = true;
  }
  return allPrintable;
}

void setup()
{
  switch (LOGGER_LEVEL)
  {
  case 0:
    loggerLevel = Logging::LoggerLevel::DEBUG;
    break;
  case 1:
    loggerLevel = Logging::LoggerLevel::INFO;
    break;
  case 2:
    loggerLevel = Logging::LoggerLevel::WARNING;
    break;
  case 3:
    loggerLevel = Logging::LoggerLevel::FAILURE;
    break;
  case 4:
    loggerLevel = Logging::LoggerLevel::OFF;
    break;
  default:
    loggerLevel = Logging::LoggerLevel::OFF;
    break;
  }

  Serial.begin(115200);
  delay(1000);

  EEPROM.begin(0x0400);
  char deviceName[MAX_NAME_LENGTH] = {};
  DeviceConfigurator::readName(deviceName, MAX_NAME_LENGTH);
  // DeviceConfigurator::writeLEDCount(16);
  // DeviceConfigurator::writeLEDOffset(0);

  if (!isPrintableString(deviceName, MAX_NAME_LENGTH)) {
    logger.warning(loggerTag, ": Device name in EEPROM is invalid, resetting to default.");
    DeviceConfigurator::writeName("Hourglass");
    DeviceConfigurator::readName(deviceName, MAX_NAME_LENGTH);
  }

  displayManager = new HourglassDisplayManager();
  inputInterface = new ButtonInputInterface(BUTTON_INPUT_PIN);
  interface = new BLEInterface(deviceName);


  fastLEDLight = new FastLEDLight(16, RING_DI_PIN);
  displayManager->addDisplayInterface(fastLEDLight);

  // lRing = new LCDRing(16, &tft);
  // displayManager->addDisplayInterface(lRing);
  
  // lTimer = new LCDTimer(&tft);
  // displayManager->addDisplayInterface(lTimer);

  // tft.init();
  // tft.setRotation(0); // Adjust rotation (0-3)

  // tft.fillScreen(TFT_BACKGROUND_COLOR);

  // DeviceConfigurator::writeName("ETHAN_TEST");
  deviceManager = new DeviceManager(displayManager, inputInterface, interface);
  deviceManager->start();
}

// unsigned long start = millis();

void loop()
{
  deviceManager->update();
}

int main()
{
  setup();
  while (true)
  {
    loop();
  }

  return 0;
}