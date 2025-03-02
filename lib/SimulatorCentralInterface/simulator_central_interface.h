#pragma once
#include "hg_central_interface.h"
#include <stdint.h>

class SimulatorCentralInterface : public HGCentralInterface
{
public:
  SimulatorCentralInterface(char *deviceName);

  void readData();
  bool isConnected();
  bool isTurn();
  void endTurn();

  int getTimer();
  int getElapsedTime();
  int getCurrentPlayer();
  int getTotalPlayers();

  // Gets the current device's player index
  int getMyPlayer();

  // Gets the current status of if the device is being skipped in the queue
  bool isSkipped();
  // Lets the device tell the server that it should be skipped for future turns
  void setSkipped();
  // Lets the device tell the server that it should not be skipped for future turns
  void unsetSkipped();

  // Check if a game is active
  bool isGameActive();

  // Check if a game is paused
  bool isGamePaused();

  // Check if the turn timer should be enforced
  bool isTurnTimerEnforced();

  uint16_t getSkippedPlayers();

  void setService();

  void poll();

  void toggleSkippedState();
  DeviceState::State getCommandedDeviceState();

private:
  uint8_t m_numberOfPlayers;
  uint8_t m_currentPlayer;
  uint8_t m_timer;
  uint8_t m_elapsedTime;
  uint8_t m_myPlayerNumber;
  uint16_t m_skippedPlayers;
  bool m_myTurn;
  bool m_skipped;
  bool m_gameActive;
  bool m_gamePaused;
  bool m_enforceTurnTimer;
};