#include "device_runtime.h"
#include "device_config.h"
#ifdef SIMULATOR
#include "simulator_tools.h"
#else
#include <Arduino.h>
#endif

#include "logger.h"

namespace
{
    const LogString loggerTag = "DeviceRuntime";
}

DeviceRuntime::DeviceRuntime(const char* name) {
    strncpy(deviceName, name, MAX_NAME_LENGTH - 1);
    deviceName[MAX_NAME_LENGTH - 1] = '\0';

    unsigned long now = millis();
    lastUpdate = now;
    lastReadOut = now;
    lastConnection = now;
    lastDisconnection = now;
    lastTurnStart = now;
}

void DeviceRuntime::setDeviceState(DeviceState::State newState)
{
    if (deviceState != newState) {
        logger.info(loggerTag, ": Device state changed from ", static_cast<int>(deviceState), " to ", static_cast<int>(newState));
    }
    deviceState = newState;
}

void DeviceRuntime::setConfigState(DeviceState::State newState)
{
    if (configState != newState) {
        logger.info(loggerTag, ": Config state changed from ", static_cast<int>(configState), " to ", static_cast<int>(newState));
    }
    configState = newState;
}
