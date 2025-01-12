
#ifndef HSVCONVERTER_H
#define HSVCONVERTER_H

#include <stdint.h>

namespace HsvConverter
{
    uint32_t rgb2hsv(uint32_t rgb);
    uint32_t hsv2rgb(uint32_t hsv);
    void constructRgb(uint32_t &rgb, uint8_t r, uint8_t g, uint8_t b);
    void constructHsv(uint32_t &hsv, uint8_t h, uint8_t s, uint8_t v);
    void constructColor(uint32_t &color, uint8_t v1, uint8_t v2, uint8_t v3);
}

#endif // HSVCONVERTER_H
