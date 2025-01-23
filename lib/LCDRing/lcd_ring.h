#pragma once
#include "light_interface.h"
#include <TFT_eSPI.h>
#include <vector>

class LCDRing : public LightInterface
{
public:
    LCDRing(const uint8_t ledCount, TFT_eSPI *tft);
    ~LCDRing();

    void begin();
    void clear();
    void show();
    void setBrightness(uint8_t brightness);
    void setPixelColor(uint8_t i, uint32_t color);

private:
  void drawPixel(const uint8_t i);
   std::vector<uint32_t> m_leds;
   std::vector<uint32_t> m_previousLeds;
   uint8_t m_brightness;
   TFT_eSPI *m_tft;
};