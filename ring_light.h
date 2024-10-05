#include <Adafruit_NeoPixel.h>
#include "device_manager.h"

struct TimerData {
  int totalTime;
  int remainingTme;
};


// Manage the display state of Neopixel devices
// 
// Contains commmon lighting modes used for representing device states
class RingLight {
private:
  DeviceState m_state = DeviceState::Off;
  uint8_t m_ledCount;
  uint8_t m_diPin;
  unsigned long m_startTime;
  struct TimerData m_timerData;
  Adafruit_NeoPixel m_ring;
  neoPixelType m_type;

  void updateLightModePairing();
  void updateLightModeTimer(double pct);

public:
  RingLight(uint8_t ledCount = 16, uint8_t diPin = 0, neoPixelType type = NEO_GRB + NEO_KHZ800);

  // Sets the ring light to the appropriate lighting mode associated with the provided device state
  void setLightMode(DeviceState state);
  
  void setBrightness(uint8_t brightness);

  void updateTimerProperties(TimerData data);

  // Updates the current light display according to the known device state and state properties
  void update();

  // Update the data for the timer prior to updating the ring light
  void updateTimerData(struct TimerData data);
};

