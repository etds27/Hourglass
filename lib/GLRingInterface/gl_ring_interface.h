#pragma once

#ifdef SIMULATOR

#include "light_interface.h"
#include <GL/glu.h>
#include <GL/gl.h>


class GLRingInterface : public LightInterface {
public:
  GLRingInterface(const uint8_t ledCount);
  ~GLRingInterface();


  void begin();
  void clear();
  void show();
  void setBrightness(uint8_t brightness);
  void setPixelColor(uint8_t i, uint32_t color);

  private:
  void drawPixel(const uint8_t i);
   uint32_t *m_leds;
   uint8_t m_brightness;
};

#endif