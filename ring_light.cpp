#include "Adafruit_NeoPixel.h"
#include "Arduino.h"
#include "api/Common.h"
#include "ring_light.h"
#include "logger.h"

RingLight::RingLight(uint8_t ledCount, uint8_t diPin, neoPixelType type) {
  Serial.println("Initializing RingLight");
  m_ledCount = ledCount;
  m_diPin = diPin;
  m_type = type;
  m_ring = Adafruit_NeoPixel(m_ledCount, m_diPin, m_type);
  m_ring.begin();
  m_ring.clear();
  Serial.println("Initialized RingLight");
}


void RingLight::setLightMode(DeviceState state) {
  Serial.println("Setting Light Mode");
  m_startTime = millis();
  m_ring.clear();
  m_state = state;
  update();
};

void RingLight::setBrightness(uint8_t brightness) {
  logger.info("Setting brightness to: " + String(brightness));
  m_ring.setBrightness(brightness);
}

void RingLight::updateLightModePairing() {
  m_ring.setBrightness(128);
  unsigned long currentSecond = (int)(millis() - m_startTime) / 100;
  Serial.println(currentSecond);
  m_ring.clear();

  /*
  // 2 equally spaced lights
  for (int i = 0; i < m_ledCount; i++) {
    if (i == currentSecond % m_ledCount) {
      m_ring.setPixelColor(i, m_ring.Color(0, 0, 255));
      m_ring.setPixelColor(((int)i + m_ledCount / 2) % m_ledCount, m_ring.Color(0, 255, 0));
    }
  }
  m_ring.show();
  */

  // 4 equally spaced lights
  for (int i = 0; i < m_ledCount; i++) {
    if (i == currentSecond % m_ledCount) {
      m_ring.setPixelColor(i, m_ring.Color(0, 0, 255));
      m_ring.setPixelColor(((int)i + m_ledCount / 4) % m_ledCount, m_ring.Color(0, 255, 255));
      m_ring.setPixelColor(((int)i + m_ledCount / 2) % m_ledCount, m_ring.Color(0, 255, 0));
      m_ring.setPixelColor(((int)i + 3 * m_ledCount / 4) % m_ledCount, m_ring.Color(255, 255, 0));
    }
  }
  m_ring.show();

  /* Alternate back and forth
  for (int i = 0; i < m_ledCount; i++) {
    if ((currentSecond % 2 == 0 && i % 2 == 0) || (currentSecond % 2 == 1 && i % 2 == 1)) {
      m_ring.setPixelColor(i, m_ring.Color(0, 0, 255));
    } else {
      m_ring.setPixelColor(i, m_ring.Color(0, 255, 255));
    }
  }
  m_ring.show();
  */
}

void RingLight::updateLightModeTimer(double pct) {
  // Restrict pct to be between 0..1
  pct = std::max(0.0, std::min(pct, 1.0));
  int filled = (int)(pct * m_ledCount);
  int deltaTime = ((int)(millis() - m_startTime)) / 200 - 60;
  logger.debug(String(pct) + " " + String(filled) + " " + String(deltaTime));

  uint32_t color;
  if (pct < 0.75) {
    color = Adafruit_NeoPixel::Color(0, 255, 0);
  } else if (pct < 0.9) {
    color = Adafruit_NeoPixel::Color(255, 255, 0);
  } else if (pct < 1.0) {
    color = Adafruit_NeoPixel::Color(255, 0, 0);
  } else if (deltaTime > 10) {
    color = Adafruit_NeoPixel::Color(0, 0, 0);
  } else {
    if (deltaTime % 2 == 0) {
      color = Adafruit_NeoPixel::Color(255, 0, 0);
    } else {
      color = Adafruit_NeoPixel::Color(0, 0, 0);
    }
  }

  for (int i = 0; i < filled; i++) {
    m_ring.setPixelColor(i, color);
  }
}


void RingLight::update() {
  // logger.info("Updating Ring Light Color");
  m_ring.clear();
  switch (m_state) {
    case DeviceState::Off:
      break;
    case DeviceState::Pairing:
      updateLightModePairing();
      break;
    case DeviceState::Timer:
      double pct = ((double)millis() - m_startTime) / m_timerData.totalTime;
      updateLightModeTimer(pct);
      break;
  };
  m_ring.show();
}

void RingLight::updateTimerData(struct TimerData data) {
  m_timerData = data;
}
