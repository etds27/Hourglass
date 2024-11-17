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
    fastLEDLight->setDisplayMode(DeviceState::ActiveTurn);
}

void loop()
{
    EasingFunction::EasingFunction *linearIn = new EasingFunction::Linear(EasingMode::EaseIn);
    EasingFunction::EasingFunction *linearOut = new EasingFunction::Linear(EasingMode::EaseOut);
    EasingFunction::EasingFunction *linearInOut = new EasingFunction::Linear(EasingMode::EaseInAndOut);

    /*
        logger.info(String(linearIn->ease(0.2)));
        logger.info(String(linearOut->ease(0.8)));
        logger.info(String(linearInOut->ease(0.8)));
        */

    EasingFunction::EasingFunction *quadIn = new EasingFunction::Circular(EasingMode::EaseIn);
    EasingFunction::EasingFunction *quadOut = new EasingFunction::Cubic(EasingMode::EaseOut);
    EasingFunction::EasingFunction *quadInOut = new EasingFunction::Cubic(EasingMode::EaseInAndOut);
    EasingFunction::EasingFunction *quadOutIn = new EasingFunction::Circular(EasingMode::EaseOutAndIn);

    int dataPoints = 20;
    double interval = 1.0 / dataPoints;
    for (int i = 0; i <= dataPoints; i++)
    {
        double input = i * interval;

        double val = quadOutIn->ease(input);
        uint8_t adjustedVal = round(val * 100);
        logger.info(String(adjustedVal));
    }
    logger.info("");
    delay(1000000);

    // fastLEDLight->update();
}