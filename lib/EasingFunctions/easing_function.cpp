#include "easing_function.h"
#include "logger.h"
#include <cmath>

EasingFunction::EasingFunction::EasingFunction(EasingMode mode)
{
    m_mode = mode;
}

EasingFunction::Linear::Linear(EasingMode mode) : EasingFunction::EasingFunction::EasingFunction(mode)
{
}

EasingFunction::Quadratic::Quadratic(EasingMode mode) : EasingFunction::EasingFunction::EasingFunction(mode)
{
}

EasingFunction::Cubic::Cubic(EasingMode mode) : EasingFunction::EasingFunction::EasingFunction(mode)
{
}

EasingFunction::Sine::Sine(EasingMode mode) : EasingFunction::EasingFunction::EasingFunction(mode)
{
}

EasingFunction::Circular::Circular(EasingMode mode) : EasingFunction::EasingFunction::EasingFunction(mode)
{
}

EasingFunction::Poly::Poly(uint8_t power, EasingMode mode) : EasingFunction::EasingFunction::EasingFunction(mode)
{
    m_power = power;
}

double EasingFunction::EasingFunction::ease(double value, double start, double end)
{
    if (value == start)
    {
        return start;
    }

    if (value == end)
    {
        return end;
    }

    if (end < start)
    {
        logger.error("End < Start");
        return 0.0;
    }

    if (value < start || value > end)
    {
        logger.error("Value outside of bounds");

        return 0.0;
    }

    if (start < 0 || end < 0)
    {
        logger.error("Start or end less than 0");
        return 0.0;
    }

    double adjustedValue = value;
    adjustedValue = (value - start) / (end - start);

    if (m_mode == EasingMode::EaseInAndOut)
    {
        double midpoint = 0.5;
        // If we are closer to the start
        if (adjustedValue < midpoint)
        {
            adjustedValue *= 2;
            m_mode = EasingMode::EaseIn;
            adjustedValue = ease(adjustedValue, 0.0, 1) / 2;
        }
        else
        {
            adjustedValue = 2 * (adjustedValue - midpoint);
            m_mode = EasingMode::EaseOut;
            adjustedValue = ease(adjustedValue, 0.0, 1) / 2 + 0.5;
        }
        m_mode = EasingMode::EaseInAndOut;
        return adjustedValue * (end - start) + start;
    }
    else if (m_mode == EasingMode::EaseOutAndIn)
    {
        double midpoint = 0.5;
        // If we are closer to the start
        if (adjustedValue < midpoint)
        {
            adjustedValue *= 2;
            m_mode = EasingMode::EaseOut;
            adjustedValue = ease(adjustedValue, 0.0, 1) / 2;
        }
        else
        {
            adjustedValue = 2 * (adjustedValue - midpoint);
            m_mode = EasingMode::EaseIn;
            adjustedValue = ease(adjustedValue, 0.0, 1) / 2 + 0.5;
        }
        m_mode = EasingMode::EaseOutAndIn;
        return adjustedValue * (end - start) + start;
    }
    /*
    logger.debug("Value:    " + String(value));
    logger.debug("Start:    " + String(start));
    logger.debug("End:      " + String(end));
    logger.debug("Adjusted: " + String(adjustedValue));
    */
    switch (m_mode)
    {
    case EasingMode::EaseIn:
        adjustedValue = calculatePointIn(value) * (end - start) + start;
        break;
    case EasingMode::EaseOut:
        adjustedValue = calculatePointOut(value) * (end - start) + start;
    default:
        break;
    }
    return adjustedValue;
}

double EasingFunction::EasingFunction::calculatePointIn(double value)
{
    logger.error("At EasingFunction default implementation");
    return -1.0;
}

double EasingFunction::EasingFunction::calculatePointOut(double value)
{
    logger.error("At EasingFunction default implementation");
    return -1.0;
}

double EasingFunction::Linear::calculatePointIn(double value)
{
    return value;
}

double EasingFunction::Linear::calculatePointOut(double value)
{
    return value;
}

double EasingFunction::Quadratic::calculatePointIn(double value)
{
    return value * value;
}

double EasingFunction::Quadratic::calculatePointOut(double value)
{
    return 1 - (1 - value) * (1 - value);
}

double EasingFunction::Cubic::calculatePointIn(double value)
{
    return pow(value, 3);
}

double EasingFunction::Cubic::calculatePointOut(double value)
{
    return 1 - pow(1 - value, 3);
}

double EasingFunction::Circular::calculatePointIn(double value)
{
    return 1 - sqrt(1 - pow(value, 2));
}

double EasingFunction::Circular::calculatePointOut(double value)
{
    return sqrt(1 - pow(value - 1, 2));
}

double EasingFunction::Poly::calculatePointIn(double value)
{
    return pow(value, m_power);
}

double EasingFunction::Poly::calculatePointOut(double value)
{
    return 1 - pow(1 - value, m_power);
}

double EasingFunction::Sine::calculatePointIn(double value)
{
    return 1 - cos((value * M_PI) / 2);
}

double EasingFunction::Sine::calculatePointOut(double value)
{
    return sin((value * M_PI) / 2);
}
