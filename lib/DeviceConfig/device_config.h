#pragma once

#include <stdint.h>
#include <Arduino.h>
#include "constants.h"

#define DEVICE_CONFIG_VERSION 1
#define MAX_NAME_LENGTH 16
#define DEVICE_CONFIG_LOCATION 0x00

struct DeviceConfig {
    uint8_t version = DEVICE_CONFIG_VERSION;
    char name[MAX_NAME_LENGTH];
    uint32_t color;
    uint32_t accentColor;
};


namespace DeviceConfigurator {
    void writeConfig(const DeviceConfig& config);
    void writeName(const char* name);
    void writeColor(uint32_t color);
    void writeAccentColor(uint32_t accentColor);

    DeviceConfig readConfig();
    void readName(char* name, size_t maxLength);
    uint32_t readColor();
    uint32_t readAccentColor();

    void printConfig(const DeviceConfig& config);
}