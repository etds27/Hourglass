#ifdef SIMULATOR
#include "simulator_tools.h"
#include "gl_ring_interface.h"
#else
#include <Arduino.h>
#include <EEPROM.h>
#include "fast_led_light.h"
#include "ble_interface.h"
#include "button_input_interface.h"
#endif

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
#include "

unsigned long lastMemoryUpdate = millis();

namespace
{
  const LogString loggerTag = "Main";
}

// SevenSegmentDisplay* sevenSegment;
// DeviceManager* deviceManager;
// RingLight* m_ring;

#ifdef SIMULATOR
GLRingInterface *gRing;
#else
TFT_eSPI tft = TFT_eSPI();
FastLEDLight *fastLEDLight;
LCDRing *lRing;
LCDTimer *lTimer;
#endif
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

#ifndef SIMULATOR
  Serial.begin(115200);
  // while (!Serial)
  //   ;
  delay(1000);

  EEPROM.begin(0x0400);
#endif
  displayManager = new HourglassDisplayManager();

#ifdef SIMULATOR
  gRing = new GLRingInterface(16);
#else


  logger.info(loggerTag, ": Creating Input Interface");
  inputInterface = new ButtonInputInterface(BUTTON_INPUT_PIN);
  logger.info(loggerTag, ": Creating Central Interface");

  char deviceName[MAX_NAME_LENGTH];
  DeviceConfigurator::readName(deviceName, MAX_NAME_LENGTH);

  if (!isPrintableString(deviceName, MAX_NAME_LENGTH)) {
    logger.warning(loggerTag, ": Device name in EEPROM is invalid, resetting to default.");
    DeviceConfigurator::writeName("Hourglass");
    DeviceConfigurator::readName(deviceName, MAX_NAME_LENGTH);
  }

  // Initializing the BLE interface so that we can read the MAC address after
  interface = new BLEInterface(deviceName);


  fastLEDLight = new FastLEDLight(16, RING_DI_PIN);
  // lRing = new LCDRing(16, &tft);
  // lTimer = new LCDTimer(&tft);
  // displayManager->addDisplayInterface(fastLEDLight);
  displayManager->addDisplayInterface(lRing);
  // displayManager->addDisplayInterface(lTimer);

  // tft.init();
  // tft.setRotation(0); // Adjust rotation (0-3)

  // tft.fillScreen(TFT_BACKGROUND_COLOR);

#endif

  // DeviceConfigurator::writeName("ETHAN_TEST");
  deviceManager = new DeviceManager(displayManager, inputInterface, interface);
  deviceManager->start();
}

// unsigned long start = millis();

void loop()
{

  if (ENABLE_DEBUG)
  {
#ifdef SIMULATOR
    if (millis() - lastMemoryUpdate > 1000)
    {
      size_t freeMemory = esp_get_free_heap_size();
      logger.info("Free Memory: ", freeMemory);
      lastMemoryUpdate = millis();
    }
#endif
  }
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