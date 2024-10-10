#include "light_interface.h"
#include "logger.h"

const int RING_REFRESH_RATE = 50;


LightInterface::LightInterface(const uint8_t ledCount, const uint8_t diPin) {
  logger.info("Initializing Light Interface");
  m_ledCount = ledCount;
  m_diPin = diPin;
  m_lastUpdate = millis();
  logger.info("Initialized Light Interface");
}

LightInterface::~LightInterface() {}


void LightInterface::setUp() {
  begin();
  clear();
}

void LightInterface::setLightMode(DeviceState state) {
  logger.debug("Setting Light Mode");
  m_startTime = millis();
  clear();
  setBrightness(DEFAULT_BRIGHTNESS);
  m_state = state;
  update();
};

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

void LightInterface::updateLightModeTimer(double pct) {
  // Restrict pct to be between 0..1
  pct = std::max(0.0, std::min(pct, 1.0));
  int filled = (int)(pct * m_ledCount);
  int deltaTime = ((int)(millis() - m_startTime)) / 200 - 60;
  uint32_t colorBuffer[16];

  uint32_t color;
  if (pct < 0.75) {
    color = GREEN;
  } else if (pct < 0.9) {
    color = YELLOW;
  } else if (pct < 1.0) {
    color = RED;
  } else if (deltaTime > 10) {
    color = BLACK;
  } else {
    if (deltaTime % 2 == 0) {
      color = RED;
    } else {
      color = BLACK;
    }
  }

  for (int i = 0; i < filled; i++) {
    colorBuffer[i] = color;
  }
  for (int i = filled; i < m_ledCount; i++) {
    colorBuffer[i] = BLACK;
  }
  displayBuffer(colorBuffer);
}

// Light mode for skipped should be a pulsing gray that changes based on brightness values
void LightInterface::updateLightModeSkipped() {
  int deltaTime = (int)(millis() - m_startTime);

  // Percentage through pulse sequence
  double pct = (deltaTime % SKIPPED_PULSE_DURATION) / (double)SKIPPED_PULSE_DURATION;
  pct = abs(pct - 0.5);
  uint8_t brightness = 2 * pct * (SKIPPED_MAX_BRIGHTNESS - SKIPPED_MIN_BRIGHTNESS) + SKIPPED_MIN_BRIGHTNESS;
  uint32_t colorBuffer[16];

  setBrightness(brightness);

  for (int i = 0; i < m_ledCount; i++) {
    colorBuffer[i] = SKIPPED_COLOR;
  }

  displayBuffer(colorBuffer);
}

void LightInterface::updateLightModeTurnSequence() {
  logger.debug("Total Players:  " + String(m_turnSequenceData.totalPlayers));
  logger.debug("My Player:      " + String(m_turnSequenceData.myPlayerIndex));
  logger.debug("Current Player: " + String(m_turnSequenceData.currentPlayerIndex));
  logger.debug("");
  uint32_t colorBuffer[16];

  for (int i = 0; i < m_ledCount; i++) {
    if (i < m_turnSequenceData.totalPlayers) {
      if (m_turnSequenceData.currentPlayerIndex == i) {
        colorBuffer[i] = CURRENT_PLAYER_COLOR;
      } else if (m_turnSequenceData.myPlayerIndex == i) {
        colorBuffer[i] = MY_PLAYER_COLOR;

      } else {
        colorBuffer[i] = OTHER_PLAYER_COLORS;
      }
    } else {
      colorBuffer[i] = BLACK;
    }
    displayBuffer(colorBuffer);
  }
}

void LightInterface::updateLightModeAwaitGameStart() {
  uint32_t colors[] = { AWAIT_GAME_COLOR_1, AWAIT_GAME_COLOR_2, AWAIT_GAME_COLOR_3, RED, PURPLE, ORANGE, WHITE, BLUE };
  uint32_t colorBuffer[16];
  int segments = max(m_gameStartData.totalPlayers, 1);
  int lengthSegment = m_ledCount / segments;
  int remainder = m_ledCount % segments;

  int currentIndex = 0;
  for (int currentSegment = 0; currentSegment < segments; currentSegment++) {
    for (int j = 0; j < lengthSegment; j++) {
      colorBuffer[currentIndex] = colors[currentSegment];
      currentIndex += 1;
    }
    if (currentSegment < remainder) {
      colorBuffer[currentIndex] = colors[currentSegment];
      currentIndex += 1;
    }
  }

  unsigned long currentSecond = (int)(millis() - m_startTime) / AWAIT_GAME_START_SPEED;
  uint8_t offset = currentSecond % m_ledCount;
  displayBuffer(colorBuffer, offset);
}

void LightInterface::updateLightModeAwaitConnection() {
  uint32_t colorBuffer[16] = {};

  unsigned long currentSecond = (int)(millis() - m_startTime) / AWAIT_CONNECTION_SPEED;

  int timeSlice = currentSecond % (m_ledCount * 2);
  int waxing = timeSlice / m_ledCount == 0;
  for (int i = 0; i < m_ledCount; i++) {
    if (waxing) {
      if (i <= timeSlice % m_ledCount) {
        colorBuffer[i] = AWAIT_CONNECTION_COLOR;
      }
    } else {
      if (i > timeSlice % m_ledCount) {
        colorBuffer[i] = AWAIT_CONNECTION_COLOR;
      }
    }
  }
  displayBuffer(colorBuffer);
}


void LightInterface::update() {
  if (millis() - m_lastUpdate < RING_REFRESH_RATE) {
    return;
  }
  // logger.info("Updating Ring Light Color");
  clear();
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
  //noInterrupts();
  show();
  // interrupts();
  m_lastUpdate = millis();
}


void LightInterface::displayBuffer(uint32_t* buffer, uint8_t offset) {
  for (int i = 0; i < m_ledCount; i++) {
    setPixelColor((i + offset) % m_ledCount, buffer[i]);
  }
}

void LightInterface::updateTimerData(struct TimerData data) {
  m_timerData = data;
}

void LightInterface::updateTurnSequenceData(struct TurnSequenceData data) {
  m_turnSequenceData = data;
}

void LightInterface::updateAwaitingGameStartData(struct GameStartData data) {
  m_gameStartData = data;
}
