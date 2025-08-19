#pragma once

#include <EEPROM.h>

#include "device_config.h"

void DeviceConfigurator::writeConfig(const DeviceConfig &config)
{
    if (config.version != DEVICE_CONFIG_VERSION) {
        Serial.println("Invalid device config version");
        return;
    }
    EEPROM.put(DEVICE_CONFIG_LOCATION, config);
    if (!EEPROM.commit()) {
        Serial.println("Failed to commit device config to EEPROM");
    } else {
        Serial.println("Device config written successfully");
    }
}

void DeviceConfigurator::writeName(const char *name)
{
    DeviceConfig config = readConfig();
    strncpy(config.name, name, MAX_NAME_LENGTH - 1);
    config.name[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination
    writeConfig(config);
    Serial.println("Device name updated");
}

void DeviceConfigurator::writeColor(uint32_t color)
{
    DeviceConfig config = readConfig();
    config.color = color;
    writeConfig(config);
    Serial.println("Device color updated");
}

void DeviceConfigurator::writeAccentColor(uint32_t accentColor)
{
    DeviceConfig config = readConfig();
    config.accentColor = accentColor;
    writeConfig(config);
    Serial.println("Device accent color updated");
}

DeviceConfig DeviceConfigurator::readConfig()
{
    DeviceConfig config;
    EEPROM.get(DEVICE_CONFIG_LOCATION, config);
    if (config.version != DEVICE_CONFIG_VERSION) {
        Serial.println("Invalid device config version");
        // Initialize with default values
        config.version = DEVICE_CONFIG_VERSION;
        strncpy(config.name, "Default", MAX_NAME_LENGTH);
        config.color = HOURGLASS_RED;
        config.accentColor = HOURGLASS_GREEN;
        Serial.println("Overwriting with default config");
        writeConfig(config); // Save the default config
    }
    return config;
}

void DeviceConfigurator::readName(char *name, size_t maxLength)
{
    if (maxLength > 0 && maxLength <= MAX_NAME_LENGTH) {
        strncpy(name, readConfig().name, maxLength - 1);
        name[maxLength - 1] = '\0'; // Ensure null-termination
    }
}

uint32_t DeviceConfigurator::readColor()
{
    return readConfig().color;
}

uint32_t DeviceConfigurator::readAccentColor()
{
    return readConfig().accentColor;
}
