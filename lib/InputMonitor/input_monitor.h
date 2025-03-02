#pragma once
#include <stdint.h>
#include "device_state.h"
#include "input_interface.h"

// A press must have been released for this amount of time to register a new press
#define MIN_RELEASE_TIME 10
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
    ButtonInputMonitor(InputInterface* inputInterface);
    ButtonInputType getAction();

  private:
    InputInterface* m_inputInterface;
    unsigned long last_press_start;
    unsigned long last_press_release;
    bool isPressed;
    uint8_t currentPresses;
};