
#include <gtest/gtest.h>
#include "easing_function.h"
#include "logger.h"
#include <cmath>
void setUp(void)
{
    // set stuff up here
}

void tearDown(void)
{
    // clean stuff up here
}

void easing_function_verification(EasingFunction::EasingFunction *function, uint8_t *expectedValues, uint8_t dataPoints);
void test_linear_easing_in();
void test_linear_easing_out();
void test_linear_easing_in_and_out();
void test_linear_easing_out_and_in();

void test_quadratic_easing_in();
void test_quadratic_easing_out();
void test_quadratic_easing_in_and_out();
void test_quadratic_easing_out_and_in();

void test_cubic_easing_in();
void test_cubic_easing_out();
void test_cubic_easing_in_and_out();
void test_cubic_easing_out_and_in();

void test_poly_easing_in();
void test_poly_easing_out();
void test_poly_easing_in_and_out();
void test_poly_easing_out_and_in();

void test_sine_easing_in();
void test_sine_easing_out();
void test_sine_easing_in_and_out();
void test_sine_easing_out_and_in();

void test_circular_easing_in();
void test_circular_easing_out();
void test_circular_easing_in_and_out();
void test_circular_easing_out_and_in();

void easing_function_verification(EasingFunction::EasingFunction *function, uint8_t *expectedValues, uint8_t dataPoints)
{

    double interval = 1.0 / dataPoints;
    for (int i = 0; i <= dataPoints; i++)
    {
        double input = i * interval;
        double val = function->ease(input);
        uint8_t adjustedVal = std::round(val * 100);
        ASSERT_GE(adjustedVal, expectedValues[i] - 1) << "Value is less than expected value";
        ASSERT_LE(adjustedVal, expectedValues[i] + 1) << "Value is greater than expected value";
    }
}

TEST(EasingTest, LinearEaseIn)
{
    EasingFunction::EasingFunction *function = new EasingFunction::Linear(EasingMode::EaseIn);
    uint8_t expectedValues[] = {
        0,
        5,
        10,
        15,
        20,
        25,
        30,
        35,
        40,
        45,
        50,
        55,
        60,
        65,
        70,
        75,
        80,
        85,
        90,
        95,
        100,
    };
    easing_function_verification(function, expectedValues, 20);
}

TEST(EasingTest, LinearEaseOut)
{
    EasingFunction::EasingFunction *function = new EasingFunction::Linear(EasingMode::EaseOut);
    uint8_t expectedValues[] = {
        0,
        5,
        10,
        15,
        20,
        25,
        30,
        35,
        40,
        45,
        50,
        55,
        60,
        65,
        70,
        75,
        80,
        85,
        90,
        95,
        100,
    };
    easing_function_verification(function, expectedValues, 20);
}

TEST(EasingTest, LinearEaseInOut)
{
    EasingFunction::EasingFunction *function = new EasingFunction::Linear(EasingMode::EaseInAndOut);
    uint8_t expectedValues[] = {
        0,
        5,
        10,
        15,
        20,
        25,
        30,
        35,
        40,
        45,
        50,
        55,
        60,
        65,
        70,
        75,
        80,
        85,
        90,
        95,
        100,
    };
    easing_function_verification(function, expectedValues, 20);
}

TEST(EasingTest, LinearEaseOutIn)
{
    EasingFunction::EasingFunction *function = new EasingFunction::Linear(EasingMode::EaseOutAndIn);
    uint8_t expectedValues[] = {
        0,
        5,
        10,
        15,
        20,
        25,
        30,
        35,
        40,
        45,
        50,
        55,
        60,
        65,
        70,
        75,
        80,
        85,
        90,
        95,
        100,
    };
    easing_function_verification(function, expectedValues, 20);
}

TEST(EasingTest, QuadraticEaseIn)
{
    EasingFunction::EasingFunction *function = new EasingFunction::Quadratic(EasingMode::EaseIn);
    uint8_t expectedValues[] = {
        0,
        0,
        1,
        2,
        4,
        6,
        9,
        12,
        16,
        20,
        25,
        30,
        36,
        42,
        49,
        56,
        64,
        72,
        81,
        90,
        100,
    };
    easing_function_verification(function, expectedValues, 20);
}

TEST(EasingTest, QuadraticEaseOut)
{
    EasingFunction::EasingFunction *function = new EasingFunction::Quadratic(EasingMode::EaseOut);
    uint8_t expectedValues[] = {
        0,
        10,
        19,
        28,
        36,
        44,
        51,
        58,
        64,
        70,
        75,
        80,
        84,
        88,
        91,
        94,
        96,
        98,
        99,
        100,
        100,
    };
    easing_function_verification(function, expectedValues, 20);
}

