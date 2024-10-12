#include "Adafruit_NeoPixel.h"
#include "Arduino.h"
#include "ring_light.h"
#include "logger.h"

void RingLight::begin() {
  m_ring->begin();
}

void RingLight::clear() {
  m_ring->clear();
}

void RingLight::show() {
  m_ring->show();
}
void RingLight::setBrightness(uint8_t brightness) {
  m_ring->setBrightness(brightness);
}

void RingLight::setPixelColor(uint8_t i, uint32_t color) {
  m_ring->setPixelColor(i, color);
}

RingLight::RingLight(const uint8_t ledCount, const uint8_t diPin) : LightInterface(ledCount, diPin) {
  m_ring = new Adafruit_NeoPixel(m_ledCount, m_diPin, NEO_GRB + NEO_KHZ800);
  setUp();
}


