#include <Arduino.h>
#include <EEPROM.h>
#include <unity.h>
#include "device_config.h"

void test_device_config_write() {
    DeviceConfig config;
    config.version = 4;
    strncpy(config.name, "TestConfig", MAX_NAME_LENGTH - 1);
    config.name[MAX_NAME_LENGTH - 1] = '\0';
    config.ledOffset = 0;
    config.ledCount = 16;
    config.buttonPin = 255;

    DeviceConfigurator::writeConfig<DeviceConfig>(config);
    DeviceConfig readBack = DeviceConfigurator::readConfig();
    TEST_ASSERT_EQUAL_INT(4, readBack.version);
    TEST_ASSERT_EQUAL_STRING("TestConfig", readBack.name);
    TEST_ASSERT_EQUAL_INT(0, readBack.ledOffset);
    TEST_ASSERT_EQUAL_INT(16, readBack.ledCount);
    TEST_ASSERT_EQUAL_INT(255, readBack.buttonPin);
}

void test_migration_v2_to_v4() {
    DeviceConfig_v2 v2;
    v2.version = 2;
    strncpy(v2.name, "TestV2", MAX_NAME_LENGTH - 1);
    v2.name[MAX_NAME_LENGTH - 1] = '\0';
    
    DeviceConfigurator::writeConfig<DeviceConfig_v2>(v2);

    DeviceConfig migrated = DeviceConfigurator::readConfig();

    TEST_ASSERT_EQUAL_INT(4, migrated.version);
    TEST_ASSERT_EQUAL_STRING("TestV2", migrated.name);
    TEST_ASSERT_EQUAL_INT(0, migrated.ledOffset);
    TEST_ASSERT_EQUAL_INT(16, migrated.ledCount);
    TEST_ASSERT_EQUAL_INT(255, migrated.buttonPin);
}

void setup() {
    Serial.begin(115200);
    EEPROM.begin(512);
    delay(2000);

    UNITY_BEGIN();
    RUN_TEST(test_device_config_write);
    RUN_TEST(test_migration_v2_to_v4);
    UNITY_END();
}

void loop() {
}