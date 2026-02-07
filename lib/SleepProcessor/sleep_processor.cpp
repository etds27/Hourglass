#include "sleep_processor.h"

#include "logger.h"
#include "device_context.h"
#include "device_runtime.h"

#ifdef SIMULATOR
#include "simulator_tools.h"
#else
#include <Arduino.h>
#endif

namespace
{
    const LogString loggerTag = "SleepProcessor";
}

SleepProcessor::SleepProcessor()
{
}

void SleepProcessor::start(DeviceContext *context, DeviceRuntime *runtime)
{
    // Allows the main device button to wake the device from sleep state
    esp_sleep_enable_ext0_wakeup(BUTTON_GPIO_PIN, HIGH);
}

bool SleepProcessor::update(DeviceContext *context, DeviceRuntime *runtime)
{

    if (runtime->deviceState == DeviceState::State::AwaitingConnection &&
        runtime->lastUpdate - runtime->lastConnection > CONNECTION_TIMEOUT)
    {
        logger.info(loggerTag, ": Entering Deep Sleep");
        runtime->deviceState = DeviceState::State::Off;
        context->displayManager->setDisplayMode(runtime->deviceState);
        context->displayManager->update(true);

#ifndef SIMULATOR
        delay(1000);
        esp_deep_sleep_start();
#endif
    }

    return true;
}