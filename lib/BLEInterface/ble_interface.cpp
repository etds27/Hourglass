
#ifndef SIMULATOR
#include "ble_interface.h"
#include "logger.h"
#include "device_config.h"

const char *SERVICE_UUID = "d7560343-51d4-4c24-a0fe-118fd9078144";
const char *DEVICE_STATE_UUID = "3f29c2e5-3837-4498-bcc1-cb33f1c10c3c";
const char *TOTAL_PLAYERS_UUID = "d776071e-9584-42db-b095-798a90049ee0";
const char *CURRENT_PLAYER_UUID = "6efe0bd2-ad04-49bb-8436-b7e1d1902fea";
const char *MY_PLAYER_UUID = "f1223124-c708-4b98-a486-48515fa59d3d";
const char *END_TURN_UUID = "c27802ab-425e-4b15-8296-4a937da7125f";
const char *TOGGLE_SKIP_UUID = "9b4fa66f-20cf-4a7b-ba6a-fc3890cbc0c7";
const char *SKIPPED_PLAYERS_UUID = "3e35aa50-3594-4b32-9684-dbacb5ba91ee";
const char *REMAINING_TIME_UUID = "4e1c05f6-c128-4bca-96c3-29c014e00eb6";
const char *TIMER_UUID = "4661b4c1-093d-4db7-bb80-5b5fe3eae519";
const char *SKIPPED_PLAYERS = "b31fa38e-a424-47ad-85d9-639cbab14e88";

const char *DEVICE_NAME_UUID = "050753a4-2b7a-41f9-912e-4310f5e750e6";
const char *DEVICE_NAME_WRITE_UUID = "050753a4-2b7a-41f9-912e-4310f5e750e6";
const char *DEVICE_COLOR_CONFIG_UUID = "85f6ff14-861b-47cf-8e41-5f5b94100bd9";
const char *DEVICE_COLOR_CONFIG_STATE_UUID = "f4c4d6e1-3b1e-4d2a-8f3a-2e5b8f0c6d7e";
const char *DEVICE_COLOR_CONFIG_WRITE_UUID = " 4408c2ec-10c0-4a76-87ab-4d9a5b51eaa7 ";

const char *DESCRIPTOR_UUID = "00002902-0000-1000-8000-00805f9b34fb";

const int BLE_POLL_RATE = 50;

// Required to let the linker know about the static instance
BLEInterface* BLEInterface::instance = nullptr;

BLEInterface::BLEInterface(char *deviceName)
{
  instance = this;
  if (!BLE.begin())
  {
    Serial.println("Starting BLE failed!");
    while (1)
      ; // Stay here if initialization fails
  }
  m_deviceName = deviceName;
  lastPoll = millis();
}
void BLEInterface::sendDeviceName(const char *name)
{
  m_deviceNameCharacteristic->setValue(name);
}

void BLEInterface::sendDeviceColorConfig(ColorConfig config)
{
  m_deviceColorConfig->writeValue((uint8_t*)&config, sizeof(ColorConfig));
}

ColorConfig BLEInterface::readColorConfig()
{
    ColorConfig config;
    m_deviceColorConfig->readValue((uint8_t*)&config, sizeof(ColorConfig));
    return config;
}

void BLEInterface::getDeviceName(char* out, uint8_t length)
{
    const String data = m_deviceNameCharacteristic->value();
    data.toCharArray(out, length);
}

DeviceState::State BLEInterface::getDeviceColorConfigState()
{   
    return  static_cast<DeviceState::State>(m_deviceColorConfigState->value());
}

bool BLEInterface::isConnected()
{
  BLE.poll();
  return BLE.connected();
}

DeviceState::State BLEInterface::getCommandedDeviceState()
{
  int rawState = m_gameState->value();
  m_deviceState = static_cast<DeviceState::State>(rawState);
  return m_deviceState;
}

bool BLEInterface::isTurn()
{
  getCommandedDeviceState();
  return DeviceState::isStateActiveTurn(m_deviceState);
}

void BLEInterface::endTurn()
{
  // Write an end turn notification by setting the value to true first to initiate the end turn
  // Then a false is written to ensure that the next `endTurn` action will write correctly
  logger.debug("Sending End Turn Notification");
  m_endTurn->writeValue(true);
  m_endTurn->writeValue(false);
}

void BLEInterface::toggleSkippedState() {
  logger.debug("Sending Skipped State Toggle Notification");
  m_toggleSkip->writeValue(true);
  m_toggleSkip->writeValue(false);
}

int BLEInterface::getTimer()
{
  m_lastTimer = m_timer->value();
  return m_lastTimer;
}

