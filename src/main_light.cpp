#ifdef SIMULATOR
#include "simulator_tools.h"
#include "gl_ring_interface.h"
#else
#include <Arduino.h>
#include <EEPROM.h>
#include "fast_led_light.h"
#endif

#include "easing_function.h"
#include "colors.h"
#include "constants.h"
#include "logger.h"
#include "device_manager.h"
#include "device_state.h"
#include "light_interface.h"


#ifdef SIMULATOR
LightInterface *fastLEDLight;
#else
FastLEDLight *fastLEDLight;
#endif
DeviceManager *deviceManager;
// ButtonInputMonitor* buttonInputMonitor;

void setup()
{
    loggerLevel = Logging::LoggerLevel::DEBUG;
    #ifndef SIMULATOR
    Serial.begin(115200);
    // while (!Serial)
    //   ;
    delay(2000);
    logger.info("Start of program");
    fastLEDLight = new FastLEDLight(16, RING_DI_PIN);
    #endif

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

    fastLEDLight->updateGameDebugData(data);
    fastLEDLight->setDisplayMode(DeviceState::State::Debug);
}

void loop()
{
    fastLEDLight->update();
}