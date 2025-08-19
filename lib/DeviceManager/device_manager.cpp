#ifndef SIMULATOR
#include <EEPROM.h>
#include "ble_interface.h"
#include "button_input_interface.h"
#include "device_config.h"
#else
#include "gl_input_interface.h"
#include "gl_ring_interface.h"
#include "simulator_central_interface.h"
#include "simulator_tools.h"
#endif

#include <cstring>
#include "device_manager.h"
#include "logger.h"

DeviceManager::DeviceManager(HourglassDisplayManager *displayManager)
{
  m_displayManager = displayManager;

  logger.info("Initializing Device Manager");
  m_deviceName = new char[MAX_NAME_LENGTH];
  readDeviceName(m_deviceName);
  logger.info("Device name: ", m_deviceName);


#ifdef SIMULATOR
  m_inputInterface = new GLInputInterface();
  m_interface = new SimulatorCentralInterface(m_deviceName);
#else

  logger.info("Creating Input Interface");
  m_inputInterface = new ButtonInputInterface(BUTTON_INPUT_PIN);
  logger.info("Creating Central Interface");
  m_interface = new BLEInterface(m_deviceName);

  m_interface->registerDeviceNameChangedCallback([this](char *name) {
    this->onDeviceNameChanged(name);
  });

  m_interface->registerDeviceColorChangedCallback([this](uint32_t color) {
    this->onDeviceColorChanged(color);
  });

  m_interface->registerDeviceAccentColorChangedCallback([this](uint32_t accentColor) {
    this->onDeviceAccentColorChanged(accentColor);
  });

  // Allows the main device button to wake the device from sleep state
  esp_sleep_enable_ext0_wakeup(BUTTON_GPIO_PIN, HIGH);
#endif

  logger.info("Creating Input Monitor");
  m_buttonMonitor = new ButtonInputMonitor(m_inputInterface);
}

DeviceManager::~DeviceManager()
{
}

void DeviceManager::start()
{
  logger.info("Starting the device manager");
  m_displayManager->setDisplayMode(DeviceState::State::Off);

  m_interface->setService();
  setWaitingForConnection();

  // Once we are connected, immediately send of the device configuration
  m_interface->sendDeviceName(m_deviceName);
  m_interface->sendDeviceColor(DeviceConfigurator::readColor());
  m_interface->sendDeviceAccentColor(DeviceConfigurator::readAccentColor());

  uint32_t now = millis();
  m_lastUpdate = now;
  m_lastReadOut = now;
  m_lastConnection = now;
  m_lastTurnStart = now;
  m_lastDisconnection = now;
}

char *DeviceManager::getDeviceName()
{
  return m_deviceName;
}

