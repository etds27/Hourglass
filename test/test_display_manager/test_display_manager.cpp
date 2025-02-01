#ifdef SIMULATOR
#include <gtest/gtest.h>
#include "light_interface.h"
#include "gl_ring_interface.h"
LightInterface *ring = new GLRingInterface(16);
#else
#include <Arduino.h>
#include "fast_led_light.h"

LightInterface *ring = new FastLEDLight(16, 16);
#endif
#include "hg_display_manager.h"
#include "logger.h"



void setUp(void)
{
    // set stuff up here
}

void tearDown(void)
{
    // clean stuff up here
}

TEST(DisplayManagerTest, AddDisplayInterface)
{
    HourglassDisplayManager * manager = new HourglassDisplayManager();
    manager->addDisplayInterface(ring);
    EXPECT_EQ(manager->count(), 1) << "Incorrect interface count";
}

#ifndef SIMULATOR
void setup()
{
    // should be the same value as for the `test_speed` option in "platformio.ini"
    // default value is test_speed=115200
    Serial.begin(115200);

    ::testing::InitGoogleTest();
}

void loop()
{
	// Run tests
	if (RUN_ALL_TESTS())
	;

	// sleep 1 sec
	delay(1000);
}

#else
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
	if (RUN_ALL_TESTS())
	;
	// Always return zero-code and allow PlatformIO to parse results
	return 0;
}
#endif