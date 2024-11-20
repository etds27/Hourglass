#include <Arduino.h>
#include "easing_function.h"
#include "colors.h"
#include "constants.h"
#include "logger.h"
#include "device_manager.h"
#include <EEPROM.h>
#include "device_state.h"
#include "fast_led_light.h"

FastLEDLight *fastLEDLight;
DeviceManager *deviceManager;
// ButtonInputMonitor* buttonInputMonitor;

void setup()
{
    loggerLevel = LoggerLevel::DEBUG;
    Serial.begin(9600);
    // while (!Serial)
    //   ;
    delay(2000);
    logger.info("Start of program");

    fastLEDLight = new FastLEDLight(16, RING_DI_PIN);
    struct TimerData data
    {
        .totalTime = 1, .elapsedTime = 1, .isTurnTimeEnforced = false
    };

    fastLEDLight->updateTimerData(data);
    fastLEDLight->setDisplayMode(DeviceState::Paused);
}

void loop()
{
    fastLEDLight->update();
}