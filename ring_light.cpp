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
  logger.debug("Setting Light Mode");
  m_startTime = millis();
  m_ring.clear();
  setBrightness(DEFAULT_BRIGHTNESS);
  m_state = state;
  update();
};

void RingLight::setBrightness(uint8_t brightness) {
  logger.debug("Setting brightness to: " + String(brightness));
  m_ring.setBrightness(brightness);
}

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

// Light mode for skipped should be a pulsing gray that changes based on brightness values
void RingLight::updateLightModeSkipped() {
  int deltaTime = (int)(millis() - m_startTime);

  // Percentage through pulse sequence
  double pct = (deltaTime % SKIPPED_PULSE_DURATION) / (double)SKIPPED_PULSE_DURATION;
  pct = abs(pct - 0.5);

  uint8_t brightness = 2 * pct * (SKIPPED_MAX_BRIGHTNESS - SKIPPED_MIN_BRIGHTNESS) + SKIPPED_MIN_BRIGHTNESS;
  logger.info("deltaTime:  " + String(deltaTime));
  logger.info("pct:        " + String(pct));
  logger.info("Brightness: " + String(brightness));
  setBrightness(brightness);

  for (int i = 0; i < m_ledCount; i++) {
    m_ring.setPixelColor(i, SKIPPED_COLOR);
  }
}

void RingLight::updateLightModeTurnSequence() {
  logger.debug("Total Players:  " + String(m_turnSequenceData.totalPlayers));
  logger.debug("My Player:      " + String(m_turnSequenceData.myPlayerIndex));
  logger.debug("Current Player: " + String(m_turnSequenceData.currentPlayerIndex));
  logger.debug("");
  for (int i = 0; i < m_ledCount; i++) {
    if (i < m_turnSequenceData.totalPlayers) {
      if (m_turnSequenceData.currentPlayerIndex == i) {
        m_ring.setPixelColor(i, CURRENT_PLAYER_COLOR);
      } else if (m_turnSequenceData.myPlayerIndex == i) {
        m_ring.setPixelColor(i, MY_PLAYER_COLOR);
      } else {
        m_ring.setPixelColor(i, OTHER_PLAYER_COLORS);
      }
    } else {
      m_ring.setPixelColor(i, 0, 0, 0);
    }
  }
}

void RingLight::updateLightModeAwaitGameStart() {
  unsigned long currentSecond = (int)(millis() - m_startTime) / AWAIT_GAME_START_SPEED;

  // 4 equally spaced lights
  for (int i = 0; i < m_ledCount; i++) {
    if (i == currentSecond % m_ledCount) {
      m_ring.setPixelColor(i, AWAIT_GAME_COLOR_1);
      m_ring.setPixelColor(((int)i + m_ledCount / 4) % m_ledCount, AWAIT_GAME_COLOR_2);
      m_ring.setPixelColor(((int)i + m_ledCount / 2) % m_ledCount, AWAIT_GAME_COLOR_3);
      m_ring.setPixelColor(((int)i + 3 * m_ledCount / 4) % m_ledCount, AWAIT_GAME_COLOR_4);
    }
  }
  m_ring.show();
}

void RingLight::updateLightModeAwaitConnection() {
  unsigned long currentSecond = (int)(millis() - m_startTime) / AWAIT_CONNECTION_SPEED;

  int timeSlice = currentSecond % (m_ledCount * 2);
  int waxing = timeSlice / m_ledCount == 0;
  for (int i = 0; i < m_ledCount * 2; i++) {
    if (waxing) {
      if (i <= timeSlice % m_ledCount) {
        m_ring.setPixelColor(i, AWAIT_CONNECTION_COLOR);
      }
    } else {
      if (i > timeSlice % m_ledCount) {
        m_ring.setPixelColor(i, AWAIT_CONNECTION_COLOR);
      }
    }
  }
}


void RingLight::update() {
    // logger.info("Updating Ring Light Color");
    m_ring.clear();
    switch (m_state) {
      case DeviceState::Off:
        break;
      case DeviceState::AwaitingConnecion:
        updateLightModeAwaitConnection();
        break;
      case DeviceState::ActiveTurn:
        {
          double pct = ((double)millis() - m_startTime) / m_timerData.totalTime;
          updateLightModeTimer(pct);
          break;
        }
      case DeviceState::Skipped:
        updateLightModeSkipped();
        break;
      case DeviceState::AwaitingTurn:
        updateLightModeTurnSequence();
        break;
      case DeviceState::AwaitingGameStart:
        updateLightModeAwaitGameStart();
        break;
    };
    m_ring.show();
}

void RingLight::updateTimerData(struct TimerData data) {
    m_timerData = data;
}

void RingLight::updateTurnSequenceData(struct TurnSequenceData data) {
    m_turnSequenceData = data;
}
