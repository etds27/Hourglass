#include <stdint.h>

class SevenSegmentDisplay {
private:
  uint8_t m_aPin;
  uint8_t m_bPin;
  uint8_t m_cPin;
  uint8_t m_dPin;
  uint8_t m_ePin;
  uint8_t m_fPin;
  uint8_t m_gPin;
  uint8_t m_dpPin;

  uint8_t numberOfPins;
  uint8_t m_pins[8];

public:
  SevenSegmentDisplay(uint8_t aPin, uint8_t bPin, uint8_t cPin, uint8_t dPin, uint8_t ePin, uint8_t fPin, uint8_t gPin, uint8_t dpPin);
  void set(uint8_t number, bool decimal = false);
  void setAll();
  void clear();
};