#pragma once

#ifndef SIMULATOR
#include <ArduinoBLE.h>
#include "hg_central_interface.h"
#include "device_state.h"
#include <stdint.h>
#include "device_config.h"

class BLEInterface : public HGCentralInterface
{
public:
  BLEInterface(char *deviceName);
  void sendDeviceName(const char *name);
  void sendDeviceColorConfig(ColorConfig config);

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

  // Config
  ColorConfig readColorConfig();
  void getDeviceName(char *out, uint8_t length);
  DeviceState::State getDeviceColorConfigState();

  // Check if a game is active
  bool isGameActive();

  // Check if a game is paused
  bool isGamePaused();

  // Check if the turn timer should be enforced
  bool isTurnTimerEnforced();

  uint16_t getSkippedPlayers();

  DeviceState::State getCommandedDeviceState();

  void setService();

  void poll() override;

private:
  /// Used to store the singleton instance of the BLEInterface.
  /// This will allow us to access the BLEInterface from the static BLE callbacks.
  static BLEInterface *instance;

  /// @brief Callback for when the device name changes.
  /// This will be called when the device name characteristic is written to.
  /// @param central The BLE device that wrote the characteristic
  /// @param characteristic The characteristic that was written to
  /// @note This callback is static so it can be used as a BLE event handler.
  static void onDeviceNameChanged(BLEDevice central, BLECharacteristic characteristic);

  /// @brief Callback for when the device name write status changes.
  /// This is triggered when the central device writes to the `DEVICE_NAME_WRITE_UUID` characteristic,
  /// signaling that the peripheral should read the new device name from the `DEVICE_NAME_UUID` characteristic.
  /// @param central The BLE device that wrote the characteristic.
  /// @param characteristic The characteristic that was written to.
  /// @note This callback is static so it can be used as a BLE event handler.
  static void onDeviceNameWriteChanged(BLEDevice central, BLECharacteristic characteristic);

  /// @brief Callback for when the device color configuration changes.
  /// This is triggered when the central device writes a new `ColorConfig` to the `DEVICE_COLOR_CONFIG_UUID` characteristic.
  /// @param central The BLE device that wrote the characteristic.
  /// @param characteristic The characteristic that was written to.
  /// @note This callback is static so it can be used as a BLE event handler.
  static void onDeviceColorConfigChanged(BLEDevice central, BLECharacteristic characteristic);

  /// @brief Callback for when the color configuration state changes.
  /// This is triggered when the central device writes a `DeviceState::State` to the `DEVICE_COLOR_CONFIG_STATE_UUID`
  /// characteristic, indicating which state's color configuration is being previewed.
  /// @param central The BLE device that wrote the characteristic.
  /// @param characteristic The characteristic that was written to.
  /// @note This callback is static so it can be used as a BLE event handler.
  static void onDeviceColorConfigStateChanged(BLEDevice central, BLECharacteristic characteristic);

  /// @brief Callback for when the device color configuration write status changes.
  /// This is triggered when the central device writes to the `DEVICE_COLOR_CONFIG_WRITE_UUID` characteristic,
  /// signaling that the peripheral should read the new color configuration and save it to EEPROM.
  /// @param central The BLE device that wrote the characteristic.
  /// @param characteristic The characteristic that was written to.
  /// @note This callback is static so it can be used as a BLE event handler.
  static void onDeviceColorConfigWriteChanged(BLEDevice central, BLECharacteristic characteristic);

  static String serviceIds[];
  char *m_deviceName;
  BLEService *m_service;
  BLEIntCharacteristic *m_numberOfPlayers;
  BLEIntCharacteristic *m_currentPlayer;
  BLEIntCharacteristic *m_timer;
  BLEIntCharacteristic *m_elapsedTime;
  BLEIntCharacteristic *m_myPlayerNumber;
  BLEIntCharacteristic *m_gameState;
  BLEBoolCharacteristic *m_endTurn;
  BLEBoolCharacteristic *m_toggleSkip;
  BLEIntCharacteristic *m_skippedPlayers;

  BLECharacteristic *m_deviceColorConfig;
  BLEIntCharacteristic *m_deviceColorConfigState;
  BLEBoolCharacteristic *m_deviceColorConfigWrite;
  BLEStringCharacteristic *m_deviceNameCharacteristic;
  BLEBoolCharacteristic *m_deviceNameWrite;

  BLEDescriptor *m_activeTurnDescriptor;
  unsigned long lastPoll;
};

#endif