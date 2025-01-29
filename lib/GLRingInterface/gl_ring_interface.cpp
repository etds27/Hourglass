#ifdef SIMULATOR

#include "gl_ring_interface.h"
#include "gl_tools.h"
#include <cmath>
#include <math.h>
#include "logger.h"
#include "constants.h"

float RING_TRANSLATION_X = -0.35f;
float RING_TRANSLATION_Y = 0.0f;
float RING_TRANSLATION_Z = 0.0f;
float RING_RADIUS = 0.33f;

float PIXEL_RADIUS = 0.05f;

void GLRingInterface::begin()
{
    // Not needed for FastLED
    return;
}

void GLRingInterface::clear()
{
    for (int i = 0; i < m_ledCount; i++)
    {
        m_leds[i] = 0x000000;
    }
    show();
}

void GLRingInterface::show()
{
    glPopMatrix();                      // Restore the model-view matrix
    glPushMatrix(); 
    glTranslatef(RING_TRANSLATION_X, RING_TRANSLATION_Y, RING_TRANSLATION_Z); // Translate left and up

    for (int i = 0; i < m_ledCount; i++)
    {
        drawPixel(i);
    }
    
    glPopMatrix();                      // Restore the model-view matrix
    glPushMatrix();                     // Save model-view matrix setting
}

void GLRingInterface::setBrightness(uint8_t brightness)
{
    m_brightness = brightness;
    logger.debug("Setting brightness to: ", brightness);
}

void GLRingInterface::drawPixel(const uint8_t i) {
    float theta = (float(i + 0.5f) / m_ledCount) * 2 * M_PI;
    float x = cosf(theta) * RING_RADIUS;
    float y = sinf(theta) * RING_RADIUS;

    gl_tools::drawCircle(x, y, PIXEL_RADIUS, 0x000000, 30);
    gl_tools::drawCircle(x, y, PIXEL_RADIUS - 0.005f, m_leds[i], 30);
}

void GLRingInterface::setPixelColor(uint8_t i, uint32_t color)
{
    m_leds[i] = color;
}

GLRingInterface::GLRingInterface(const uint8_t ledCount)
    : LightInterface(ledCount, 0)
{
    m_leds = new uint32_t[ledCount]{};
    setUp();
}

GLRingInterface::~GLRingInterface()
{
    delete[] m_leds;
}

#endif