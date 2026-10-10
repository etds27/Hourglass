#include <EEPROM.h>
#include "device_config.h"
#include "logger.h"
#include "colors.h"

namespace
{
    // Local logger tag for this module
    const LogString loggerTag = "DeviceConfigurator";

    DeviceConfig_v3 migrate2to3(const DeviceConfig_v2 &v2) {
        DeviceConfig_v3 v3;
        v3.version = 3;
        strncpy(v3.name, v2.name, MAX_NAME_LENGTH - 1);
        v3.name[MAX_NAME_LENGTH - 1] = '\0';
        v3.ledOffset = 0;
        v3.ledCount = 16;
        return v3;
    }

    DeviceConfig_v4 migrate3to4(const DeviceConfig_v3 &v3) {
        DeviceConfig_v4 v4;
        v4.version = 4;
        strncpy(v4.name, v3.name, MAX_NAME_LENGTH - 1);
        v4.name[MAX_NAME_LENGTH - 1] = '\0';
        v4.ledOffset = v3.ledOffset;
        v4.ledCount = v3.ledCount;
        v4.buttonPin = 255;
        v4.lightPin = 255;
        v4.motorPin = 255;
        return v4;
    }
}





// Write the main device configuration to EEPROM
void DeviceConfigurator::writeConfig(const DeviceConfig &config)
{
    // Ensure the version matches the expected version
    if (config.version != DEVICE_CONFIG_VERSION) {
        logger.warning(loggerTag, ": Invalid device config version");
        return;
    }

    writeConfig<DeviceConfig>(config);
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

void DeviceConfigurator::writeLEDOffset(uint8_t offset)
{
    DeviceConfig config = readConfig();
    config.ledOffset = offset;
    if (config.ledOffset != offset) {
        logger.info(loggerTag, ": LED offset unchanged");
        return;
    }
    logger.info(loggerTag, ": LED offset updated to: ", config.ledOffset);
    writeConfig(config);
}

void DeviceConfigurator::writeLEDCount(uint8_t count)
{
    if (count == 0) {
        logger.warning(loggerTag, ": LED count cannot be zero");
        return;
    }
    DeviceConfig config = readConfig();
    config.ledCount = count;
    if (config.ledCount != count) {
        logger.info(loggerTag, ": LED count unchanged");
        return;
    }
    logger.info(loggerTag, ": LED count updated to: ", config.ledCount);
    writeConfig(config);
}

void DeviceConfigurator::writeButtonPin(uint8_t pin)
{
    DeviceConfig config = readConfig();
    config.buttonPin = pin;
    if (config.buttonPin != pin) {
        logger.info(loggerTag, ": Button pin unchanged");
        return;
    }
    logger.info(loggerTag, ": Button pin updated to: ", config.buttonPin);
    writeConfig(config);
}

void DeviceConfigurator::writeLightPin(uint8_t pin)
{
    DeviceConfig config = readConfig();
    config.lightPin = pin;
    if (config.lightPin != pin) {
        logger.info(loggerTag, ": Light pin unchanged");
        return;
    }
    logger.info(loggerTag, ": Light pin updated to: ", config.lightPin);
    writeConfig(config);
}

void DeviceConfigurator::writeMotorPin(uint8_t pin)
{
    DeviceConfig config = readConfig();
    config.motorPin = pin;
    if (config.motorPin != pin) {
        logger.info(loggerTag, ": Motor pin unchanged");
        return;
    }
    logger.info(loggerTag, ": Motor pin updated to: ", config.motorPin);
    writeConfig(config);
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

uint8_t DeviceConfigurator::readConfigVersion()
{
    uint8_t version = 0;
    EEPROM.get(DEVICE_CONFIG_VERSION_LOCATION, version);
    return version;
}

uint8_t DeviceConfigurator::readVersion()
{
    return readConfigVersion();
}

void DeviceConfigurator::writeVersion(uint8_t version)
{
    EEPROM.put(DEVICE_CONFIG_VERSION_LOCATION, version);
    EEPROM.commit();
}

DeviceConfig DeviceConfigurator::migrateConfig(uint8_t storedVersion)
{
    if (storedVersion == DEVICE_CONFIG_VERSION) {
        logger.info(loggerTag, ": Stored config version matches current version (", DEVICE_CONFIG_VERSION, "), migration complete");
        return readConfig<DeviceConfig>();
    }

    if (storedVersion < 2 || storedVersion == 0xFF || storedVersion > DEVICE_CONFIG_VERSION) {
        logger.warning(loggerTag, ": Invalid stored config version (", storedVersion, "), resetting to default config");
        DeviceConfig defaultConfig = DeviceConfig{};
        defaultConfig.version = DEVICE_CONFIG_VERSION;
        strncpy(defaultConfig.name, "Default", MAX_NAME_LENGTH - 1);
        defaultConfig.name[MAX_NAME_LENGTH - 1] = '\0';
        writeConfig<DeviceConfig>(defaultConfig);
        return defaultConfig;
    }

    logger.info(loggerTag, ": Starting migration step for schema version ", storedVersion);

    if (storedVersion == 2) {
        DeviceConfig_v2 v2 = readConfig<DeviceConfig_v2>();
        logger.info(loggerTag, ": Migrating config 2 -> 3");
        DeviceConfig_v3 v3 = migrate2to3(v2);
        writeConfig<DeviceConfig_v3>(v3);
    } else if (storedVersion == 3) {
        DeviceConfig_v3 v3 = readConfig<DeviceConfig_v3>();
        logger.info(loggerTag, ": Migrating config 3 -> 4");
        DeviceConfig_v4 v4 = migrate3to4(v3);
        writeConfig<DeviceConfig_v4>(v4);
    }

    return migrateConfig(storedVersion + 1);
}

// Read the main device configuration from EEPROM
DeviceConfig DeviceConfigurator::readConfig()
{
    uint8_t storedVersion = readConfigVersion();

    // Handle invalid, uninitialized (0xFF / < 2), or downgraded EEPROM
    if (storedVersion < 2 || storedVersion == 0xFF || storedVersion > DEVICE_CONFIG_VERSION) {
        logger.warning(loggerTag, ": Invalid/uninitialized/downgraded device config version (", storedVersion, "), initializing defaults");
        DeviceConfig defaultConfig = DeviceConfig{};
        defaultConfig.version = DEVICE_CONFIG_VERSION;
        strncpy(defaultConfig.name, "Default", MAX_NAME_LENGTH - 1);
        defaultConfig.name[MAX_NAME_LENGTH - 1] = '\0';
        writeConfig<DeviceConfig>(defaultConfig);
        printConfig(defaultConfig);
        return defaultConfig;
    }

    // Stepwise migration if stored version is out of date
    if (storedVersion < DEVICE_CONFIG_VERSION) {
        DeviceConfig migratedConfig = migrateConfig(storedVersion);
        printConfig(migratedConfig);
        return migratedConfig;
    }

    // Current version
    DeviceConfig config = readConfig<DeviceConfig>();
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

        config = readDefaultColorConfig(index);

        // Save first default color config
        writeColorConfig(config, 0);
        return config;
    }

    if (header.configsWritten & (1u << index)) {
        logger.info(loggerTag, ": Reading existing color config at index ", index);
    } else {
        logger.info(loggerTag, ": No color config at index ", index, ", returning default colors for this index");
        config = readDefaultColorConfig(index);
        return config;
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

ColorConfig DeviceConfigurator::readDefaultColorConfig(uint16_t index)
{
    DeviceState::State state = static_cast<DeviceState::State>(index);
    switch (state)
    {
    case DeviceState::State::BuzzerAwaitingTurnEnd:
        return ColorConfig{.colors = {0xFFFF00FF, 0xFFFF00FF, 0xFFFF00FF, 0xFFFF00FF}};
    case DeviceState::State::BuzzerWinnerPeriod:
        return ColorConfig{.colors = {BUZZER_ACTIVE_TURN_COLOR_WINNER, BLUE, 0x0, 0x0}};
    case DeviceState::State::BuzzerResults:
        return ColorConfig{.colors = {BUZZER_ACTIVE_TURN_COLOR_LOSER, BLUE, 0x0, 0x0}};
    case DeviceState::State::AwaitingConnection: return ColorConfig{.colors = {HOURGLASS_BLUE, 0x0, 0x0, 0x0}};
    case DeviceState::State::Skipped: return ColorConfig{.colors = {WHITE, 0x0, 0x0, 0x0}};
    case DeviceState::State::DeviceColorMode: return ColorConfig{.colors = {WHITE, WHITE, HOURGLASS_BLUE, HOURGLASS_YELLOW}};

    default:
        return DEFAULT_COLOR_CONFIG;
    }
}

int8_t DeviceConfigurator::readLEDOffset()
{
    DeviceConfig config = readConfig();
    return config.ledOffset;
}

uint8_t DeviceConfigurator::readLEDCount()
{
    DeviceConfig config = readConfig();
    return config.ledCount;
}

uint8_t DeviceConfigurator::readButtonPin()
{
    DeviceConfig config = readConfig();
    return config.buttonPin;
}

uint8_t DeviceConfigurator::readLightPin()
{
    DeviceConfig config = readConfig();
    return config.lightPin;
}

uint8_t DeviceConfigurator::readMotorPin()
{
    DeviceConfig config = readConfig();
    return config.motorPin;
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
    logger.info(loggerTag, "LED Offset:   ", config.ledOffset);
    logger.info(loggerTag, "LED Count:    ", config.ledCount);
    logger.info(loggerTag, "Button Pin:   ", config.buttonPin);
    logger.info(loggerTag, "Light Pin:    ", config.lightPin);
    logger.info(loggerTag, "Motor Pin:    ", config.motorPin);
    logger.info(loggerTag, "=====================");
}

// Print a dynamic color config for debugging
void DeviceConfigurator::printColorConfig(const ColorConfig &cfg, uint16_t index)
{
    logger.info(loggerTag, "=== Color Config ", index, " ===");
    for (int i = 0; i < 4; ++i) {
        logger.info(loggerTag, "Color[", i, "]: ", ColorFormatter::toHex16(cfg.colors[i]).c_str());
    }
}