int BLEInterface::getElapsedTime()
{
  if (m_elapsedTime->written()) {
    m_lastElapsedTimePoll = millis();
  }
  m_lastElapsedTime = m_elapsedTime->value();
  return m_lastElapsedTime;
}

int BLEInterface::getMyPlayer()
{
  m_lastMyPlayer = m_myPlayerNumber->value();
  return m_lastMyPlayer;
}

int BLEInterface::getCurrentPlayer()
{
  m_lastCurrentPlayer = m_currentPlayer->value();
  return m_lastCurrentPlayer;
}

int BLEInterface::getTotalPlayers()
{
  m_lastTotalPlayers = m_numberOfPlayers->value();
  return m_lastTotalPlayers;
}

uint16_t BLEInterface::getSkippedPlayers() 
{
  return (uint16_t)m_skippedPlayers->value();
}

bool BLEInterface::isGameActive()
{
  getCommandedDeviceState();
  return DeviceState::isStateActiveGame(m_deviceState);
}

bool BLEInterface::isGamePaused()
{
  getCommandedDeviceState();
  return m_deviceState == DeviceState::State::Paused;
}

bool BLEInterface::isTurnTimerEnforced()
{
  getCommandedDeviceState();
  return m_deviceState == DeviceState::State::ActiveTurnEnforced;
}

void BLEInterface::poll()
{
  if (millis() - lastPoll > BLE_POLL_RATE)
  {
    BLE.poll();
  }
}

void BLEInterface::onDeviceNameChanged(BLEDevice central, BLECharacteristic characteristic)
{
  if (instance->m_deviceNameChangeCallback)
  {
    char name[MAX_NAME_LENGTH] {};
    const uint8_t * data = characteristic.value();
    int len = characteristic.valueLength();
    strncpy(name, (const char*)data, len);
    name[min(len, (int) sizeof(name) - 1) ] = '\0';
    instance->m_deviceNameChangeCallback(name);
  }
  else
  {
    logger.warning("No device name change callback registered");
  }
}

void BLEInterface::onDeviceNameWriteChanged(BLEDevice central, BLECharacteristic characteristic)
{
  if (instance->m_deviceNameWriteChangeCallback)
  {
    const bool writeStatus = characteristic.value()[0];
    if (writeStatus) {
      instance->m_deviceNameWriteChangeCallback(writeStatus);
    } else {
      logger.info("Device name write status not enabled");
    }

  }
  else
  {
    logger.warning("No device name write change callback registered");
  }
}

void BLEInterface::onDeviceColorConfigChanged(BLEDevice central, BLECharacteristic characteristic)
{
  if (instance->m_deviceColorConfigChangeCallback)
  {
    ColorConfig config;
    characteristic.readValue((uint8_t*)&config, sizeof(ColorConfig));
    instance->m_deviceColorConfigChangeCallback(config);
  }
  else
  {
    logger.warning("No device color change callback registered");
  }
}

void BLEInterface::onDeviceColorConfigStateChanged(BLEDevice central, BLECharacteristic characteristic)
{
  if (instance->m_deviceColorConfigStateChangeCallback)
  {
    DeviceState::State state = static_cast<DeviceState::State>(characteristic.value()[0]);
    instance->m_deviceColorConfigStateChangeCallback(state);
  }
  else
  {
    logger.warning("No device color state change callback registered");
  }
}

void BLEInterface::onDeviceColorConfigWriteChanged(BLEDevice central, BLECharacteristic characteristic)
{
  if (instance->m_deviceColorConfigWriteChangeCallback)
  {
    bool writeStatus = characteristic.value()[0];
    if (writeStatus) {
      instance->m_deviceColorConfigWriteChangeCallback(writeStatus);
    } else {
      logger.info("Device color write status not enabled");
    }
  }
  else
  {
    logger.warning("No device color write change callback registered");
  }
}

