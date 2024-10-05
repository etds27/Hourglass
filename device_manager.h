#pragma once
#include "input_monitor.h"
#include "ble_interface.h"

enum class DeviceState {
  Off,
  Unpaired,
  Pairing,
  Timer,
  TurnSequence
};

class DeviceManager {
  public:
    DeviceManager();

    // Device Name
    char* getDeviceName();
    char* readDeviceName();
    void writeDeviceName(const char* deviceName);

    bool getTurn();
    void updateTurn(bool turn);
    void endTurn();

    // Updates the device state based on the current state and the input action provided
    void handleButtonInput(ButtonInputType inputType);

  private:
    void handleSingleButtonPress();
    bool m_isTurn = false;
    BLEInterface* m_interface;
    char* m_deviceName[];
};