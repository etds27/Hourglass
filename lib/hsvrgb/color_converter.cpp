
#include "color_converter.h"
#include "logger.h"
#include <iostream>

namespace ColorTransform
{
    namespace {
        void constructColor(uint32_t &color, uint8_t v1, uint8_t v2, uint8_t v3)
        {
            color = (v1 << 16) | (v2 << 8) | v3;
        }
    }

    void constructRgb(uint32_t &rgb, uint8_t r, uint8_t g, uint8_t b)
    {
        constructColor(rgb, r, g, b);
    }

    void constructHsv(uint32_t &hsv, uint8_t h, uint8_t s, uint8_t v)
    {
        constructColor(hsv, h, s, v);
    }

    DimColor::DimColor(const uint8_t brightness)
    {
        m_brightness = brightness;
    }

    uint32_t DimColor::transformColor(const uint32_t color)
    {
        Rgb2Hsv* rgb2Hsv = new Rgb2Hsv(color);
        Hsv2Rgb* hsv2Rgb = new Hsv2Rgb(color);

        char buffer[50];
        sprintf(buffer, "RGB Color (Initial): %d", color);
        logger.debug(buffer);
        uint32_t hsvColor = rgb2Hsv->applyTransform(color);
        sprintf(buffer, "HSV Color (Initial): %d", hsvColor);
        logger.debug(buffer);

        uint8_t v = hsvColor & 0xFF;
        v = (double)(v * m_brightness) / 255.0;

        hsvColor = (hsvColor & 0xFFFF00) | v;
        sprintf(buffer, "HSV Color (Adjusted): %d", hsvColor);
        logger.debug(buffer);

        uint32_t adjustedRgb = hsv2Rgb->applyTransform(hsvColor);
        sprintf(buffer, "RGB Color (Adjusted): %d\n", adjustedRgb);
        logger.debug(buffer);
        
        delete rgb2Hsv;
        delete hsv2Rgb;

        return adjustedRgb;
    }

    BrightenColor::BrightenColor(const uint8_t brightness)
    {
        m_brightness = brightness;
    }

    uint32_t BrightenColor::transformColor(const uint32_t color)
    {
        return 0;
    }

    SwapRedBlue::SwapRedBlue()
    {
    }

    SwapRedGreen::SwapRedGreen()
    {
    }

    uint32_t SwapRedGreen::transformColor(uint32_t color)
    {
        uint8_t r = (color >> 16) & 0xFF;
        uint8_t g = (color >> 8) & 0xFF;
        uint8_t b = color & 0xFF;
        
        uint32_t newColor;
        constructColor(newColor, g, r, b);
        return newColor;
    }

    uint32_t SwapRedBlue::transformColor(const uint32_t color)
    {
        uint8_t r = (color >> 16) & 0xFF;
        uint8_t g = (color >> 8) & 0xFF;
        uint8_t b = color & 0xFF;

        uint32_t newColor;
        constructColor(newColor, b, g, r);
        return newColor;
    }

    SwapGreenBlue::SwapGreenBlue()
    {
    }

    uint32_t SwapGreenBlue::transformColor(const uint32_t color)
    {
        uint8_t r = (color >> 16) & 0xFF;
        uint8_t g = (color >> 8) & 0xFF;
        uint8_t b = color & 0xFF;

        uint32_t newColor;
        constructColor(newColor, r, b, g);
        return newColor;
    }

    InvertColor::InvertColor()
    {
    }

    uint32_t InvertColor::transformColor(const uint32_t color)
    {
        uint8_t r = (color >> 16) & 0xFF;
        uint8_t g = (color >> 8) & 0xFF;
        uint8_t b = color & 0xFF;

        uint32_t newColor;
        constructColor(newColor, (255 - r), (255 - g), (255 - b));
        return newColor;
    }

    ShiftLeft::ShiftLeft()
    {
    }

    uint32_t ShiftLeft::transformColor(const uint32_t color)
    {
        uint8_t r = (color >> 16) & 0xFF;
        uint8_t g = (color >> 8) & 0xFF;
        uint8_t b = color & 0xFF;
        uint32_t newColor;
        constructColor(newColor, g, b, r);
        return newColor;
    }

    ShiftRight::ShiftRight()
    {
    }

    uint32_t ShiftRight::transformColor(const uint32_t color)
    {
        uint8_t r = (color >> 16) & 0xFF;
        uint8_t g = (color >> 8) & 0xFF;
        uint8_t b = color & 0xFF;
        uint32_t newColor;
        constructColor(newColor, b, r, g);
        return newColor;
    }

    Rgb2Hsv::Rgb2Hsv(const uint32_t color)
    {
    }

    uint32_t Rgb2Hsv::transformColor(const uint32_t color)
    {
        uint32_t hsv = 0x000000;
        double min, max, delta;

        double r = ((color >> 16) & 0xFF) / 255.0;
        double g = ((color >> 8) & 0xFF) / 255.0;
        double b = (color & 0xFF) / 255.0;
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

    Hsv2Rgb::Hsv2Rgb(const uint32_t color)
    {
    }

    uint32_t Hsv2Rgb::transformColor(const uint32_t color)
    {
        double hh, p, q, t, ff;
        long i;
        uint32_t rgb;

        double h = ((color >> 16) & 0xFF) / 255.0 * 360.0;
        double s = ((color >> 8) & 0xFF) / 255.0;
        double v = (color & 0xFF) / 255.0;
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

    uint32_t ColorTransform::applyTransform(const uint32_t color)
    {
        return transformColor(color);
    }

    uint32_t ColorTransform::transformColor(const uint32_t color)
    {
        logger.error("At ColorTransform default implementation");
        return -1;
    }
} // namespace ColorTransform