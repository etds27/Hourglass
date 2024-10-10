#pragma once
#include <stdint.h>
#include "device_state.h"

#define LONG_PRESS_DURATION 500
#define MULTI_CLICK_TIMEOUT 300


enum class ButtonInputType {
  None,
  ButtonPress,
  DoubleButtonPress,
  TripleButtonPress,
  LongPress,
  TurnSequence
};

class ButtonInputMonitor {
  public:
    ButtonInputMonitor(uint8_t pin);
    ButtonInputType getAction();

  private:
    uint8_t m_pin;
    unsigned long last_press_start;
    unsigned long last_press_release;
    bool isPressed;
    uint8_t currentPresses;
    bool getValue();
};