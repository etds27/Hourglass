#pragma once
#include "light_interface.h"
#include <GL/glu.h>
#include <GL/gl.h>

class FastLEDLight : public LightInterface {
public:
  FastLEDLight(const uint8_t ledCount, const uint8_t diPin);
  ~FastLEDLight();


  void begin();
  void clear();
  void show();
  void setBrightness(uint8_t brightness);
  void setPixelColor(uint8_t i, uint32_t color);
};