TEST(EasingTest, QuadraticEaseInOut)
{
    EasingFunction::EasingFunction *function = new EasingFunction::Quadratic(EasingMode::EaseInAndOut);
    uint8_t expectedValues[] = {
        0,
        1,
        2,
        5,
        8,
        13,
        18,
        25,
        32,
        41,
        50,
        60,
        68,
        76,
        82,
        88,
        92,
        96,
        98,
        100,
        100};
    easing_function_verification(function, expectedValues, 20);
}

TEST(EasingTest, QuadraticEaseOutIn)
{
    EasingFunction::EasingFunction *function = new EasingFunction::Quadratic(EasingMode::EaseOutAndIn);
    uint8_t expectedValues[] = {
        0,
        10,
        18,
        26,
        32,
        38,
        42,
        46,
        48,
        50,
        50,
        51,
        52,
        55,
        58,
        63,
        68,
        75,
        82,
        91,
        100,
    };
    easing_function_verification(function, expectedValues, 20);
}

TEST(EasingTest, CubicEaseIn)
{
    EasingFunction::EasingFunction *function = new EasingFunction::Cubic(EasingMode::EaseIn);
    uint8_t expectedValues[] = {
        0,
        0,
        0,
        0,
        1,
        2,
        3,
        4,
        6,
        9,
        13,
        17,
        22,
        27,
        34,
        42,
        51,
        61,
        73,
        86,
        100,
    };
    easing_function_verification(function, expectedValues, 20);
}

TEST(EasingTest, CubicEaseOut)
{
    EasingFunction::EasingFunction *function = new EasingFunction::Cubic(EasingMode::EaseOut);
    uint8_t expectedValues[] = {
        0,
        14,
        27,
        39,
        49,
        58,
        66,
        73,
        78,
        83,
        88,
        91,
        94,
        96,
        97,
        98,
        99,
        100,
        100,
        100,
        100,
    };
    easing_function_verification(function, expectedValues, 20);
}

TEST(EasingTest, CubicEaseInOut)
{
    EasingFunction::EasingFunction *function = new EasingFunction::Cubic(EasingMode::EaseInAndOut);
    uint8_t expectedValues[] = {
        0,
        0,
        0,
        1,
        3,
        6,
        11,
        17,
        26,
        36,
        50,
        64,
        74,
        83,
        89,
        94,
        97,
        99,
        100,
        100,
        100,
    };
    easing_function_verification(function, expectedValues, 20);
}

TEST(EasingTest, CubicEaseOutIn)
{
    EasingFunction::EasingFunction *function = new EasingFunction::Cubic(EasingMode::EaseOutAndIn);
    uint8_t expectedValues[] = {
        0,
        14,
        24,
        33,
        39,
        44,
        47,
        49,
        50,
        50,
        50,
        50,
        50,
        51,
        53,
        56,
        61,
        67,
        76,
        86,
        100,
    };
    easing_function_verification(function, expectedValues, 20);
}

TEST(EasingTest, PolyEaseIn)
{
    EasingFunction::EasingFunction *function = new EasingFunction::Poly(4, EasingMode::EaseIn);
    uint8_t expectedValues[] = {
        0,
        0,
        0,
        0,
        0,
        0,
        1,
        2,
        3,
        4,
        6,
        9,
        13,
        18,
        24,
        32,
        41,
        52,
        66,
        81,
        100,
    };
    easing_function_verification(function, expectedValues, 20);
}

TEST(EasingTest, PolyEaseOut)
{
    EasingFunction::EasingFunction *function = new EasingFunction::Poly(4, EasingMode::EaseOut);
    uint8_t expectedValues[] = {
        0,
        19,
        34,
        48,
        59,
        68,
        76,
        82,
        87,
        91,
        94,
        96,
        97,
        98,
        99,
        100,
        100,
        100,
        100,
        100,
        100,
    };
    easing_function_verification(function, expectedValues, 20);
}

TEST(EasingTest, PolyEaseInOut)
{
    EasingFunction::EasingFunction *function = new EasingFunction::Poly(4, EasingMode::EaseInAndOut);
    uint8_t expectedValues[] = {
        0,
        0,
        0,
        0,
        1,
        3,
        6,
        12,
        20,
        33,
        50,
        67,
        80,
        88,
        94,
        97,
        99,
        100,
        100,
        100,
        100,
    };
    easing_function_verification(function, expectedValues, 20);
}

