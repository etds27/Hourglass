#pragma once

#include <stdint.h>
#include <Arduino.h>
#include <EEPROM.h>
#include "constants.h"
#include "device_state.h"

#define DEVICE_CONFIG_VERSION 4
#define MAX_NAME_LENGTH 16
#define DEVICE_CONFIG_LOCATION 0x0000
#define DEVICE_CONFIG_VERSION_LOCATION DEVICE_CONFIG_LOCATION


#define DEVICE_COLOR_CONFIG_LOCATION 0x00FF
#define DEVICE_COLOR_CONFIG_VERSION_LOCATION DEVICE_COLOR_CONFIG_LOCATION
#define DEVICE_COLOR_CONFIG_VERSION 1
#define DEVICE_MAX_COLOR_CONFIGS DeviceState::NUMBER_OF_STATES



struct DeviceConfig_v2 {
    uint8_t version = 2;
    char name[MAX_NAME_LENGTH];
};

struct DeviceConfig_v3 {
    uint8_t version = 3;
    char name[MAX_NAME_LENGTH];
    uint8_t ledOffset = 0;
    uint8_t ledCount = 16;
};

struct DeviceConfig {
    uint8_t version = DEVICE_CONFIG_VERSION;
    char name[MAX_NAME_LENGTH];
    uint8_t ledOffset = 0;
    uint8_t ledCount = 16;
    uint8_t buttonPin = 255;
    uint8_t lightPin = 255;
    uint8_t motorPin = 255;
};

using DeviceConfig_v4 = DeviceConfig;


struct ColorConfigHeader {
    uint8_t version = DEVICE_COLOR_CONFIG_VERSION;
    uint32_t configsWritten = 0;
};

struct ColorConfig {
    uint32_t colors[4]; // four 32-bit colors
};


namespace DeviceConfigurator {
    // Generic templated read and write for any version schema struct
    template <typename T>
    T readConfig() {
        T config{};
        EEPROM.get(DEVICE_CONFIG_LOCATION, config);
        return config;
    }

    template <typename T>
    void writeConfig(const T& config) {
        EEPROM.put(DEVICE_CONFIG_LOCATION, config);
        EEPROM.commit();
    }

    void writeConfig(const DeviceConfig& config);
    void writeVersion(uint8_t version);
    void writeName(const char* name);
    void writeLEDOffset(uint8_t offset);
    void writeLEDCount(uint8_t count);
    void writeButtonPin(uint8_t pin);
    void writeLightPin(uint8_t pin);
    void writeMotorPin(uint8_t pin);

    void writeColorConfig(const ColorConfig& colorConfig, uint16_t index = -1);

    uint8_t readConfigVersion();
    DeviceConfig migrateConfig(uint8_t storedVersion);
    DeviceConfig readConfig();


    void readName(char* name, size_t maxLength);
    ColorConfig readColorConfig(uint16_t index);
    ColorConfig readDefaultColorConfig(uint16_t index);
    uint8_t readVersion();
    int8_t readLEDOffset();
    uint8_t readLEDCount();
    uint8_t readButtonPin();
    uint8_t readLightPin();
    uint8_t readMotorPin();

    void printConfig(const DeviceConfig& config);
    void printColorConfig(const ColorConfig &cfg, uint16_t index = -1);

    ColorConfigHeader readColorConfigHeader();

    const ColorConfig DEFAULT_COLOR_CONFIG = {
        .colors = {HOURGLASS_RED, HOURGLASS_GREEN, HOURGLASS_BLUE, HOURGLASS_YELLOW}
    };
}
