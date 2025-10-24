#pragma once
#include "input_monitor.h"
#include "input_interface.h"
#include "hg_central_interface.h"
#include "hg_display_interface.h"
#include "device_state.h"
#include "constants.h"
#include "hg_display_manager.h"

/// @brief Manages the overall state and logic of the Hourglass device.
/// This class is the central coordinator, responsible for initializing hardware,
/// managing BLE communication, processing user input, and controlling the display
/// based on the current game state commanded by the central device.
class DeviceManager
{
public:
  /// @brief Constructs a new DeviceManager.
  /// @param displayManager The manager for all display interfaces.
  DeviceManager(HourglassDisplayManager *displayManager, InputInterface *inputInterface = nullptr, HGCentralInterface *centralInterface = nullptr);

  /// @brief Destroys the DeviceManager.
  ~DeviceManager();

  /// @brief Gets the current device name.
  /// @return A pointer to the character array holding the device name.
  char *getDeviceName();

  /// @brief Reads the device name from persistent storage (EEPROM).
  /// @param out Pointer to a character array to store the name.
  /// @return A pointer to the output character array.
  char *readDeviceName(char *out);

#ifndef SIMULATOR
  /// @brief Writes the device name to persistent storage (EEPROM).
  /// @param deviceName The new name to write.
  /// @param length The length of the new name.
  void writeDeviceName(const char *deviceName, uint8_t length);

  /// @brief Writes a color configuration to persistent storage (EEPROM).
  /// @param config The color configuration to write.
  void writeDeviceColorConfig(ColorConfig config, DeviceState::State configState);
#endif

  /// @brief Initializes the device, including BLE services and display interfaces.
  /// Sets the initial state to AwaitingConnection.
  void start();

  /// @brief The main update loop for the device.
  /// This should be called repeatedly. It processes game state changes,
  /// handles user input, and updates the display.
  void update();

private:
  /// @brief Manages all attached display interfaces.
  HourglassDisplayManager *m_displayManager;

  /// @brief The current state of the device.
  DeviceState::State m_deviceState;

  /// @brief Timestamp of the last turn start.
  unsigned long m_lastTurnStart;

  /// @brief Fetches the latest state from the central and checks if it has changed.
  /// @return True if the device state has changed, false otherwise.
  bool updateCommandedDeviceState();

  /// @brief Flag indicating if the `start()` method has been called.
  bool m_started = false;

  /// @brief Flag indicating if a game is currently active.
  bool m_isGameActive = false;

  /// @brief Timestamp of the last call to `update()`. Used for calculating delta time.
  unsigned long m_lastUpdate;

  /// @brief Timestamp of the last debug printout to the serial interface.
  unsigned long m_lastReadOut;

  /// @brief Timestamp of the last time the device was connected to a central.
  /// Used to determine eligibility for deep sleep.
  unsigned long m_lastConnection;

  /// @brief Last time the device was disconnected from the central
  /// Used to ensure characteristics are discovered before processing state changes.
  unsigned long m_lastDisconnection;

  /// @brief Interface for communication with the central device (e.g., BLE).
  HGCentralInterface *m_interface;

  /// @brief Monitors the physical button for presses and other actions.
  ButtonInputMonitor *m_buttonMonitor;

  /// @brief Low-level interface for reading button input.
  InputInterface *m_inputInterface;

  /// @brief The name of this Hourglass device.
  char *m_deviceName;

  /// @brief Flag indicating if color-blind friendly colors should be used.
  bool m_colorBlindMode = false;

  /// @brief Flag indicating if the turn sequence should be oriented by the turn order, or by the device's player
  /// If true (absolute), the 1st player is at the top.
  /// If false (player-relative), this device's player is at the top.
  bool m_absoluteOrientation = true;

  /// @brief Toggle the device display orientation
  void toggleDeviceOrientation();

  /// @brief Toggles the color-blind mode for the display.
  void toggleColorBlindMode();

  /// @brief Puts the device into a low-power deep sleep state.
  void enterDeepSleep();

  /// @brief Main logic for processing the current game state and handling transitions.
  void processGameState();

  /// @brief Updates all display interfaces based on the current state.
  /// @param force If true, forces a redraw even if the refresh rate hasn't elapsed.
  void updateDisplay(bool force = false);

  /// @brief Updates the display when the device is in `ConfigurationMode`.
  /// @param force If true, forces a redraw.
  void updateConfigDisplay(bool force = false);

  /// @brief Sets the device state to `AwaitingConnection` and updates the display.
  void setWaitingForConnection();

  /// @brief Updates the display manager with data needed for the `AwaitingGameStart` state.
  void updateAwaitingGameStartData();

  /// @brief Updates the display manager with the latest timer data.
  void updateTimer();

  /// @brief Sets the display mode on the display manager.
  void updateDisplayMode();

  /// @brief Checks if the current state is an active turn for this device.
  /// @return True if it is this device's turn, false otherwise.
  bool isActiveTurn();

  /// @brief Callback for when the device name is changed via BLE. (No-op)
  void onDeviceNameChanged(char *name);
  /// @brief Callback triggered to commit a device name change from BLE.
  void onDeviceNameWriteChanged(bool write);
  /// @brief Callback for when a color config is previewed via BLE.
  void onDeviceColorConfigChanged(ColorConfig config);
  /// @brief Callback triggered to commit a color config change from BLE.
  void onDeviceColorConfigWriteChanged(bool write);
  /// @brief Callback for when the state to be configured is changed via BLE.
  void onDeviceColorConfigStateChanged(DeviceState::State state);
  /// @brief The state currently being configured in `ConfigurationMode`.
  DeviceState::State m_configState = DeviceState::State::Off;

  /// @brief Notifies the central device that this device's turn has ended.
  void sendEndTurn();
  /// @brief Updates the display manager with data for the turn sequence display.
  void updateTurnSequence();

  /// @brief Registers all necessary callbacks with the central interface.
  void registerCallbacks();
};