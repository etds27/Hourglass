#include <Arduino.h>
#include "easing_function.h"
#include "colors.h"
#include "constants.h"
#include "logger.h"
#include "device_manager.h"
#include <EEPROM.h>
#include "device_state.h"
#include "fast_led_light.h"
#include "light_interface.h"

FastLEDLight *fastLEDLight;
DeviceManager *deviceManager;
// ButtonInputMonitor* buttonInputMonitor;

void setup()
{
    loggerLevel = LoggerLevel::DEBUG;
    Serial.begin(115200);
    // while (!Serial)
    //   ;
    delay(2000);
    logger.info("Start of program");

    fastLEDLight = new FastLEDLight(16, RING_DI_PIN);
    uint32_t* buffer = new uint32_t[16]{};

    // Use for calibrating each light.
    // To move lights to left, increase ring offset
    // To move lights to right, decrease ring offset
    buffer[0] = BLUE;
    buffer[1] = GREEN;
    struct GameDebugData data
    {
        .buffer = buffer
    };

    fastLEDLight->updateTimerData(data);
    fastLEDLight->setDisplayMode(DeviceState::State::AwaitingConnection);
}

void loop()
{
    fastLEDLight->update();
}