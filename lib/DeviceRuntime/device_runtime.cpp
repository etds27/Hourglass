#include "device_runtime.h"
#include "device_config.h"
#ifdef SIMULATOR
#include "simulator_tools.h"
#else
#include <Arduino.h>
#endif

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