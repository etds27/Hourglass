#include "seven_segment.h"
#include "Arduino.h"
#include "logger.h"

SevenSegmentDisplay::SevenSegmentDisplay(uint8_t aPin,
                                         uint8_t bPin,
                                         uint8_t cPin,
                                         uint8_t dPin,
                                         uint8_t ePin,
                                         uint8_t fPin,
                                         uint8_t gPin,
                                         uint8_t dpPin) {
  m_aPin = aPin;
  m_bPin = bPin;
  m_cPin = cPin;
  m_dPin = dPin;
  m_ePin = ePin;
  m_fPin = fPin;
  m_gPin = gPin;
  m_dpPin = dpPin;

  numberOfPins = 8;
  
  uint8_t pins[8] = { m_aPin, m_bPin, m_cPin, m_dPin, m_ePin, m_fPin, m_gPin, m_dpPin };
  memcpy(m_pins, pins, sizeof(pins));

  for (int i = 0; i < numberOfPins; i++) {
    pinMode(m_pins[i], OUTPUT);
  }
}

void SevenSegmentDisplay::set(uint8_t number, bool decimal) {
  if (number < 0) {
    logger.warning("Number less than 0");
    return;
  }
  if (number > 9) {
    logger.warning("Number greater than 9");
    return;
  }
  clear();

  if (number == 0 || number == 2 || number == 3 || number == 5 || number == 6 || number == 7 || number == 8 || number == 9) {
    logger.debug("Setting A Pin");
    digitalWrite(m_aPin, HIGH);
  }
  if (number == 0 || number == 1 || number == 2 || number == 3 || number == 4 || number == 7 || number == 8 || number == 9) {
    logger.debug("Setting B Pin");
    digitalWrite(m_bPin, HIGH);
  }
  if (number == 0 || number == 1 || number == 3 || number == 4 || number == 5 || number == 6 || number == 7 || number == 8 || number == 9) {
    logger.debug("Setting C Pin");
    digitalWrite(m_cPin, HIGH);
  }
  if (number == 0 || number == 2 || number == 3 || number == 5 || number == 6 || number == 8) {
    logger.debug("Setting D Pin");
    digitalWrite(m_dPin, HIGH);
  }
  if (number == 0 || number == 2 || number == 6 || number == 8) {
    logger.debug("Setting E Pin");
    digitalWrite(m_ePin, HIGH);
  }
  if (number == 0 || number == 4 || number == 5 || number == 6 || number == 8 || number == 9) {
    logger.debug("Setting F Pin");
    digitalWrite(m_fPin, HIGH);
  }
  if (number == 2 || number == 3 || number == 4 || number == 5 || number == 6 || number == 8 || number == 9) {
    logger.debug("Setting G Pin");
    digitalWrite(m_gPin, HIGH);
  }
  if (decimal) {
    logger.debug("Setting DP Pin");
    digitalWrite(m_dpPin, HIGH);
  }
}

void SevenSegmentDisplay::setAll() {
  for (int i = 0; i < numberOfPins; i++) {
    logger.debug("Setting ", i, " (", m_pins[i], ") Pin");
    digitalWrite(m_pins[i], HIGH);
  }
}

void SevenSegmentDisplay::clear() {
  for (int i = 0; i < numberOfPins; i++) {
    digitalWrite(m_pins[i], LOW);
  }
}