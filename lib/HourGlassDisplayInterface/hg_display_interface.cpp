#include "hg_display_interface.h"
#include "logger.h"
#include "constants.h"

#ifdef SIMULATOR
#include "simulator_tools.h"
#endif

namespace
{
    const LogString loggerTag = "HGDisplayInterface";
}

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

void HGDisplayInterface::setAbsoluteOrientation(bool orientation)
{
  m_absoluteOrientation = orientation;
}

void HGDisplayInterface::setDisplayMode(DeviceState::State state)
{
  logger.debug(loggerTag, "Setting Light Mode");
  m_startTime = millis();
  clear();
  m_state = state;
  update();
};

void HGDisplayInterface::update(bool force)
{
  // Exit if attempting to redraw display before reaching the refresh rate
  if (millis() - m_lastUpdate < m_refreshRate && !force)
  {
    return;
  }

  if (getClearBeforeUpdate())
  {
    clear();
  }

  switch (m_state)
  {
  case DeviceState::State::Off:
    clear();
    break;
  case DeviceState::State::AwaitingConnection:
    updateLightModeAwaitConnection();
    break;
  case DeviceState::State::ActiveTurnEnforced:
    updateLightModeActiveTurnTimer();
    break;
  case DeviceState::State::ActiveTurnNotEnforced:
    updateLightModeActiveTurnNoTimer();
    break;
  case DeviceState::State::Skipped:
    updateLightModeSkipped();
    break;
  case DeviceState::State::AwaitingTurn:
    updateLightModeTurnSequence();
    break;
  case DeviceState::State::AwaitingGameStart:
    updateLightModeAwaitGameStart();
    break;
  case DeviceState::State::Paused:
    updateGamePaused();
    break;
  case DeviceState::State::Debug:
    updateGameDebug();
    break;
  case DeviceState::State::BuzzerAwaitingBuzz:
    updateLightModeBuzzerAwaitingBuzz();
    break;
  case DeviceState::State::BuzzerAwaitingBuzzTimed:
    updateLightModeBuzzerAwaitingBuzzTimed();
    break;
  case DeviceState::State::BuzzerWinnerPeriod:
    updateLightModeWinnerPeriod();
    break;
  case DeviceState::State::BuzzerWinnerPeriodTimed:
    updateLightModeWinnerPeriodTimed();
    break;
  case DeviceState::State::BuzzerResults:
    updateLightModeBuzzerResults();
    break;
  case DeviceState::State::BuzzerAwaitingTurnStart:
    updateLightModeAwaitTurnStart();
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

void HGDisplayInterface::updateAwaitingGameStartData(const struct GameStartData data)
{
  m_gameStartData = data;
}

void HGDisplayInterface::updateBuzzerResultsData(BuzzerResultsData data)
{
  m_buzzerResultsData = data;
}

bool HGDisplayInterface::getClearBeforeUpdate() const
{
  return m_clearBeforeUpdate;
}

void HGDisplayInterface::updatePrimaryColor(uint32_t color)
{
  logger.info(loggerTag, ": Updating primary color to ", color);
  m_primaryColor = color;
}

void HGDisplayInterface::updateAccentColor(uint32_t accentColor)
{
  logger.info(loggerTag, ": Updating accent color to ", accentColor);
  m_primaryColor = accentColor;
}


