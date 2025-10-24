#include "device_manager.h"
#include "logger.h"

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

namespace
{
  const LogString loggerTag = "DeviceManager";
}

DeviceManager::DeviceManager(HourglassDisplayManager *displayManager, InputInterface *inputInterface, HGCentralInterface *centralInterface)
{
  m_displayManager = displayManager;
  m_inputInterface = inputInterface;
  m_interface = centralInterface;

  logger.info(loggerTag, ": Initializing Device Manager");
  m_deviceName = new char[MAX_NAME_LENGTH];
  readDeviceName(m_deviceName);
  logger.info(loggerTag, ": Device name: ", m_deviceName);


#ifndef SIMULATOR
  registerCallbacks();

  // Allows the main device button to wake the device from sleep state
  esp_sleep_enable_ext0_wakeup(BUTTON_GPIO_PIN, HIGH);
#endif
  m_interface->sendDeviceName(m_deviceName);

  logger.info(loggerTag, ": Creating Input Monitor");
  m_buttonMonitor = new ButtonInputMonitor(m_inputInterface);
}

DeviceManager::~DeviceManager() = default;

void DeviceManager::start()
{
  logger.info(loggerTag, ": Starting the device manager");
  m_displayManager->setDisplayMode(DeviceState::State::Off);

  m_interface->setService();
  setWaitingForConnection();

  // Once we are connected, immediately send the device configuration
  // logger.info(loggerTag, ": Sending initial device configuration to central");
  m_interface->sendDeviceName(m_deviceName);
  // logger.info(loggerTag, ": Sent device name to central: ", m_deviceName);
  // logger.info(loggerTag, ": Sending default color configuration to central");
  m_interface->sendDeviceColorConfig(DeviceConfigurator::DEFAULT_COLOR_CONFIG);

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
  out = new char[10]{'S', 'I', 'M', 'U', 'L', 'A', 'T', 'O', 'R', '\0'};
#else
  DeviceConfigurator::readName(arduinoID, MAX_NAME_LENGTH);
#endif

  strncpy(out, arduinoID, MAX_NAME_LENGTH);
  out[MAX_NAME_LENGTH - 1] = '\0'; // Ensure null-termination
  return out;
}

#ifndef SIMULATOR
void DeviceManager::writeDeviceName(const char *deviceName, uint8_t length)
{
  DeviceConfigurator::writeName(deviceName);
  readDeviceName(m_deviceName);
  logger.info(loggerTag, ": Device name changed to: ", deviceName);
  m_interface->sendDeviceName(deviceName);
}

void DeviceManager::writeDeviceColorConfig(ColorConfig config, DeviceState::State configState)
{
  DeviceConfigurator::writeColorConfig(config, static_cast<uint16_t>(configState));
  // m_interface->sendDeviceColorConfig(config);
  DeviceConfigurator::printColorConfig(config, static_cast<uint16_t>(configState));
}

#endif

bool DeviceManager::isActiveTurn()
{
  return m_interface->isTurn();
}

void DeviceManager::onDeviceNameChanged(char *name)
{
  // No-op
  // Device name writes are handled immediately in onDeviceNameChanged
}

void DeviceManager::onDeviceNameWriteChanged(bool write)
{
  if (write)
  {
    logger.info(loggerTag, ": Device name write enabled");
    m_interface->getDeviceName(m_deviceName, MAX_NAME_LENGTH);
    logger.info(loggerTag, ": onDeviceNameWriteChanged received. Device name changed to: ", m_deviceName);
    writeDeviceName(m_deviceName, strlen(m_deviceName));
  }
  else
  {
    logger.info(loggerTag, ": Device name write not enabled");
  }
}

void DeviceManager::onDeviceColorConfigChanged(ColorConfig config)
{
  DeviceConfigurator::printColorConfig(config, 0);
  // writeDeviceColorConfig(config);
}

void DeviceManager::onDeviceColorConfigWriteChanged(bool write)
{
  if (write)
  {
    logger.info(loggerTag, ": Device color config write enabled");
    DeviceState::State configState= m_interface->getDeviceColorConfigState();
    ColorConfig config = m_interface->readColorConfig();
    // DeviceConfigurator::printColorConfig(config, configState);
    writeDeviceColorConfig(config, configState);
  }
  else
  {
    logger.info(loggerTag, ": Device color config write not enabled");
  }
}

void DeviceManager::onDeviceColorConfigStateChanged(DeviceState::State state)
{
  logger.info(loggerTag, ": onDeviceColorConfigStateChanged received. New state: ", static_cast<int>(state));
  // Update the app with the new configuration state
  ColorConfig config = DeviceConfigurator::readColorConfig(static_cast<uint16_t>(state));
  m_interface->sendDeviceColorConfig(config);
}

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
    logger.info(loggerTag, ": Device State transition: ",
                static_cast<int>(m_deviceState), " -> ",
                static_cast<int>(newDeviceState));
  }
  m_deviceState = newDeviceState;
  return diff;
}

void DeviceManager::updateTimer()
{
  int timer = m_interface->getTimer();
  int elapsedTime = m_interface->getExpectedElapsedTime();
  bool isTurnTimeEnforced = m_interface->isTurnTimerEnforced();
  TimerData data{.totalTime = timer, .elapsedTime = elapsedTime, .isTurnTimeEnforced = isTurnTimeEnforced};

  m_displayManager->updateTimerData(data);
}

void DeviceManager::setWaitingForConnection()
{
  if (m_deviceState == DeviceState::State::AwaitingConnection)
    return;

  logger.info(loggerTag, ": Setting device state to: AwaitingConnection");
  m_deviceState = DeviceState::State::AwaitingConnection;
  updateDisplayMode();
}

