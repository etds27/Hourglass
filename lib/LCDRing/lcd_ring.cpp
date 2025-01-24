#include "lcd_ring.h"
#include "logger.h"
#include <stdlib.h>
#include <iterator>
#include <algorithm>

#define PIXEL_RADIUS 15
#define RING_RADIUS (TFT_WIDTH - (PIXEL_RADIUS + 35)) / 2

LCDRing::LCDRing(const uint8_t ledCount, TFT_eSPI *tft)
{
    m_tft = tft;
    m_leds = std::vector<uint16_t>(m_ledCount, 0);
    m_previousLeds = std::vector<uint16_t>(m_ledCount, 0);
    setUp();
}

LCDRing::~LCDRing()
{
}

void LCDRing::begin()
{
    // Not needed for LCDRing
    return;
}

void LCDRing::clear()
{
    for (int i = 0; i < m_ledCount; i++)
    {
        m_leds[i] = 0x000000;
    }
    // show();
}

void LCDRing::show()
{
    uint8_t drawn = 0;
    for (int i = 0; i < m_ledCount; i++)
    {
        // Only draw the pixel if it differs from the last time it was displayed
        if (m_leds[i] != m_previousLeds[i]) {
            drawn += 1;
            drawPixel(i);
        }
    }
    m_previousLeds = m_leds;
}

void LCDRing::setBrightness(uint8_t brightness)
{
    m_brightness = brightness;
    logger.debug("Setting brightness to: " + brightness);
}

void LCDRing::setPixelColor(uint8_t i, uint32_t color)
{

    m_leds[i] = rgb24to565(color);
}

uint8_t LCDRing::getRingOffset() const
{
    return m_ringOffset;
}

bool LCDRing::getClearBeforeUpdate() const
{
    return m_clearBeforeUpdate;
}

void LCDRing::drawPixel(const uint8_t i)
{
    double theta = (i  + 0.5) / 16.0 * 2 * PI;
    int centerX = (int)(RING_RADIUS * cos(theta)) + TFT_WIDTH / 2;
    int centerY = (int)(RING_RADIUS * sin(theta)) + TFT_HEIGHT / 2;
    m_tft->fillSmoothCircle(centerX, centerY, PIXEL_RADIUS, m_leds[i]);
}

// TODO: Move to color conversion library when available
uint16_t LCDRing::rgb24to565(uint32_t color)
{
    uint8_t r = color >> 16;
    uint8_t g = color >> 8 & 0xFF;
    uint8_t b = color & 0xFF;
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}
