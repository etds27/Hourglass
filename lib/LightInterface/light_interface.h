#pragma once
#include <stdint.h>
#include "constants.h"
#include "colors.h"
#include "device_state.h"

struct GameStartData
{
  int totalPlayers;
};

struct TimerData
{
  int totalTime;
  int elapsedTime;
};

struct TurnSequenceData
{
  int totalPlayers;
  int myPlayerIndex;
  int currentPlayerIndex;
};

class LightInterface
{
protected:
  DeviceState m_state = DeviceState::Off;

  uint8_t m_ledCount;
  uint8_t m_diPin;

  bool m_colorBlindMode = false;

  unsigned long m_startTime;
  struct TimerData m_timerData;
  struct TurnSequenceData m_turnSequenceData;
  struct GameStartData m_gameStartData;

  unsigned long m_lastUpdate;

  void updateLightModePairing();
  void updateLightModeTimer();
  void updateLightModeSkipped();
  void updateLightModeTurnSequence();
  void updateLightModeAwaitGameStart();
  void updateLightModeAwaitConnection();

  // Take a buffer of size (2, 4, 8) and populate a full 16 item buffer using the condensed buffer
  // The fill option will fill in the space between the
  void expandBuffer(const uint32_t *smallBuffer, uint32_t *fullBuffer, uint8_t size, bool fill = true);

  // Replicate the smaller buffer to fit into the full buffer
  void extendBuffer(const uint32_t *smallBuffer, uint32_t *fullBuffer, uint8_t size);

  // Rotate the full sized color buffer by some offset
  void offsetBuffer(uint32_t *buffer, uint8_t offset);

  void displayBuffer(uint32_t *buffer);

  void setUp();

public:
  LightInterface(const uint8_t ledCount = 16, const uint8_t diPin = 0);
  virtual ~LightInterface();

  // Sets the current colorblind mode for the ringlight
  void setColorBlindMode(bool colorBlindMode);

  // Sets the ring light to the appropriate lighting mode associated with the provided device state
  void setLightMode(DeviceState state);

  // Light ring mode for when the game is paused by the companion app
  void updateGamePaused();

  // Updates the current light display according to the known device state and state properties
  void update();

  // Update the data for the timer prior to updating the ring light
  void updateTimerData(struct TimerData data);
  void updateTurnSequenceData(struct TurnSequenceData data);
  void updateAwaitingGameStartData(struct GameStartData data);

  virtual void begin() = 0;
  virtual void clear() = 0;
  virtual void show() = 0;
  virtual void setBrightness(uint8_t brightness) = 0;
  virtual void setPixelColor(uint8_t i, uint32_t color) = 0;
};
