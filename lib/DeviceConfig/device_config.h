#pragma once

#include <stdint.h>
#include <Arduino.h>
#include "constants.h"
#include "device_state.h"

#define DEVICE_CONFIG_VERSION 2
#define MAX_NAME_LENGTH 16
#define DEVICE_CONFIG_LOCATION 0x0000

#define DEVICE_COLOR_CONFIG_LOCATION 0x1000
#define DEVICE_COLOR_CONFIG_VERSION 1
#define DEVICE_MAX_COLOR_CONFIGS DeviceState::NUMBER_OF_STATES

struct DeviceConfig {
    uint8_t version = DEVICE_CONFIG_VERSION;
    char name[MAX_NAME_LENGTH];
};

struct ColorConfigHeader {
    uint8_t version = DEVICE_COLOR_CONFIG_VERSION;
    uint32_t configsWritten = 0;
};

struct ColorConfig {
    uint32_t colors[4]; // four 32-bit colors
};


namespace DeviceConfigurator {
    void writeConfig(const DeviceConfig& config);
    void writeName(const char* name);

    void writeColorConfig(const ColorConfig& colorConfig, uint16_t index = -1);

    DeviceConfig readConfig();
    void readName(char* name, size_t maxLength);
    ColorConfig readColorConfig(uint16_t index);

    void printConfig(const DeviceConfig& config);
    void printColorConfig(const ColorConfig &cfg, uint16_t index);

    ColorConfigHeader readColorConfigHeader();


}