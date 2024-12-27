#include "FastLED.h"
#include "fast_led_light.h"
#include "logger.h"

void FastLEDLight::begin() {
  // Not needed for FastLED
  return;
}

void FastLEDLight::clear() {
  FastLED.clear();
}

void FastLEDLight::show() {
  FastLED.show();
}

void FastLEDLight::setBrightness(uint8_t brightness) {
  FastLED.setBrightness(brightness);
  logger.debug("Setting brightness to: " + String(brightness));
}

void FastLEDLight::setPixelColor(uint8_t i, uint32_t color) {
  leds[i] = CRGB(color);
}

FastLEDLight::FastLEDLight(const uint8_t ledCount, const uint8_t diPin)
  : LightInterface(ledCount, diPin) {
  leds = new CRGB[ledCount];
  FastLED.addLeds<NEOPIXEL, RING_DI_PIN>(leds, ledCount);
  setUp();
}

FastLEDLight::~FastLEDLight() {
  delete[] leds;
}
