#pragma once

#ifndef SIMULATOR
#include <ArduinoBLE.h>
#include "hg_central_interface.h"
#include "device_state.h"

class BLEInterface : public HGCentralInterface
{
public:
  BLEInterface(char *deviceName);

    void readData();
    bool isConnected();
    bool isTurn();
    void endTurn();
    void toggleSkippedState();


    int getTimer();
    int getElapsedTime() override;

    int getCurrentPlayer();
    int getTotalPlayers();

    // Gets the current device's player index
    int getMyPlayer();

    // Check if a game is active
    bool isGameActive();

    // Check if a game is paused
    bool isGamePaused();

    // Check if the turn timer should be enforced
    bool isTurnTimerEnforced();

    DeviceState::State getCommandedDeviceState();


  void setService();

  void poll();

private:
  static String serviceIds[];

  BLEService *m_service;
  BLEIntCharacteristic *m_numberOfPlayers;
  BLEIntCharacteristic *m_currentPlayer;
  BLEIntCharacteristic *m_timer;
  BLEIntCharacteristic *m_elapsedTime;
  BLEIntCharacteristic *m_myPlayerNumber;
  BLEIntCharacteristic *m_gameState;
  BLEBoolCharacteristic *m_endTurn;
  BLEBoolCharacteristic *m_toggleSkip;
  BLEByteCharacteristic *m_skippedPlayers;

  BLEDescriptor *m_activeTurnDescriptor;
  BLEDescriptor *m_skippedDescriptor;
};

#endif