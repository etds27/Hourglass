#include "hg_display_interface.h"
#include "logger.h"
#include "constants.h"


HGDisplayInterface::~HGDisplayInterface() {}

void HGDisplayInterface::setUp()
{
  begin();
  clear();
}

void HGDisplayInterface::setColorBlindMode(bool colorBlindMode)
{
  m_colorBlindMode = colorBlindMode;
}

void HGDisplayInterface::setDisplayMode(DeviceState state)
{
  logger.debug("Setting Light Mode");
  m_startTime = millis();
  clear();
  m_state = state;
  update();
};

void HGDisplayInterface::update(bool force)
{
  if (millis() - m_lastUpdate < RING_REFRESH_RATE && !force)
  {
    return;
  }
  // logger.info("Updating Ring Light Color");
  clear();
  switch (m_state)
  {
  case DeviceState::Off:
    clear();
    break;
  case DeviceState::AwaitingConnecion:
    updateLightModeAwaitConnection();
    break;
  case DeviceState::ActiveTurn:
    updateLightModeActiveTurn();
    break;
  case DeviceState::Skipped:
    updateLightModeSkipped();
    break;
  case DeviceState::AwaitingTurn:
    updateLightModeTurnSequence();
    break;
  case DeviceState::AwaitingGameStart:
    updateLightModeAwaitGameStart();
    break;
  case DeviceState::Paused:
    updateGamePaused();
    break;
  };
  // noInterrupts();
  show();
  // interrupts();
  m_lastUpdate = millis();
}

void HGDisplayInterface::updateTimerData(struct TimerData data)
{
  m_timerData = data;
}

void HGDisplayInterface::updateTurnSequenceData(struct TurnSequenceData data)
{
  m_turnSequenceData = data;
}

void HGDisplayInterface::updateAwaitingGameStartData(struct GameStartData data)
{
  m_gameStartData = data;
}
