#include <EEPROM.h>
#include "device_config.h"
#include "logger.h"

namespace
{
    // Local logger tag for this module
    const LogString loggerTag = "DeviceConfigurator";
}

// Write the main device configuration to EEPROM
void DeviceConfigurator::writeConfig(const DeviceConfig &config)
{
    // Ensure the version matches the expected version
    if (config.version != DEVICE_CONFIG_VERSION) {
        logger.info(loggerTag, ": Invalid device config version");
        return;
    }

    // Write the config struct to EEPROM at static location
    EEPROM.put(DEVICE_CONFIG_LOCATION, config);

    // Commit changes to EEPROM and log result
    if (!EEPROM.commit()) {
        logger.error(loggerTag, ": Failed to commit device config to EEPROM");
    } else {
        logger.info(loggerTag, ": Device config written successfully");
    }
}

// Update only the device name in EEPROM
void DeviceConfigurator::writeName(const char *name)
{
    DeviceConfig config = readConfig();

    // Only update if the new name differs
    if (strcmp(config.name, name) != 0) {
        strncpy(config.name, name, MAX_NAME_LENGTH - 1);
        config.name[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination
        writeConfig(config);
        logger.info(loggerTag, ": Device name updated to: ", config.name);
    } else {
        logger.info(loggerTag, ": Device name unchanged");
    }
}

// Write a dynamic color configuration to EEPROM
// If index is UINT16_MAX (default), append to end of stored configs
void DeviceConfigurator::writeColorConfig(const ColorConfig &colorConfig, uint16_t index)
{
    ColorConfigHeader header = readColorConfigHeader();

    // Index provided but out of bounds
    if (index >= DEVICE_MAX_COLOR_CONFIGS) {
        logger.warning(loggerTag, ": Index out of bounds for color config");
        return;
    }

    // Calculate EEPROM address for this color config
    uint32_t addr = DEVICE_COLOR_CONFIG_LOCATION + sizeof(ColorConfigHeader) + index * sizeof(ColorConfig);
    EEPROM.put(addr, colorConfig);

    header.configsWritten |= (1u << index); // Mark this index as written
    EEPROM.put(DEVICE_COLOR_CONFIG_LOCATION, header); // Update header in EEPROM

    logger.info(loggerTag, ": Color config written at index ", index, " at address ", addr);
    // Commit to EEPROM and log
    if (!EEPROM.commit()) {
        logger.error(loggerTag, ": Failed to commit color config at index ", index);
    } else {
        logger.info(loggerTag, ": Color config written successfully at index ", index);
    }
}

// Read the main device configuration from EEPROM
DeviceConfig DeviceConfigurator::readConfig()
{
    DeviceConfig config;
    EEPROM.get(DEVICE_CONFIG_LOCATION, config);

    // Handle invalid or uninitialized EEPROM
    if (config.version != DEVICE_CONFIG_VERSION) {
        logger.warning(loggerTag, ": Invalid device config version, initializing defaults");
        config.version = DEVICE_CONFIG_VERSION;
        strncpy(config.name, "Default", MAX_NAME_LENGTH);

        // Save default config back to EEPROM
        writeConfig(config);
    }

    printConfig(config);
    return config;
}

// Read only the device name from EEPROM
void DeviceConfigurator::readName(char *name, size_t maxLength)
{
    if (maxLength > 0 && maxLength <= MAX_NAME_LENGTH) {
        strncpy(name, readConfig().name, maxLength - 1);
        name[maxLength - 1] = '\0'; // Ensure null-termination
    }
}

// Read a specific color configuration by index
ColorConfig DeviceConfigurator::readColorConfig(uint16_t index)
{
    ColorConfig config;
    ColorConfigHeader header = readColorConfigHeader();

    // Handle invalid or uninitialized header
    if (header.version != DEVICE_COLOR_CONFIG_VERSION) {
        logger.info(loggerTag, ": Invalid color config version, initializing default colors");

        // Initialize header in EEPROM
        header.version = DEVICE_COLOR_CONFIG_VERSION;
        EEPROM.put(DEVICE_COLOR_CONFIG_LOCATION, header);
        EEPROM.commit();

        // Save first default color config
        writeColorConfig(DEFAULT_COLOR_CONFIG, 0);
        return config;
    }

    if (header.configsWritten & (1u << index)) {
        logger.info(loggerTag, ": Reading existing color config at index ", index);
    } else {
        logger.info(loggerTag, ": No color config at index ", index, ", returning default colors");
        return DEFAULT_COLOR_CONFIG;
    }

    // Check for invalid index
    if (index >= DEVICE_MAX_COLOR_CONFIGS) {
        logger.warning(loggerTag, ": Index out of bounds for color config");
        return ColorConfig{};
    }

    // Read color config from EEPROM
    uint32_t addr = DEVICE_COLOR_CONFIG_LOCATION + sizeof(ColorConfigHeader) + index * sizeof(ColorConfig);
    EEPROM.get(addr, config);
    return config;
}

// Read the header for dynamic color configs
ColorConfigHeader DeviceConfigurator::readColorConfigHeader()
{
    ColorConfigHeader header;
    EEPROM.get(DEVICE_COLOR_CONFIG_LOCATION, header);

    // Initialize header if EEPROM is empty or invalid
    if (header.version != DEVICE_COLOR_CONFIG_VERSION) {
        logger.info(loggerTag, ": Initializing color config header");
        header.version = DEVICE_COLOR_CONFIG_VERSION;
        header.configsWritten = 0;
        EEPROM.put(DEVICE_COLOR_CONFIG_LOCATION, header);
        EEPROM.commit();
    }

    logger.info(loggerTag, ": Color Config Version:    ", header.version);
    return header;
}

// Print main device configuration for debugging
void DeviceConfigurator::printConfig(const DeviceConfig &config)
{
    logger.info(loggerTag, "=== Device Config ===");
    logger.info(loggerTag, "Version:      ", config.version);
    logger.info(loggerTag, "Name:         ", config.name);
    logger.info(loggerTag, "=====================");
}

// Print a dynamic color config for debugging
void DeviceConfigurator::printColorConfig(const ColorConfig &cfg, uint16_t index)
{
    logger.info(loggerTag, "=== Color Config ", index, " ===");
    for (int i = 0; i < 4; ++i) {
        logger.info(loggerTag, "Color[", i, "]: ", cfg.colors[i]);
    }
}
