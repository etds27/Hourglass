#pragma once
#include "input_monitor.h"
#include "ble_interface.h"
#include "light_interface.h"



#include "device_state.h"
#include "constants.h"

class DeviceManager {
public:
  DeviceManager();

  // Device Name
  char* getDeviceName();

  // Reads the device name from the EEPROM where it is stored.
  char* readDeviceName(char* out);
  void writeDeviceName(char* deviceName, uint8_t length);


  void start();

  // This is the main loop of the device
  void update();

private:
  DeviceState m_deviceState;
  bool m_started = false;
  bool m_isGameActive = false;
  unsigned long lastUpdate;
  unsigned long lastReadOut;
  BLEInterface* m_interface;
  ButtonInputMonitor* m_buttonMonitor;
  char* m_deviceName;
  LightInterface* m_ring;

  void processGameState();
  void updateRing();

  void setWaitingForConnection();
  void setAwaitGameStart();
  void updateAwaitingGameStartData();

  void setSkipped();
  void unsetSkipped();

  void updateTimer();
  void updateRingMode();

  bool isActiveTurn();

  void startTurn();
  void endTurn();

  void setTurnSequenceMode();
  void updateTurnSequence();

};