
#include "HsvConverter.h"
#include "logger.h"
#include <iostream>

namespace HsvConverter
{

    uint32_t rgb2hsv(uint32_t rgb)
    {
        uint32_t hsv = 0x000000;
        double min, max, delta;

        double r = ((rgb >> 16) & 0xFF) / 255.0;
        double g = ((rgb >> 8) & 0xFF) / 255.0;
        double b = (rgb & 0xFF) / 255.0;
        char buffer[50];
        sprintf(buffer, "rgb2hsv: R: %f G: %f B: %f", r, g, b);
        logger.debug(buffer);
        double h, s, v = 0.0;

        min = r < g ? r : g;
        min = min < b ? min : b;

        max = r > g ? r : g;
        max = max > b ? max : b;

        v = max;
        delta = max - min;
        if (delta < 0.00001)
        {
            constructHsv(hsv, 0, 0, v);
            return hsv;
        }
        if (max > 0.0)
        { // NOTE: if Max is == 0, this divide would cause a crash
            h = 0;
            s = (delta / max);
        }
        else
        {
            // if max is 0, then r = g = b = 0
            // s = 0, h is undefined
            constructHsv(hsv, 0, 0, v);
            return hsv;
        }

        if (r >= max)
        {                        // > is bogus, just keeps compilor happy
            h = (g - b) / delta; // between yellow & magenta
        }
        else if (g >= max)
        {
            h = 2.0 + (b - r) / delta; // between cyan & yellow
        }
        else
        {
            h = 4.0 + (r - g) / delta; // between magenta & cyan
        }
        h *= 60.0; // degrees

        if (h < 0.0)
            h += 360.0;

        sprintf(buffer, "rgb2hsv: H: %f S: %f V: %f", h, s, v);
        logger.debug(buffer);
        h = h / 360.0 * 255;
        s *= 255;
        v *= 255;
        constructHsv(hsv, h, s, v);
        return hsv;
    }

    uint32_t hsv2rgb(uint32_t hsv)
    {
        double hh, p, q, t, ff;
        long i;
        uint32_t rgb;

        double h = ((hsv >> 16) & 0xFF) / 255.0 * 360.0;
        double s = ((hsv >> 8) & 0xFF) / 255.0;
        double v = (hsv & 0xFF) / 255.0;
        char buffer[50];
        sprintf(buffer, "hsv2rgb: H: %f S: %f V: %f", h, s, v);
        logger.debug(buffer);

        double r, g, b = 0;

        if (s <= 0.0)
        { // < is bogus, just shuts up warnings
            r = v;
            g = v;
            b = v;
            constructRgb(rgb, r, g, b);
            return rgb;
        }
        hh = h;
        if (hh >= 360.0)
            hh = 0.0;
        hh /= 60.0;
        i = (long)hh;
        ff = hh - i;
        p = v * (1.0 - s);
        q = v * (1.0 - (s * ff));
        t = v * (1.0 - (s * (1.0 - ff)));

        switch (i)
        {
        case 0:
            r = v;
            g = t;
            b = p;
            break;
        case 1:
            r = q;
            g = v;
            b = p;
            break;
        case 2:
            r = p;
            g = v;
            b = t;
            break;

        case 3:
            r = p;
            g = q;
            b = v;
            break;
        case 4:
            r = t;
            g = p;
            b = v;
            break;
        case 5:
        default:
            r = v;
            g = p;
            b = q;
            break;
        }
        sprintf(buffer, "hsv2rgb: R: %f G: %f B: %f", r, g, b);
        logger.debug(buffer);
        r *= 255.0;
        g *= 255.0;
        b *= 255.0;
        constructRgb(rgb, r, g, b);

        return rgb;
    }

    void constructRgb(uint32_t &rgb, uint8_t r, uint8_t g, uint8_t b)
    {
        constructColor(rgb, r, g, b);
    }

    void constructHsv(uint32_t &hsv, uint8_t h, uint8_t s, uint8_t v)
    {
        constructColor(hsv, h, s, v);
    }

    void constructColor(uint32_t &color, uint8_t v1, uint8_t v2, uint8_t v3)
    {
        color = (v1 << 16) | (v2 << 8) | v3;
    }

} // namespace HsvConverter
