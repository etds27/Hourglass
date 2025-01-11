#ifdef SIMULATOR
#include "simulator_tools.h"
#include "gl_ring_interface.h"
#else
#include <Arduino.h>
#include <EEPROM.h>
#include "fast_led_light.h"
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
#include <TFT_eSPI.h>


unsigned long lastMemoryUpdate = millis();

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
  delay(2000);
  logger.info("Start of program");
  // Start the BLE peripheral

  EEPROM.begin(8);
  displayManager = new HourglassDisplayManager();

  #ifdef SIMULATOR
  gRing = new GLRingInterface(16);
  #else
  // fastLEDLight = new FastLEDLight(16, RING_DI_PIN);
  lRing = new LCDRing(16, &tft);
  lTimer = new LCDTimer(&tft);
  // displayManager->addDisplayInterface(fastLEDLight);
  displayManager->addDisplayInterface(lRing);
  displayManager->addDisplayInterface(lTimer);

  tft.init();
  tft.setRotation(0); // Adjust rotation (0-3)

  tft.fillScreen(TFT_BACKGROUND_COLOR);

  #endif

  deviceManager = new DeviceManager(displayManager);
  // deviceManager->writeDeviceName("HG7     ", 8);
  // logger.info(String(deviceManager->getDeviceName()));
  deviceManager->start();

}

// unsigned long start = millis();

void loop()
{

  if (ENABLE_DEBUG) {
    #ifdef SIMULATOR
    if (millis() - lastMemoryUpdate > 1000) {
      size_t freeMemory = esp_get_free_heap_size();
      logger.info("Free Memory: " + freeMemory);
      lastMemoryUpdate = millis();
    }
    #endif
  }
  deviceManager->update();
}

int main() {
  setup();
  while (true) {
    loop();
  }

  return 0;
}