void BLEInterface::setService()
{
  logger.info(SERVICE_UUID);
  m_service = new BLEService(SERVICE_UUID);

  // Number of players
  // m_dummy = new BLEIntCharacteristic(DUMMY_UUID, BLERead | BLEWrite);
  // BLEDescriptor numPlayersDescriptor = BLEDescriptor("6137", "Number of players attached to the host");
  // m_numberOfPlayers->addDescriptor(numPlayersDescriptor);
  // m_service->addCharacteristic(*m_dummy);

  // Number of players
  m_numberOfPlayers = new BLEIntCharacteristic(TOTAL_PLAYERS_UUID, BLERead | BLEWrite);
  // BLEDescriptor numPlayersDescriptor = BLEDescriptor("6137", "Number of players attached to the host");
  // m_numberOfPlayers->addDescriptor(numPlayersDescriptor);
  m_service->addCharacteristic(*m_numberOfPlayers);

  // Current player
  m_currentPlayer = new BLEIntCharacteristic(CURRENT_PLAYER_UUID, BLEWrite | BLERead);
  m_service->addCharacteristic(*m_currentPlayer);

  // Timer
  m_timer = new BLEIntCharacteristic(TIMER_UUID, BLEWrite | BLERead);
  m_service->addCharacteristic(*m_timer);

  // Remaining time
  m_elapsedTime = new BLEIntCharacteristic(REMAINING_TIME_UUID, BLEWrite | BLERead);
  m_service->addCharacteristic(*m_elapsedTime);

  // Device's player number
  m_myPlayerNumber = new BLEIntCharacteristic(MY_PLAYER_UUID, BLEWrite | BLERead);
  m_service->addCharacteristic(*m_myPlayerNumber);

  // Game State Characteristic
  m_gameState = new BLEIntCharacteristic(DEVICE_STATE_UUID, BLEWrite | BLENotify | BLERead);
  m_service->addCharacteristic(*m_gameState);

  // End turn
  m_endTurn = new BLEBoolCharacteristic(END_TURN_UUID, BLEWrite | BLENotify | BLERead);
  m_service->addCharacteristic(*m_endTurn);

  // Skip action Characteristic
  m_toggleSkip = new BLEBoolCharacteristic(TOGGLE_SKIP_UUID, BLEWrite | BLENotify | BLERead);
  m_service->addCharacteristic(*m_toggleSkip);

  // Game is currently paused
  m_skippedPlayers = new BLEIntCharacteristic(SKIPPED_PLAYERS, BLEWrite | BLERead);
  m_service->addCharacteristic(*m_skippedPlayers);

  // ========= CONFIGURATION CHARACTERISTICS ==========

  // Device Name Characteristic
  m_deviceNameCharacteristic = new BLEStringCharacteristic(DEVICE_NAME_UUID, BLERead | BLEWrite, MAX_NAME_LENGTH);
  m_deviceNameCharacteristic->setEventHandler(BLEWritten, onDeviceNameChanged);
  m_service->addCharacteristic(*m_deviceNameCharacteristic);

  // Device Name Write Characteristic

  m_deviceNameWrite = new BLEBoolCharacteristic(DEVICE_NAME_WRITE_UUID, BLERead | BLEWrite);
  m_deviceNameWrite->setEventHandler(BLEWritten, onDeviceNameWriteChanged);
  m_service->addCharacteristic(*m_deviceNameCharacteristic);

  // Setting a 16 byte config to match the 4 x 4 size color config
  m_deviceColorConfig = new BLECharacteristic(DEVICE_COLOR_CONFIG_UUID, BLERead | BLEWrite | BLENotify, 16);
  m_deviceColorConfig->setEventHandler(BLEWritten, onDeviceColorConfigChanged);
  m_service->addCharacteristic(*m_deviceColorConfig);

  // Device Color Config State Characteristic
  m_deviceColorConfigState = new BLEIntCharacteristic(DEVICE_COLOR_CONFIG_STATE_UUID, BLERead | BLEWrite | BLENotify);
  m_deviceColorConfigState->setEventHandler(BLEWritten, onDeviceColorConfigStateChanged);
  m_service->addCharacteristic(*m_deviceColorConfigState);

  // Device Color Config Write Characteristic
  m_deviceColorConfigWrite = new BLEBoolCharacteristic(DEVICE_COLOR_CONFIG_WRITE_UUID, BLERead | BLEWrite);
  m_deviceColorConfigWrite->setEventHandler(BLEWritten, onDeviceColorConfigWriteChanged);
  m_service->addCharacteristic(*m_deviceColorConfigWrite);

  logger.info("Advertising with name: ", m_deviceName);
  BLE.setLocalName(m_deviceName);
  BLE.setAdvertisedService(*m_service);
  BLE.addService(*m_service);

  BLE.advertise();
  logger.info("Connected");
}

void BLEInterface::readData()
{
  if (!isConnected())
  {
    logger.warning("BLE interface not connected");
    return;
  }
  logger.info("Num Players:    ", getTotalPlayers());
  logger.info("Current Player: ", getCurrentPlayer());
  logger.info("Timer:          ", getTimer());
  logger.info("Elapsed Time:   ", getElapsedTime());
  logger.info("My number:      ", getMyPlayer());
  logger.info("\n");
}

#endif