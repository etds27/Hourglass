#include <EEPROM.h>
#include "device_manager.h"
#include "ble_interface.h"
#include "logger.h"

DeviceManager::DeviceManager(HourglassDisplayManager *displayManager)
{
  m_displayManager = displayManager;

  logger.info("Initializing Device Manager");
  m_deviceName = new char[8];
  readDeviceName(m_deviceName);
  logger.info("Device name: " + String(m_deviceName));

  logger.info("Initializing Display Data Structs");
  gameStartData = new GameStartData;

  m_buttonMonitor = new ButtonInputMonitor(BUTTON_INPUT_PIN);
  // Allows the main device button to wake the device from sleep state
  esp_sleep_enable_ext0_wakeup(BUTTON_GPIO_PIN, HIGH);
  m_interface = new BLEInterface(m_deviceName);
}

DeviceManager::~DeviceManager()
{
  delete gameStartData;
}

void DeviceManager::start()
{
  logger.info("Starting the device manager");
  m_displayManager->setDisplayMode(DeviceState::State::Off);

  m_interface->setService();
  setWaitingForConnection();
  uint32_t now = millis();
  m_lastUpdate = now;
  m_lastReadOut = now;
  m_lastConnection = now;
  m_lastTurnStart = now;
}

char *DeviceManager::getDeviceName()
{
  return m_deviceName;
}

char *DeviceManager::readDeviceName(char *out)
{
  int i;
  // Read the arduinos ID

  char arduinoID[8] = {};

  for (i = 0; i < 8; i++)
  {
    arduinoID[i] = EEPROM.read(i);
  }
  arduinoID[i] = '\0';
  logger.info("Read arduino name");
  logger.info(String(arduinoID));
  strcpy(out, arduinoID);
  return out;
}

void DeviceManager::writeDeviceName(char *deviceName, uint8_t length)
{
  // Write unique ID to EEPROM
  int i;
  logger.info("Writing Device Name: " + String(deviceName));
  for (i = 0; i < length; i++)
  {
    EEPROM.write(i, deviceName[i]);
  }
  EEPROM.write(i, '\0');
  EEPROM.commit();
  logger.info("Wrote device name to EEPROM: " + String(deviceName));
  m_deviceName = deviceName;
}

bool DeviceManager::isActiveTurn()
{
  return m_interface->isTurn();
}

// Transition from the active turn to 'turn sequence' mode
void DeviceManager::sendEndTurn()
{
  m_interface->endTurn();
}

bool DeviceManager::updateCommandedDeviceState()
{
  DeviceState::State newDeviceState = m_interface->getCommandedDeviceState();
  bool diff = m_deviceState != newDeviceState;
  if (diff) {
    char message[50];
    sprintf(message, "Device State transition: %i -> %i", static_cast<int>(m_deviceState), static_cast<int>(newDeviceState));
    logger.info(message);
  }
  m_deviceState = m_interface->getCommandedDeviceState();
  return diff;
}

void DeviceManager::updateTimer()
{
  int timer = m_interface->getTimer();
  int elapsedTime = m_interface->getElapsedTime();
  bool isTurnTimeEnforced = m_interface->isTurnTimerEnforced();
  struct TimerData data = {
      .totalTime = timer,
      .elapsedTime = elapsedTime,
      .isTurnTimeEnforced = isTurnTimeEnforced};

  m_displayManager->updateTimerData(data);
}

void DeviceManager::setWaitingForConnection()
{
  if (m_deviceState == DeviceState::State::AwaitingConnection)
  {
    return;
  }
  logger.info("Setting device state to: AwaitingConnection");
  m_deviceState = DeviceState::State::AwaitingConnection;
  updateRingMode();
}

void DeviceManager::toggleColorBlindMode()
{
  m_colorBlindMode = !m_colorBlindMode;
  logger.info("Setting Color Blind Mode to: " + String(m_colorBlindMode));
  m_displayManager->setColorBlindMode(m_colorBlindMode);
  updateRing();
}

