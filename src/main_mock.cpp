#ifdef SIMULATOR
#include "simulator_tools.h"
#include "gl_ring_interface.h"
#else
#include <Arduino.h>
#include <EEPROM.h>
#include "mock_central_interface.h"
#include "mock_input_interface.h"
#endif

#include "colors.h"
#include "constants.h"
#include "logger.h"
#include "device_manager.h"
#include "device_state.h"
#include "hg_display_manager.h"
#include "device_config.h"

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
#endif
DeviceManager *deviceManager;
HourglassDisplayManager *displayManager;
InputInterface *inputInterface;
MockCentralInterface *interface;
unsigned long startTime = millis();



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

  EEPROM.begin(sizeof(DeviceConfig));
#endif
  displayManager = new HourglassDisplayManager();

#ifdef SIMULATOR
  gRing = new GLRingInterface(16);
#else

  char deviceName[MAX_NAME_LENGTH];
  DeviceConfigurator::readName(deviceName, MAX_NAME_LENGTH);

  logger.info(loggerTag, ": Creating Input Interface");
  inputInterface = new MockInputInterface();
  logger.info(loggerTag, ": Creating Central Interface");
  interface = new MockCentralInterface();


  // fastLEDLight = new FastLEDLight(16, RING_DI_PIN);
  // displayManager->addDisplayInterface(lRing);
  // displayManager->addDisplayInterface(lTimer);

#endif

  DeviceConfigurator::writeName("ETHAN_TEST");
  deviceManager = new DeviceManager(displayManager, inputInterface, interface);
  deviceManager->start();
  logger.info(loggerTag, ": Device Manager started");
}

// unsigned long start = millis();

void loop()
{

  if (millis() - startTime > 1000)
  {

    // Test changing device name
    // logger.info("Triggering device name change from central");
    // interface->triggerDeviceNameChange("Ethan 2");
    // interface->triggerDeviceNameWrite(true);
    // interface->triggerDeviceNameWrite(false);

    // Test changing color config
    // logger.info(loggerTag, ": Triggering color config change from central");
    // ColorConfig newConfig = {{
    //     0xFF0000, // Red
    //     0x00FF00, // Green
    //     0x0000FF, // Blue
    //     0xFFFF00  // Yellow
    // }};
    // interface->triggerDeviceColorConfigChange(newConfig);
    // interface->triggerDeviceColorConfigStateChange(DeviceState::State::AwaitingGameStart);
    // interface->triggerDeviceColorConfigWrite(true);
    // interface->triggerDeviceColorConfigWrite(false);


    while (millis() - startTime < 2000)
      ;
    // exit(0);
  }

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
  startTime = millis();
  while (true)
  {
    loop();
  }

  return 0;
}