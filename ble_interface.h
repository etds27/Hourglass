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
  private:
    static String serviceIds[];
    char* m_deviceName;
    BLEService* m_service;
    BLEBoolCharacteristic* m_endTurn;
    BLEIntCharacteristic* m_numberOfPlayers;
    BLEIntCharacteristic* m_currentPlayer;
    BLEIntCharacteristic* m_timer;
    BLEIntCharacteristic* m_myPlayerNumber;
    BLEBoolCharacteristic* m_myTurn;
};