void DeviceManager::enterDeepSleep()
{
  logger.info("Entering Deep Sleep");
  m_deviceState = DeviceState::State::Off;
  updateRingMode();
  updateRing(true);
  delay(1000);
  esp_deep_sleep_start();
}

void DeviceManager::updateRing(bool force)
{
  m_displayManager->update(force);
}

void DeviceManager::updateAwaitingGameStartData()
{
  gameStartData->totalPlayers = m_interface->getTotalPlayers();
  m_displayInterface->updateAwaitingGameStartData(*gameStartData);
}

void DeviceManager::updateTurnSequence()
{
  int totalPlayers = m_interface->getTotalPlayers();
  int myPlayer = m_interface->getMyPlayer();
  int currentPlayer = m_interface->getCurrentPlayer();

  struct TurnSequenceData data = {.totalPlayers = totalPlayers, .myPlayerIndex = myPlayer, .currentPlayerIndex = currentPlayer};
  m_displayManager->updateTurnSequenceData(data);
}

void DeviceManager::updateRingMode()
{
  m_displayManager->setDisplayMode(m_deviceState);
}

void DeviceManager::update()
{
  unsigned long currentTime = millis();
  unsigned long deltaTime = currentTime - m_lastUpdate;
  m_lastUpdate = currentTime;

  // Log data from the interface
  if (currentTime - m_lastReadOut > 2000)
  {
    logger.info("Update Period: " + String(deltaTime));
    // m_interface->readData();
    m_lastReadOut = currentTime;
  }
  // logger.info("Running " + String(deltaTime));
  BLE.poll();
  processGameState();
  updateRing();
}

void DeviceManager::processGameState()
{
  ButtonInputType buttonAction = m_buttonMonitor->getAction();

  if (m_started)
  {
    logger.error("Attempting to update the device manager before running `start()`");
    while (1)
      ;
  }

  // Check how long we have been awaiting connection.
  // If it is longer than the no connection timeout, enter deep sleep
  if (m_deviceState == DeviceState::State::AwaitingConnection && m_lastUpdate - m_lastConnection > CONNECTION_TIMEOUT)
  {
    enterDeepSleep();
  }

  if (buttonAction == ButtonInputType::TripleButtonPress)
  {
    toggleColorBlindMode();
  }

  if (!m_interface->isConnected())
  {
    setWaitingForConnection();
    return;
  }
  m_lastConnection = m_lastUpdate;

  // This prolongs the Awaiting Connection state for EXPECTED_CHARACTERISTIC_DISCOVERY ms after the initial device connection is initiated by the central device
  // During this time, the central device will discover and populate all characteristics so when the commanded state is first shown, all data is available
  // If this check is not made, the display will have undefined behavior between initial connection and service discovery
  if (m_lastUpdate - m_lastDisconnection < EXPECTED_CHARACTERISTIC_DISCOVERY) {
    return;
  }


  // *** DISPLAY FOR ACTIVE GAME ***
  // Determine is a device state change was made. If it was, we will update the state after getting display specific data
  bool updateDeviceState = updateCommandedDeviceState();
  // Retrieve state specific data for display modes
  switch (m_deviceState)
  {
  case DeviceState::State::AwaitingGameStart:
    updateAwaitingGameStartData();
    break;
  case DeviceState::State::ActiveTurnEnforced:
    updateTimer();
    break;
  case DeviceState::State::AwaitingTurn:
    updateTurnSequence();
    break;
  }

  if (updateDeviceState) {
    m_displayInterface->setDisplayMode(m_deviceState);
  }

  // *** INPUT PROCESSING ***
  if (buttonAction == ButtonInputType::DoubleButtonPress && DeviceState::isStateSkipEligible(m_deviceState))
  {
    m_interface->toggleSkippedState();
  }

  if (isActiveTurn() && buttonAction == ButtonInputType::ButtonPress) {
    sendEndTurn();
  }
}