char *DeviceManager::readDeviceName(char *out)
{
  char arduinoID[MAX_NAME_LENGTH] = {};
#ifdef SIMULATOR
  out = new char[10]{
      'S', 'I', 'M', 'U', 'L', 'A', 'T', 'O', 'R', '\0'};
#else
  int i;
  // Read the arduinos ID
  DeviceConfigurator::readName(arduinoID, MAX_NAME_LENGTH);
  
#endif
  // logger.info(arduinoID);
  strncpy(out, arduinoID, MAX_NAME_LENGTH);
  out[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination
  return out;
}

#ifndef SIMULATOR
void DeviceManager::writeDeviceName(const char *deviceName, uint8_t length)
{
  // Write unique ID to EEPROM
  DeviceConfigurator::writeName(deviceName);

  // Read the device name so it is reflected in the DeviceManager
  readDeviceName(m_deviceName);
  logger.info("Device name changed to: ", deviceName);
  m_interface->sendDeviceName(deviceName);
}

void DeviceManager::writeDeviceColor(uint32_t color)
{
  // Write the color to the EEPROM
  DeviceConfigurator::writeColor(color);

  // Read the color so it is reflected in the DeviceManager
  m_interface->sendDeviceColor(color);
  logger.info("Device color updated to: ", color);
}

void DeviceManager::writeDeviceAccentColor(uint32_t accentColor)
{
  // Write the accent color to the EEPROM
  DeviceConfigurator::writeAccentColor(accentColor);

  // Read the accent color so it is reflected in the DeviceManager
  m_interface->sendDeviceAccentColor(accentColor);
  logger.info("Device accent color updated to: ", accentColor);
}
#endif

bool DeviceManager::isActiveTurn()
{
  return m_interface->isTurn();
}

void DeviceManager::onDeviceNameChanged(char *name)
{
  writeDeviceName(name, strlen(name));
}

void DeviceManager::onDeviceColorChanged(uint32_t color)
{
  writeDeviceColor(color);
}

void DeviceManager::onDeviceAccentColorChanged(uint32_t accentColor)
{
  writeDeviceAccentColor(accentColor);
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
  if (diff)
  {
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
  int elapsedTime = m_interface->getExpectedElapsedTime();
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
  updateDisplayMode();
}

void DeviceManager::toggleDeviceOrientation()
{
  m_absoluteOrientation = !m_absoluteOrientation;
  logger.info("Updated absolute orientation to: ", m_absoluteOrientation);
  m_displayManager->setAbsoluteOrientation(m_absoluteOrientation);
  updateDisplay();
}

void DeviceManager::toggleColorBlindMode()
{
  m_colorBlindMode = !m_colorBlindMode;
  logger.info("Setting Color Blind Mode to: ", m_colorBlindMode);
  m_displayManager->setColorBlindMode(m_colorBlindMode);
  updateDisplay();
}

void DeviceManager::enterDeepSleep()
{
  logger.info("Entering Deep Sleep");
  m_deviceState = DeviceState::State::Off;
  updateDisplayMode();
  updateDisplay(true);

#ifndef SIMULATOR
  delay(1000);
  esp_deep_sleep_start();
#endif
}

void DeviceManager::updateDisplay(bool force)
{
  m_displayManager->update(force);
}

void DeviceManager::updateAwaitingGameStartData()
{
  GameStartData data{
      .totalPlayers = m_interface->getTotalPlayers()};
  m_displayManager->updateAwaitingGameStartData(data);
}

void DeviceManager::updateTurnSequence()
{
  int totalPlayers = m_interface->getTotalPlayers();
  int myPlayer = m_interface->getMyPlayer();
  int currentPlayer = m_interface->getCurrentPlayer();
  uint16_t skippedPlayers = m_interface->getSkippedPlayers();

  struct TurnSequenceData data = {.totalPlayers = totalPlayers, .myPlayerIndex = myPlayer, .currentPlayerIndex = currentPlayer, .skippedPlayers = skippedPlayers};
  m_displayManager->updateTurnSequenceData(data);
}

void DeviceManager::updateDisplayMode()
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
    logger.info("Update Period: ", deltaTime);

    if (ENABLE_DEBUG)
    {
      m_interface->readData();
    }
    m_lastReadOut = currentTime;
  }
#ifndef SIMULATOR
  BLE.poll();
#endif
  processGameState();

  updateDisplay();
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
    logger.debug("Entering deep sleep");
    // enterDeepSleep();
  }

  if (buttonAction == ButtonInputType::LongPress) {
    toggleDeviceOrientation();
  }

  if (buttonAction == ButtonInputType::TripleButtonPress)
  {
    toggleColorBlindMode();
  }

  if (!m_interface->isConnected())
  {
    m_lastDisconnection = m_lastUpdate;
    setWaitingForConnection();
    return;
  }
  m_lastConnection = m_lastUpdate;

  // This prolongs the Awaiting Connection state for EXPECTED_CHARACTERISTIC_DISCOVERY ms after the initial device connection is initiated by the central device
  // During this time, the central device will discover and populate all characteristics so when the commanded state is first shown, all data is available
  // If this check is not made, the display will have undefined behavior between initial connection and service discovery
  if (m_lastUpdate - m_lastDisconnection < EXPECTED_CHARACTERISTIC_DISCOVERY)
  {
    return;
  }

  // *** DISPLAY FOR ACTIVE GAME ***
  // Determine is a device state change was made. If it was, we will update the state after getting display specific data
  bool updateDeviceState = updateCommandedDeviceState();
  // Retrieve state specific data for display modes

  if (DeviceState::deviceStateRequiresGameStartData(m_deviceState)) 
  {
    updateAwaitingGameStartData();
  }

  if (DeviceState::deviceStateRequiresTurnSequenceData(m_deviceState)) 
  {
    updateTurnSequence();
  }

  if (DeviceState::deviceStateRequiresTimeData(m_deviceState)) 
  {
    updateTimer();
  }

  if (updateDeviceState)
  {
    m_displayManager->setDisplayMode(m_deviceState);
  }

  // *** INPUT PROCESSING ***
  if (buttonAction == ButtonInputType::DoubleButtonPress && DeviceState::isStateSkipEligible(m_deviceState))
  {
    m_interface->toggleSkippedState();
  }

  if (isActiveTurn() && buttonAction == ButtonInputType::ButtonPress)
  {
    sendEndTurn();
  }
}
