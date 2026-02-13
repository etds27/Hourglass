#include <Arduino.h>
#include "fast_led_light.h"
#include <EEPROM.h>
#include "ble_interface.h"
#include "button_input_interface.h"

#include "colors.h"
#include "constants.h"
#include "logger.h"
#include "device_manager.h"
#include "device_state.h"
#include "fast_led_light.h"
#include "hg_display_manager.h"
#include "device_config.h"
#include "device_context.h"
#include "device_runtime.h"

#include "sleep_processor.h"
#include "runtime_processor.h"
#include "display_processor.h"
#include "hg_central_processor.h"
#include "input_processor.h"

unsigned long lastMemoryUpdate = millis();

namespace
{
  const LogString loggerTag = "Main";
}

FastLEDLight *fastLEDLight;
DeviceManager *deviceManager;
HourglassDisplayManager *displayManager;
InputInterface *inputInterface;
HGCentralInterface *interface;

DeviceRuntime *createRuntime(char *deviceName)
{
  DeviceRuntime *runtime = new DeviceRuntime(deviceName);
  return runtime;
}

DeviceContext *createContext(char *deviceName)
{
  HourglassDisplayManager *displayManager = new HourglassDisplayManager();
  FastLEDLight *fastLEDLight = new FastLEDLight(16, RING_DI_PIN);
  displayManager->addDisplayInterface(fastLEDLight);

  logger.info(loggerTag, "Creating central interface");
  HGCentralInterface *centralInterface = new BLEInterface(deviceName);
  logger.info(loggerTag, "Creating button input interface");
  ButtonInputInterface *buttonInputInterface = new ButtonInputInterface(BUTTON_INPUT_PIN);
  logger.info(loggerTag, "Creating button input monitor");
  ButtonInputMonitor *buttonInputMonitor = new ButtonInputMonitor(buttonInputInterface);
  logger.info(loggerTag, "Creating sleep processor");
  SleepProcessor *sleepProcessor = new SleepProcessor();
  logger.info(loggerTag, "Creating display processor");
  DisplayProcessor *displayProcessor = new DisplayProcessor();
  logger.info(loggerTag, "Creating central processor");
  CentralProcessor *centralProcessor = new CentralProcessor();
  logger.info(loggerTag, "Creating runtime processor");
  RuntimeProcessor *runtimeProcessor = new RuntimeProcessor();
  logger.info(loggerTag, "Creating input processor");
  InputProcessor *inputProcessor = new InputProcessor();

  DeviceContext *context = new DeviceContext{
      .centralInterface = centralInterface,
      .buttonInputMonitor = buttonInputMonitor,
      .displayManager = displayManager,
      .sleepProcessor = sleepProcessor,
      .displayProcessor = displayProcessor,
      .centralProcessor = centralProcessor,
      .runtimeProcessor = runtimeProcessor,
      .inputProcessor = inputProcessor};
  return context;
}

bool isPrintableString(const char *buf, size_t len)
{
  bool allPrintable = true;
  for (size_t i = 0; i < len; i++)
  {
    uint8_t c = buf[i];
    if (c == 0xFF)
      return false; // uninitialized EEPROM
    if (c == 0x00)
      break; // reached end of string
    if (c < 0x20 || c > 0x7E)
      return false; // invalid char
    allPrintable = true;
  }
  return allPrintable;
}

void setup()
{
  // The production board writes the motor pin high by default on boot. This resets it so the motor isnt always running until first notification
  pinMode(MOTOR_PIN, OUTPUT);
  analogWrite(MOTOR_PIN, 0);

#ifdef PROD_RELEASE
  loggerLevel = Logging::LoggerLevel::Off;
#else
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
#endif

  Serial.begin(115200);
  delay(1000);

  EEPROM.begin(0x0400);
  char deviceName[MAX_NAME_LENGTH] = {};
  DeviceConfigurator::readName(deviceName, MAX_NAME_LENGTH);
  // DeviceConfigurator::writeLEDCount(16);
  // DeviceConfigurator::writeLEDOffset(0);

  if (!isPrintableString(deviceName, MAX_NAME_LENGTH))
  {
    logger.warning(loggerTag, ": Device name in EEPROM is invalid, resetting to default.");
    DeviceConfigurator::writeName("Hourglass");
    DeviceConfigurator::readName(deviceName, MAX_NAME_LENGTH);
  }

  // displayManager = new HourglassDisplayManager();
  // inputInterface = new ButtonInputInterface(BUTTON_INPUT_PIN);
  // interface = new BLEInterface(deviceName);

  // fastLEDLight = new FastLEDLight(16, RING_DI_PIN);
  // displayManager->addDisplayInterface(fastLEDLight);

  // lRing = new LCDRing(16, &tft);
  // displayManager->addDisplayInterface(lRing);

  // lTimer = new LCDTimer(&tft);
  // displayManager->addDisplayInterface(lTimer);

  // tft.init();
  // tft.setRotation(0); // Adjust rotation (0-3)

  // tft.fillScreen(TFT_BACKGROUND_COLOR);

  // DeviceConfigurator::writeName("ETHAN_TEST");




  DeviceContext *context = createContext(deviceName);
  DeviceRuntime *runtime = createRuntime(deviceName);

  deviceManager = new DeviceManager(context, runtime);
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