TEST(EasingTest, PolyEaseOutIn)
{
    EasingFunction::EasingFunction *function = new EasingFunction::Poly(4, EasingMode::EaseOutAndIn);
    uint8_t expectedValues[] = {
        0,
        17,
        30,
        38,
        44,
        47,
        49,
        50,
        50,
        50,
        50,
        50,
        50,
        50,
        51,
        53,
        56,
        62,
        70,
        83,
        100,
    };
    easing_function_verification(function, expectedValues, 20);
}

TEST(EasingTest, SineEaseIn)
{
    EasingFunction::EasingFunction *function = new EasingFunction::Sine(EasingMode::EaseIn);
    uint8_t expectedValues[] = {
        0,
        0,
        1,
        3,
        5,
        8,
        11,
        15,
        19,
        24,
        29,
        35,
        41,
        48,
        55,
        62,
        69,
        77,
        84,
        92,
        100,
    };
    easing_function_verification(function, expectedValues, 20);
    delete function;
}

TEST(EasingTest, SineEaseOut)
{
    EasingFunction::EasingFunction *function = new EasingFunction::Sine(EasingMode::EaseOut);
    uint8_t expectedValues[] = {
        0,
        8,
        16,
        23,
        31,
        38,
        45,
        52,
        59,
        65,
        71,
        76,
        81,
        85,
        89,
        92,
        95,
        97,
        99,
        100,
        100,
    };
    easing_function_verification(function, expectedValues, 20);
    delete function;
}

TEST(EasingTest, SineEaseInOut)
{
    EasingFunction::EasingFunction *function = new EasingFunction::Sine(EasingMode::EaseInAndOut);
    uint8_t expectedValues[] = {
        0,
        1,
        2,
        5,
        10,
        15,
        21,
        27,
        35,
        42,
        50,
        58,
        65,
        73,
        79,
        85,
        90,
        95,
        98,
        99,
        100,
    };
    easing_function_verification(function, expectedValues, 20);
    delete function;
}

TEST(EasingTest, SineEaseOutIn)
{
    EasingFunction::EasingFunction *function = new EasingFunction::Sine(EasingMode::EaseOutAndIn);
    uint8_t expectedValues[] = {
        0,
        8,
        15,
        23,
        29,
        35,
        40,
        45,
        48,
        49,
        50,
        51,
        52,
        55,
        60,
        65,
        71,
        77,
        85,
        92,
        100,
    };
    easing_function_verification(function, expectedValues, 20);
    delete function;
}

TEST(EasingTest, CircularEaseIn)
{
    EasingFunction::EasingFunction *function = new EasingFunction::Circular(EasingMode::EaseIn);
    uint8_t expectedValues[] = {
        0,
        0,
        1,
        1,
        2,
        3,
        5,
        6,
        8,
        11,
        13,
        16,
        20,
        24,
        29,
        34,
        40,
        47,
        56,
        69,
        100,
    };
    easing_function_verification(function, expectedValues, 20);
    delete function;
}

TEST(EasingTest, CircularEaseOut)
{
    EasingFunction::EasingFunction *function = new EasingFunction::Circular(EasingMode::EaseOut);
    uint8_t expectedValues[] = {
        0,
        31,
        44,
        53,
        60,
        66,
        71,
        76,
        80,
        84,
        87,
        89,
        92,
        94,
        95,
        97,
        98,
        99,
        99,
        100,
        100,
    };
    easing_function_verification(function, expectedValues, 20);
    delete function;
}

TEST(EasingTest, CircularEaseInOut)
{
    EasingFunction::EasingFunction *function = new EasingFunction::Circular(EasingMode::EaseInAndOut);
    uint8_t expectedValues[] = {
        0,
        0,
        1,
        2,
        4,
        7,
        10,
        14,
        20,
        28,
        50,
        72,
        80,
        86,
        90,
        93,
        96,
        98,
        99,
        100,
        100,
    };
    easing_function_verification(function, expectedValues, 20);
    delete function;
}

TEST(EasingTest, CircularEaseOutIn)
{
    EasingFunction::EasingFunction *function = new EasingFunction::Circular(EasingMode::EaseOutAndIn);
    uint8_t expectedValues[] = {
        0,
        22,
        30,
        36,
        40,
        43,
        46,
        48,
        49,
        50,
        50,
        50,
        51,
        52,
        54,
        57,
        60,
        64,
        70,
        78,
        100,
    };
    easing_function_verification(function, expectedValues, 20);
    delete function;
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