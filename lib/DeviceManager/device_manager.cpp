#include <EEPROM.h>
#include "device_manager.h"
#include "logger.h"

#if LED_ADAFRUIT
#include "ring_light.h"
#else
#include "fast_led_light.h"
#endif

DeviceManager::DeviceManager()
{
  logger.info("Initializing Device Manager");
  m_deviceName = new char[8];
  readDeviceName(m_deviceName);
  logger.info("Device name: " + String(m_deviceName));

#if LED_ADAFRUIT
  logger.info("Loading Adafruit RingLight");
  m_ring = new RingLight(RING_LED_COUNT, RING_DI_PIN);
#else
  logger.info("Loading FastLED RingLight");
  m_ring = new FastLEDLight(RING_LED_COUNT, RING_DI_PIN);
#endif
  m_buttonMonitor = new ButtonInputMonitor(BUTTON_INPUT_PIN);
  m_interface = new BLEInterface(m_deviceName);
}

void DeviceManager::start()
{
  logger.info("Starting the device manager");
  m_ring->setBrightness(5);
  m_ring->setLightMode(DeviceState::Off);

  m_interface->setService(0);
  setWaitingForConnection();
  lastUpdate = millis();
  lastReadOut = millis();
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
  return m_deviceState == DeviceState::ActiveTurn;
}

// Transition from the active turn to 'turn sequence' mode
void DeviceManager::endTurn()
{
  m_interface->endTurn();
  setTurnSequenceMode();
}

void DeviceManager::startTurn()
{
  logger.info("Setting device state to: ActiveTurn");
  m_deviceState = DeviceState::ActiveTurn;
  updateTimer();
  updateRingMode();
}

void DeviceManager::setSkipped()
{
  if (m_deviceState == DeviceState::Skipped)
  {
    return;
  }
  logger.info("Setting device state to: Skipped");
  m_deviceState = DeviceState::Skipped;
  m_interface->setSkipped();
  updateRingMode();
}

void DeviceManager::unsetSkipped()
{
  m_interface->unsetSkipped();
  if (isActiveTurn())
  {
    startTurn();
  }
  else
  {
    setTurnSequenceMode();
  }
  updateRingMode();
}

void DeviceManager::updateTimer()
{
  int timer = m_interface->getTimer();
  int elapsedTime = m_interface->getElapsedTime();
  struct TimerData data = {.totalTime = timer, .elapsedTime = elapsedTime};
  m_ring->updateTimerData(data);
}

void DeviceManager::setTurnSequenceMode()
{
  logger.info("Setting device state to: AwaitingTurn");
  m_deviceState = DeviceState::AwaitingTurn;
  updateTurnSequence();
  updateRingMode();
}

void DeviceManager::setAwaitGameStart()
{
  if (m_deviceState == DeviceState::AwaitingGameStart)
  {
    return;
  }
  logger.info("Setting device state to: AwaitingGameStart");
  m_deviceState = DeviceState::AwaitingGameStart;
  updateRingMode();
}

void DeviceManager::setWaitingForConnection()
{
  if (m_deviceState == DeviceState::AwaitingConnecion)
  {
    return;
  }
  logger.info("Setting device state to: AwaitingConnecion");
  m_deviceState = DeviceState::AwaitingConnecion;
  updateRingMode();
}

void DeviceManager::updateRing()
{
  m_interface->poll();
  m_ring->update();
}

void DeviceManager::updateAwaitingGameStartData()
{
  struct GameStartData data = {
      .totalPlayers = m_interface->getTotalPlayers()};
  m_ring->updateAwaitingGameStartData(data);
}

void DeviceManager::updateTurnSequence()
{
  int totalPlayers = m_interface->getTotalPlayers();
  int myPlayer = m_interface->getMyPlayer();
  int currentPlayer = m_interface->getCurrentPlayer();

  struct TurnSequenceData data = {.totalPlayers = totalPlayers, .myPlayerIndex = myPlayer, .currentPlayerIndex = currentPlayer};
  m_ring->updateTurnSequenceData(data);
}

void DeviceManager::updateRingMode()
{
  m_ring->setLightMode(m_deviceState);
}

void DeviceManager::update()
{
  unsigned long currentTime = millis();
  unsigned long deltaTime = currentTime - lastUpdate;
  lastUpdate = currentTime;


  // Log data from the interface
  if (currentTime - lastReadOut > 1000)
  {
    logger.info("Update Period: " + String(deltaTime));
    m_interface->readData();
    lastReadOut = currentTime;
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

  if (!m_interface->isConnected())
  {
    setWaitingForConnection();
    return;
  }

  bool isGameActive = m_interface->isGameActive();
  if (!isGameActive)
  {
    updateAwaitingGameStartData();
    setAwaitGameStart();
    return;
  }

  // Get all information about device inputs and device interface
  bool interfaceTurn = m_interface->isTurn();
  int currentPlayer = m_interface->getCurrentPlayer();
  bool isSkipped = m_interface->getSkipped();
  bool isTurn = m_deviceState == DeviceState::ActiveTurn;
  int totalPlayers = m_interface->getTotalPlayers();

  // Game has started and device state needs to be updated
  if (m_deviceState == DeviceState::AwaitingGameStart)
  {

    // This means we are the first player to go in the game. Immediately start the turn
    if (interfaceTurn)
    {
      startTurn();
    }
    else
    {
      // Otherwise go into Awaiting Turn mode
      setTurnSequenceMode();
    }
    return;
  }

  if (buttonAction == ButtonInputType::DoubleButtonPress)
  {
    // If a double press action was detected
    if (isSkipped)
    {
      unsetSkipped();
    }
    else
    {
      if (isActiveTurn())
      {
        endTurn();
      }
      setSkipped();
    }
  }

  // If we are in a skipped state, immediately leave the program
  if (isSkipped)
  {
    return;
  }
  else if (m_deviceState == DeviceState::Skipped && !isSkipped)
  {
    unsetSkipped();
  }

  if (buttonAction == ButtonInputType::ButtonPress && isTurn)
  {
    // If a button was pressed and it is the person's turn
    endTurn();
  }

  if (isTurn)
  {
    updateTimer();
  }
  else if (interfaceTurn != isTurn && !isTurn)
  {
    // If the turn just started
    startTurn();
  }
  else if (m_interface->isTurn() != isTurn && isTurn)
  {
    // If the turn just ended
    endTurn();
  }
  else
  {
    updateTurnSequence();
  }
}
