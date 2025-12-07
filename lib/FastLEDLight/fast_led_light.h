#pragma once

#ifndef SIMULATOR
#include <FastLED.h>
#include "light_interface.h"

class FastLEDLight : public LightInterface {
public:
  FastLEDLight(const uint8_t ledCount, const uint8_t diPin);
  ~FastLEDLight();


  void begin();
  void clear();
  void show();
  void setBrightness(uint8_t brightness);
  void setPixelColor(uint8_t i, uint32_t color);
private:
  CRGB* leds;
};

#endif