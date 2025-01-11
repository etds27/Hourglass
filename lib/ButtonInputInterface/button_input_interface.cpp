#include "button_input_interface.h"

#ifndef SIMULATOR
#include <Arduino.h>

ButtonInputInterface::ButtonInputInterface(uint8_t pin)
{
    m_pin = pin;
    pinMode(m_pin, INPUT);
}

bool ButtonInputInterface::getValue()
{
    return digitalRead(m_pin) == HIGH;
}

#endif