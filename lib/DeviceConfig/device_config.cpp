#include <EEPROM.h>

#include "device_config.h"
#include "logger.h"

void DeviceConfigurator::writeConfig(const DeviceConfig &config)
{
    if (config.version != DEVICE_CONFIG_VERSION) {
        logger.info("Invalid device config version");
        return;
    }
    EEPROM.put(DEVICE_CONFIG_LOCATION, config);
    if (!EEPROM.commit()) {
        logger.info("Failed to commit device config to EEPROM");
    } else {
        logger.info("Device config written successfully");
    }
}

void DeviceConfigurator::writeName(const char *name)
{
    DeviceConfig config = readConfig();

    if (strcmp(config.name, name) != 0) {
        strncpy(config.name, name, MAX_NAME_LENGTH - 1);
        config.name[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination
        writeConfig(config);
        logger.info("Device name updated to: ", config.name);
    } else {
        logger.info("Device name unchanged");
    }
}

void DeviceConfigurator::writeColor(uint32_t color)
{
    DeviceConfig config = readConfig();
    config.color = color;
    writeConfig(config);
    logger.info("Device color updated to ", color);
}

void DeviceConfigurator::writeAccentColor(uint32_t accentColor)
{
    DeviceConfig config = readConfig();
    config.accentColor = accentColor;
    writeConfig(config);
    logger.info("Device accent color updated to ", accentColor);
}

DeviceConfig DeviceConfigurator::readConfig()
{
    DeviceConfig config;
    EEPROM.get(DEVICE_CONFIG_LOCATION, config);
    if (config.version != DEVICE_CONFIG_VERSION) {
        logger.info("Invalid device config version");
        // Initialize with default values
        config.version = DEVICE_CONFIG_VERSION;
        strncpy(config.name, "Default", MAX_NAME_LENGTH);
        config.color = HOURGLASS_RED;
        config.accentColor = HOURGLASS_GREEN;
        logger.info("Overwriting with default config");
        writeConfig(config); // Save the default config
    }
    printConfig(config);
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

void DeviceConfigurator::printConfig(const DeviceConfig &config)
{
    logger.info("=== Device Config ===");
    logger.info("Version:      ", config.version);
    logger.info("Name:         ", config.name);
    logger.info("Color:        ", config.color);
    logger.info("Accent Color: ", config.accentColor);
    logger.info("=====================");
}