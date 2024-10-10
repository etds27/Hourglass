#pragma once
#include <Adafruit_NeoPixel.h>
#include "device_state.h"
#include "constants.h"
#include "light_interface.h"

// Manage the display state of Neopixel devices
//
// Contains commmon lighting modes used for representing device states
class RingLight : public LightInterface {
private:
  Adafruit_NeoPixel* m_ring;
public:
  RingLight(const uint8_t ledCount, const uint8_t diPin);
  ~RingLight() {}

  void begin();
  void clear();
  void show();
  void setBrightness(uint8_t brightness);
  void setPixelColor(uint8_t i, uint32_t color);
};