void DeviceManager::toggleDeviceOrientation()
{
  m_absoluteOrientation = !m_absoluteOrientation;
  logger.info(loggerTag, ": Updated absolute orientation to: ", m_absoluteOrientation);
  m_displayManager->setAbsoluteOrientation(m_absoluteOrientation);
  updateDisplay();
}

void DeviceManager::toggleColorBlindMode()
{
  m_colorBlindMode = !m_colorBlindMode;
  logger.info(loggerTag, ": Setting Color Blind Mode to: ", m_colorBlindMode);
  m_displayManager->setColorBlindMode(m_colorBlindMode);
  updateDisplay();
}

void DeviceManager::enterDeepSleep()
{
  logger.info(loggerTag, ": Entering Deep Sleep");
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
  if (m_deviceState == DeviceState::State::ConfigurationMode)
  {
    updateConfigDisplay(force);
    return;
  }

  m_displayManager->update(force);
}

void DeviceManager::updateConfigDisplay(bool force)
{
  DeviceState::State configState = m_interface->getDeviceColorConfigState();

  if (DeviceState::deviceStateRequiresAuxiliaryData(configState))
  {
    logger.info(loggerTag, ": updateConfigDisplay: New config state requires auxiliary data: ", static_cast<int>(configState));
    configState = DeviceState::State::Off;
  }

  bool diff = m_configState != configState;
  if (diff || configState == DeviceState::State::Off)
  {
    logger.info(loggerTag, ": Device Config State transition: ",
                static_cast<int>(m_configState), " -> ",
                static_cast<int>(configState));
    m_displayManager->setDisplayMode(m_configState);
  }
  m_configState = configState;
}

void DeviceManager::updateAwaitingGameStartData()
{
  GameStartData data{.totalPlayers = m_interface->getTotalPlayers()};
  m_displayManager->updateAwaitingGameStartData(data);
}

void DeviceManager::updateTurnSequence()
{
  TurnSequenceData data{
      .totalPlayers = m_interface->getTotalPlayers(),
      .myPlayerIndex = m_interface->getMyPlayer(),
      .currentPlayerIndex = m_interface->getCurrentPlayer(),
      .skippedPlayers = m_interface->getSkippedPlayers()};
  m_displayManager->updateTurnSequenceData(data);
}

void DeviceManager::registerCallbacks()
{
  m_interface->registerDeviceNameChangedCallback([this](char *name)
                                                 { this->onDeviceNameChanged(name); });

  m_interface->registerDeviceColorConfigChangedCallback([this](ColorConfig config)
                                                        { this->onDeviceColorConfigChanged(config); });
  m_interface->registerDeviceNameWriteCallback([this](bool write)
                                               { this->onDeviceNameWriteChanged(write); });
  m_interface->registerDeviceColorConfigStateChangeCallback([this](DeviceState::State state)
                                                            { this->onDeviceColorConfigStateChanged(state); });
  m_interface->registerDeviceColorConfigWriteCallback([this](bool write)
                                                      { this->onDeviceColorConfigWriteChanged(write); });
}

void DeviceManager::updateDisplayMode()
{
  logger.info(loggerTag, ": Updating display mode to: ", static_cast<int>(m_deviceState));
  m_displayManager->setDisplayMode(m_deviceState);
}

void DeviceManager::update()
{
  unsigned long currentTime = millis();
  unsigned long deltaTime = currentTime - m_lastUpdate;
  m_lastUpdate = currentTime;

  if (currentTime - m_lastReadOut > 2000)
  {
    logger.info(loggerTag, ": Update Period: ", deltaTime);

    if (ENABLE_DEBUG)
      m_interface->readData();

    m_lastReadOut = currentTime;
  }

#ifndef SIMULATOR
  m_interface->poll();
#endif
  processGameState();
  updateDisplay();
}

void DeviceManager::processGameState()
{
  ButtonInputType buttonAction = m_buttonMonitor->getAction();

  if (m_started)
  {
    logger.error(loggerTag, ": Attempting to update before running `start()`");
    while (1)
      ;
  }

  if (m_deviceState == DeviceState::State::AwaitingConnection &&
      m_lastUpdate - m_lastConnection > CONNECTION_TIMEOUT)
  {
    logger.debug(loggerTag, ": Entering deep sleep");
    // enterDeepSleep();
  }

  if (buttonAction == ButtonInputType::LongPress)
    toggleDeviceOrientation();

  if (buttonAction == ButtonInputType::TripleButtonPress)
    toggleColorBlindMode();

  if (!m_interface->isConnected())
  {
    m_lastDisconnection = m_lastUpdate;
    setWaitingForConnection();
    return;
  }

  m_lastConnection = m_lastUpdate;

  if (m_lastUpdate - m_lastDisconnection < EXPECTED_CHARACTERISTIC_DISCOVERY)
    return;

  bool updateDeviceState = updateCommandedDeviceState();

  if (m_deviceState == DeviceState::State::ConfigurationMode)
  {
    return;
  }

  if (DeviceState::deviceStateRequiresGameStartData(m_deviceState))
    updateAwaitingGameStartData();

  if (DeviceState::deviceStateRequiresTurnSequenceData(m_deviceState))
    updateTurnSequence();

  if (DeviceState::deviceStateRequiresTimeData(m_deviceState))
    updateTimer();

  if (updateDeviceState)
    m_displayManager->setDisplayMode(m_deviceState);

  if (buttonAction == ButtonInputType::DoubleButtonPress &&
      DeviceState::isStateSkipEligible(m_deviceState))
  {
    m_interface->toggleSkippedState();
  }

  if (isActiveTurn() && buttonAction == ButtonInputType::ButtonPress)
    sendEndTurn();
}
