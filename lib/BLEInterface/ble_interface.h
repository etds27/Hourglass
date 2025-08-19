#pragma once

#ifndef SIMULATOR
#include <ArduinoBLE.h>
#include "hg_central_interface.h"
#include "device_state.h"
#include <stdint.h>

class BLEInterface : public HGCentralInterface
{
public:
  BLEInterface(char *deviceName);

  void sendDeviceName(const char *name);
  void sendDeviceColor(uint32_t color);
  void sendDeviceAccentColor(uint32_t accentColor);

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

  uint16_t getSkippedPlayers();

  DeviceState::State getCommandedDeviceState();

  void setService();

  void poll();

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

  /// @brief Callback for when the device color changes.
  /// This will be called when the device color characteristic is written to.
  /// @param central The BLE device that wrote the characteristic
  /// @param characteristic The characteristic that was written to
  /// @note This callback is static so it can be used as a BLE event handler.
  static void onDeviceColorChanged(BLEDevice central, BLECharacteristic characteristic);

  /// @brief Callback for when the device accent color changes.
  /// This will be called when the device accent color characteristic is written to.
  /// @param central The BLE device that wrote the characteristic
  /// @param characteristic The characteristic that was written to
  /// @note This callback is static so it can be used as a BLE event handler.
  static void onDeviceAccentColorChanged(BLEDevice central, BLECharacteristic characteristic);

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

  BLEIntCharacteristic *m_deviceColor;
  BLEIntCharacteristic *m_deviceAccentColor;
  BLEStringCharacteristic *m_deviceNameCharacteristic;

  BLEDescriptor *m_activeTurnDescriptor;
};

#endif