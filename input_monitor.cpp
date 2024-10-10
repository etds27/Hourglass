#include "input_monitor.h"
#include "logger.h"



ButtonInputMonitor::ButtonInputMonitor(uint8_t pin) {
  m_pin = pin;
  last_press_start = millis();
  last_press_release = millis();
  isPressed = false;
  currentPresses = 0;

  pinMode(m_pin, INPUT);
};

bool ButtonInputMonitor::getValue() {
  return digitalRead(m_pin) == HIGH;
}

ButtonInputType ButtonInputMonitor::getAction() {
  // Get the current pin value at the start
  bool value = getValue();
  unsigned long currentTime = millis();

  // Reset the current multi press counter if past the timeout
  if (currentTime - last_press_release > MULTI_CLICK_TIMEOUT && !isPressed) {
    int originalCurrentPresses = currentPresses;
    currentPresses = 0;
    if (originalCurrentPresses == 1) {
      logger.info("Returning Single Button Press");
      return ButtonInputType::ButtonPress;
    } else if (originalCurrentPresses == 2) {
      logger.info("Returning Double Button Press");
      return ButtonInputType::DoubleButtonPress;
    } else if (originalCurrentPresses == 3) {
      logger.info("Returning Triple Button Press");
      return ButtonInputType::TripleButtonPress;
    }
  }

  // If the monitor doesnt know it is pressed but it is
  if (!isPressed && value) {
    isPressed = true;
    last_press_start = currentTime;
    currentPresses += 1;

    logger.info("Started press " + String(currentPresses));

  } else if (isPressed && !value) {
    last_press_release = currentTime;
    isPressed = false;

    if (last_press_release - last_press_start > LONG_PRESS_DURATION && currentPresses == 1) {
      logger.info("Ended long press");
      currentPresses = 0;
      return ButtonInputType::LongPress;
    } else {
      logger.info("Ended press");
    }
  }
}