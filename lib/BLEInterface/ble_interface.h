#pragma once
#include <ArduinoBLE.h>
#include "hg_central_interface.h"

class BLEInterface : public HGCentralInterface
{
public:
  BLEInterface(char *deviceName);

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


  void setService();

  void poll();

private:
  static String serviceIds[];

  BLEService *m_service;
  // BLEIntCharacteristic* m_dummy;
  BLEIntCharacteristic *m_numberOfPlayers;
  BLEIntCharacteristic *m_currentPlayer;
  BLEIntCharacteristic *m_timer;
  BLEIntCharacteristic *m_elapsedTime;
  BLEIntCharacteristic *m_myPlayerNumber;
  BLEBoolCharacteristic *m_myTurn;
  BLEBoolCharacteristic *m_skipped;
  BLEBoolCharacteristic *m_gameActive;
  BLEBoolCharacteristic *m_gamePaused;
  BLEBoolCharacteristic *m_enforceTurnTimer;

  BLEDescriptor *m_activeTurnDescriptor;
  BLEDescriptor *m_skippedDescriptor;
};