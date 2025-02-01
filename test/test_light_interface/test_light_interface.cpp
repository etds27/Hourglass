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
#include "color_converter.h"


void compareBuffer(uint32_t *actualBuffer, uint32_t *expectedBuffer, uint8_t size) {
    for (int i = 0; i < size; i++) {
        char message[50];
        sprintf(message, "Element (%d) %d != %d", i, actualBuffer[i], expectedBuffer[i]);
        ASSERT_EQ(actualBuffer[i], expectedBuffer[i]) << message;
    }
}


TEST(LightInterfaceTest, OffsetBufferPositive) {
    uint32_t buffer[8] = {};
    buffer[2] = 1;

    uint32_t expectedBuffer[8] = {
        0, 0, 0, 0, 0, 1, 0, 0
    };

    ring->offsetBuffer(buffer, 3, 8);
    compareBuffer(buffer, expectedBuffer, 8);
}

TEST(LightInterfaceTest, OffsetBufferNegative) {
    uint32_t buffer[8] = {};
    buffer[2] = 1;

    uint32_t expectedBuffer[8] = {
        0, 0, 0, 0, 0, 0, 0, 1
    };

    ring->offsetBuffer(buffer, -3, 8);
    compareBuffer(buffer, expectedBuffer, 8);
}

TEST(LightInterfaceTest, ReverseBuffer) {
    uint32_t buffer[8] = {
        0, 1, 2, 3, 4, 5, 6, 7
    };
    uint32_t expectedBuffer[8] = {
        7, 6, 5, 4, 3, 2, 1, 0
    };

    ring->reverseBuffer(buffer, 8);
    compareBuffer(buffer, expectedBuffer, 8);
}

TEST(LightInterfaceTest, SolidBuffer) {
    uint32_t buffer[8] = {};

    uint32_t color = 1;
    uint32_t expectedBuffer[8] = {
        color, color, color, color, color, color, color, color
    };

    ring->solidBuffer(buffer, 8, color);
    compareBuffer(buffer, expectedBuffer, 8);
}

TEST(LightInterfaceTest, ColorBuffer) {
    uint32_t buffer[8] = {
        0, 2, 0, 2, 0, 2, 0, 2
    };

    uint32_t expectedBuffer[8] = {
        0, 1, 0, 1, 0, 1, 0, 1
    };

    ring->colorBuffer(buffer, 8, 1);
    compareBuffer(buffer, expectedBuffer, 8);
}

TEST(LightInterfaceTest, ExpandBufferCleanFill) {
    uint32_t buffer[4] = {
        0, 1, 2, 3
    };

    uint32_t fullBuffer[8] = {};

    uint32_t expectedBuffer[8] = {
        0, 0, 1, 1, 2, 2, 3, 3
    };

    ring->expandBuffer(buffer, fullBuffer, 4, 8, true);
    compareBuffer(fullBuffer, expectedBuffer, 8);
}

TEST(LightInterfaceTest, ExpandBufferCleanNoFill) {
    uint32_t buffer[4] = {
        0, 1, 2, 3
    };

    uint32_t fullBuffer[8] = {};

    uint32_t expectedBuffer[8] = {
        0, 0, 1, 0, 2, 0, 3, 0
    };

    ring->expandBuffer(buffer, fullBuffer, 4, 8, false);
    compareBuffer(fullBuffer, expectedBuffer, 8);
}

TEST(LightInterfaceTest, ExpandBufferNotCleanFill) {
    uint32_t buffer[4] = {
        0, 1, 2, 3
    };

    uint32_t fullBuffer[10] = {};

    uint32_t expectedBuffer[10] = {
        0, 0, 0, 1, 1, 1, 2, 2, 3, 3
    };

    ring->expandBuffer(buffer, fullBuffer, 4, 10, true);
    compareBuffer(fullBuffer, expectedBuffer, 10);
}

TEST(LightInterfaceTest, ExpandBufferNotCleanNoFill) {
    uint32_t buffer[4] = {
        0, 1, 2, 3
    };

    uint32_t fullBuffer[10] = {};

    uint32_t expectedBuffer[10] = {
        0, 0, 0, 1, 0, 0, 2, 0, 3, 0
    };
    ring->expandBuffer(buffer, fullBuffer, 4, 10, false);

    compareBuffer(fullBuffer, expectedBuffer, 10);
}

TEST(LightInterfaceTest, OverlayBuffer) {
    uint32_t buffer[8] = {
        1, 0, 1, 0, 1, 0, 1, 0
    };

    uint32_t overlayBuffer[8] = {
        0, 2, 0, 2, 0, 2, 0, 2
    };

    uint32_t expectedBuffer[8] = {
        1, 2, 1, 2, 1, 2, 1, 2
    };

    ring->overlayBuffer(buffer, overlayBuffer, 8, false);
    compareBuffer(buffer, expectedBuffer, 8);
}

TEST(LightInterfaceTest, OverlayBufferInverse) {
    uint32_t buffer[8] = {
        2, 2, 2, 2, 2, 2, 2, 2
    };

    uint32_t overlayBuffer[8] = {
        1, 1, 0, 1, 1, 0, 1, 1
    };

    uint32_t expectedBuffer[8] = {
        2, 2, 0, 2, 2, 0, 2, 2
    };

    ring->overlayBuffer(buffer, overlayBuffer, 8, true);
    compareBuffer(buffer, expectedBuffer, 8);
}

TEST(LightInterfaceTest, ExtendBuffer) {
    uint32_t buffer[2] = {
        1, 2
    };

    uint32_t fullBuffer[8] = {8};

    uint32_t expectedBuffer[8] = {
        1, 2, 1, 2, 1, 2, 1, 2
    };

    ring->extendBuffer(buffer, fullBuffer, 2, 8);
    compareBuffer(fullBuffer, expectedBuffer, 8);
}

TEST(LightInterfaceTest, ExtendBufferNegative) {
    uint32_t buffer[3] = {
        1, 2, 3
    };

    uint32_t fullBuffer[8] = {};

    uint32_t expectedBuffer[8] = {
        0, 0, 0, 0, 0, 0, 0, 0
    };

    ring->extendBuffer(buffer, fullBuffer, 3, 8);
    compareBuffer(fullBuffer, expectedBuffer, 8);
}

TEST(LightInterfaceTest, TransformBufferColor) {
    uint32_t buffer[3] = {
        RED, GREEN, BLUE
    };

    uint32_t expectedBuffer[3] = {
        BLUE, GREEN, RED
    };

    ColorTransform::ColorTransform *transform = new ColorTransform::SwapRedBlue();
    ring->transformBufferColor(buffer, 3, transform);
    delete transform;

    ring->printBuffer(buffer, 3);
    ring->printBuffer(expectedBuffer, 3);

    compareBuffer(buffer, expectedBuffer, 3);
}

TEST(LightInterfaceTest, CopyBuffer) {
    uint32_t buffer[2] = {
        1, 2
    };

    uint32_t fullBuffer[2] = {};

    uint32_t expectedBuffer[2] = {
        1, 2
    };

    ring->copyBuffer(buffer, fullBuffer, 2);
    compareBuffer(fullBuffer, expectedBuffer, 2);
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