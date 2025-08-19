#pragma once
#include "input_monitor.h"
#include "input_interface.h"
#include "hg_central_interface.h"
#include "hg_display_interface.h"
#include "device_state.h"
#include "constants.h"
#include "hg_display_manager.h"

// Maintains the device state while it is powered on
class DeviceManager
{
public:
  DeviceManager(HourglassDisplayManager *displayManager);
  ~DeviceManager();
  // Device Name
  char *getDeviceName();

  // Reads the device name from the EEPROM where it is stored.
  char *readDeviceName(char *out);



  // Initializes the bluetooth peripheral interface and sets up the light for output
  void start();

  // This is the main loop of the device.
  // Handles retrieving BLE information, processing button inputs, determining game state, and updating the light
  void update();

private:
  HourglassDisplayManager *m_displayManager;
  DeviceState::State m_deviceState;

  unsigned long m_lastTurnStart;

  bool updateCommandedDeviceState();

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

  /// @brief Last time the device was disconnected from the central
  /// Used to display AwaitingConnection state after connection has been made
  unsigned long m_lastDisconnection;

  // Interface for retrieving information from the central device
  HGCentralInterface *m_interface;

  // Monitor for button to control turn status
  ButtonInputMonitor *m_buttonMonitor;
  InputInterface *m_inputInterface;

  // Device name
  char *m_deviceName;

  // Display output for current game state
  // HGDisplayInterface *m_displayInterface;

  // Flag indicating the current color blind status
  bool m_colorBlindMode = false;

  /// @brief Flag indicating if the turn sequence should be oriented by the turn order, or by the device's player
  /// If true, the 1st player in the player order will be represented in the top right
  /// If false, the device's player will be represented in the top right
  bool m_absoluteOrientation = true;

  /// @brief Toggle the device display orientation
  void toggleDeviceOrientation();

  // Toggle the current colorblind state of the output
  void toggleColorBlindMode();

  // Prepare the device for shutdown and enter deep sleep
  void enterDeepSleep();

  // Determines the current game state based off current device state, and inputs
  void processGameState();

  // Update the display interface with the current game state
  void updateDisplay(bool force = false);
  
  void setWaitingForConnection();;
  void updateAwaitingGameStartData();

  void updateTimer();
  void updateDisplayMode();

  bool isActiveTurn();

  void onDeviceNameChanged(char *name);
  void onDeviceColorChanged(uint32_t color);
  void onDeviceAccentColorChanged(uint32_t accentColor);

  // Writes the device name to the EEPROM and updates the stored device name for the Device Manager
  #ifndef SIMULATOR
  void writeDeviceName(const char *deviceName, uint8_t length);
  void writeDeviceColor(uint32_t color);
  void writeDeviceAccentColor(uint32_t accentColor);
  #endif

  void updateDeviceName(const char *name);
  void updateDeviceColor(uint32_t color);
  void updateDeviceAccentColor(uint32_t accentColor);


  // Send the end of the turn to the central device
  void sendEndTurn();
  void updateTurnSequence();
};