#include <Arduino.h>
#include <unity.h>
#include "hg_display_manager.h"
#include "fast_led_light.h"
#include "logger.h"

void setUp(void)
{
    // set stuff up here
}

void tearDown(void)
{
    // clean stuff up here
}

void test_add_display_interface() {
    HourglassDisplayManager * manager = new HourglassDisplayManager();
    FastLEDLight * fRing = new FastLEDLight(16, 16);
    manager->addDisplayInterface(fRing);
    TEST_ASSERT_TRUE(manager->count() == 1);
}