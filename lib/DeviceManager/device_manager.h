#pragma once
#include "input_monitor.h"
#include "hg_central_interface.h"
#include "hg_display_interface.h"
#include "device_state.h"
#include "constants.h"
#include "TFT_eSPI.h"

// Maintains the device state while it is powered on
class DeviceManager
{
public:
#if DISPLAY_TYPE == 2
  DeviceManager(TFT_eSPI* tft);
#else
  DeviceManager();
#endif
  // Device Name
  char *getDeviceName();

  // Reads the device name from the EEPROM where it is stored.
  char *readDeviceName(char *out);

  // Writes the device name to the EEPROM and updates the stored device name for the Device Manager
  void writeDeviceName(char *deviceName, uint8_t length);

  // Initializes the bluetooth peripheral interface and sets up the light for output
  void start();

  // This is the main loop of the device.
  // Handles retrieving BLE information, processing button inputs, determining game state, and updating the light
  void update();

private:
  DeviceState m_deviceState;

  unsigned long m_lastTurnStart;

  // Flag indicating that the `start()` has been called
  bool m_started = false;

  // Flag indicating that a game is currently active
  bool m_isGameActive = false;

  // Last time the `update()` was called
  // Comparing this to the current `millis()` gives the devices refresh rate
  unsigned long m_lastUpdate;

  // Last time the BLE information was printed to the Serial interface
  unsigned long m_lastReadOut;

  // Last time the device was connected to the central
  // Used to determine deep sleep eligibility
  unsigned long m_lastConnection;

  // Interface for retrieving information from the central device
  HGCentralInterface *m_interface;

  // Monitor for button to control turn status
  ButtonInputMonitor *m_buttonMonitor;

  // Device name
  char *m_deviceName;

  // Display output for current game state
  HGDisplayInterface *m_displayInterface;

  // Flag indicating the current color blind status
  bool m_colorBlindMode = false;

  // Toggle the current colorblind state of the output
  void toggleColorBlindMode();

  // Prepare the device for shutdown and enter deep sleep
  void enterDeepSleep();

  // Determines the current game state based off current device state, and inputs
  void processGameState();

  // Update the display interface with the current game state
  void updateRing(bool force = false);
  
  void setWaitingForConnection();
  void setGamePaused();
  void setAwaitGameStart();
  void updateAwaitingGameStartData();

  void setSkipped();
  void unsetSkipped();

  void updateTimer();
  void updateRingMode();

  bool isActiveTurn();

  void startTurn();

  // Send the end of the turn to the central device
  void sendEndTurn();
  void endTurn();

  void setTurnSequenceMode();
  void updateTurnSequence();
};