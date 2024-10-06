#include "BLECharacteristic.h"
#pragma once
#include <ArduinoBLE.h>

class BLEInterface {
  public:
    BLEInterface(char* deviceName);
    void setService(uint8_t serviceIndex);
    void readData();
    bool isConnected();
    bool isTurn();
    void endTurn();
    int getTimer();
    int getCurrentPlayer();
    int getTotalPlayers();

    // Gets the current device's player index
    int getMyPlayer();

    // Gets the current status of if the device is being skipped in the queue
    bool getSkipped();
    // Lets the device tell the server that it should be skipped for future turns
    void setSkipped();
    // Lets the device tell the server that it should not be skipped for future turns
    void unsetSkipped();

    // Check if a game is active;
    bool isGameActive();

  private:
    static String serviceIds[];
    char* m_deviceName;
    BLEService* m_service;
    BLEIntCharacteristic* m_numberOfPlayers;
    BLEIntCharacteristic* m_currentPlayer;
    BLEIntCharacteristic* m_timer;
    BLEIntCharacteristic* m_myPlayerNumber;
    BLEBoolCharacteristic* m_myTurn;
    BLEBoolCharacteristic* m_skipped;
    BLEBoolCharacteristic* m_gameActive;
};