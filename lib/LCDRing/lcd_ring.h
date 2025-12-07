#pragma once
#include "light_interface.h"
#include <TFT_eSPI.h>
#include <vector>

class LCDRing : public LightInterface
{
public:
    LCDRing(const uint8_t ledCount, TFT_eSPI *tft);
    ~LCDRing();

    void begin() override;
    void clear() override;
    void show() override;
    void setBrightness(uint8_t brightness) override;
    void setPixelColor(uint8_t i, uint32_t color) override;

protected:
  bool m_clearBeforeUpdate = false;
  bool getClearBeforeUpdate() const override;

private:
  void drawPixel(const uint8_t i);
  uint16_t rgb24to565(uint32_t color);
   std::vector<uint16_t> m_leds;
   std::vector<uint16_t> m_previousLeds;
   uint8_t m_brightness;
   TFT_eSPI *m_tft;
};