#pragma once

#include <stdint.h>

namespace ColorTransform
{
    class ColorTransform
    {
    public:
        /// @brief Apply a transform to the color provided and return a new one
        /// @param color Color to transform
        /// @param transform Function that transforms color
        /// @return New transformed color
        uint32_t applyTransform(const uint32_t color);

    protected:
        virtual uint32_t transformColor(const uint32_t color);
    };

    class Rgb2Hsv : public ColorTransform
    {
    public:
        Rgb2Hsv(const uint32_t color);

    protected:
        virtual uint32_t transformColor(const uint32_t color) override;
    };

    class Hsv2Rgb : public ColorTransform
    {
    public:
        Hsv2Rgb(const uint32_t color);

    protected:
        virtual uint32_t transformColor(const uint32_t color) override;
    };

    class DimColor : public ColorTransform
    {
    public:
        /// @brief Dim an RGB color by a specific value
        ///
        /// The color will be converted to HSV, then have the brightness value reduced, then converted back to RGB
        /// @param brightness Brightness adjustment factor between 0 and 255
        DimColor(const uint8_t brightness);

    protected:
        virtual uint32_t transformColor(const uint32_t color) override;

    private:
        uint8_t m_brightness;
    };

    class BrightenColor : public ColorTransform
    {
    public:
        BrightenColor(const uint8_t brightness);

    protected:
        virtual uint32_t transformColor(const uint32_t color) override;

    private:
        uint8_t m_brightness;
    };

    class SwapRedGreen : public ColorTransform
    {
    public:
        SwapRedGreen();

    protected:
        virtual uint32_t transformColor(const uint32_t color) override;
    };

    class SwapRedBlue : public ColorTransform
    {
    public:
        SwapRedBlue();

    protected:
        virtual uint32_t transformColor(const uint32_t color) override;
    };

    class SwapGreenBlue : public ColorTransform
    {
    public:
        SwapGreenBlue();

    protected:
        virtual uint32_t transformColor(const uint32_t color) override;
    };

    class InvertColor : public ColorTransform
    {
    public:
        InvertColor();

    protected:
        virtual uint32_t transformColor(const uint32_t color) override;
    };

    class ShiftRight : public ColorTransform
    {
    public:
        ShiftRight();

    protected:
        virtual uint32_t transformColor(const uint32_t color) override;
    };

    class ShiftLeft : public ColorTransform
    {
    public:
        ShiftLeft();

    protected:
        virtual uint32_t transformColor(const uint32_t color) override;
    };

    void constructRgb(uint32_t &rgb, uint8_t r, uint8_t g, uint8_t b);
    void constructHsv(uint32_t &hsv, uint8_t h, uint8_t s, uint8_t v);
}