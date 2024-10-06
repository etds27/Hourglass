#include <EEPROM.h>
#include "device_manager.h"
#include "logger.h"


DeviceManager::DeviceManager() {
  logger.info("Initializing Device Manager");
  m_deviceName = getDeviceName();
  m_interface = new BLEInterface(m_deviceName);
  m_buttonMonitor = new ButtonInputMonitor(BUTTON_INPUT_PIN);
  m_ring = new RingLight(RING_LED_COUNT, RING_DI_PIN, NEO_GRB + NEO_KHZ800);
}

void DeviceManager::start() {
  m_ring->setBrightness(5);
  m_ring->setLightMode(DeviceState::Off);

  m_interface->setService(0);

  lastUpdate = millis();
}

char* DeviceManager::getDeviceName() {
  return m_deviceName;
}

char* DeviceManager::readDeviceName() {
  int i;
  // Read the arduinos ID
  char arduinoID[8];
  unsigned char value;
  for (i = 0; i < 8; i++) {
    value = EEPROM.read(i);
    arduinoID[i] = value;
  }
  arduinoID[i] = '\0';
  logger.info("Device name: " + String(arduinoID));
  return arduinoID;
}

void DeviceManager::writeDeviceName(char* deviceName, uint8_t length) {
  // Write unique ID to EEPROM
  int i;
  logger.info("Writing Device Name: " + String(deviceName));
  for (i = 0; i < length; i++) {
    EEPROM.write(i, deviceName[i]);
  }
  EEPROM.write(i, '\0');
  logger.info("Wrote device name to EEPROM: " + String(deviceName));
  m_deviceName = deviceName;
}

bool DeviceManager::isActiveTurn() {
  return m_deviceState == DeviceState::ActiveTurn;
}

// Transition from the active turn to 'turn sequence' mode
void DeviceManager::endTurn() {
  m_interface->endTurn();
  setTurnSequenceMode();
}

void DeviceManager::startTurn() {
  logger.info("Turn Started");
  logger.info("Setting device state to: AwaitingGameStart");
  m_deviceState = DeviceState::ActiveTurn;
  updateTimer();
  updateRingMode();
}

void DeviceManager::setSkipped() {
  m_interface->setSkipped();
  m_ring->setLightMode(DeviceState::Skipped);
}

void DeviceManager::unsetSkipped() {
  m_interface->unsetSkipped();
  if (isActiveTurn()) {
    m_ring->setLightMode(DeviceState::ActiveTurn);
  } else {
    m_ring->setLightMode(DeviceState::AwaitingTurn);
  }
}

void DeviceManager::updateTimer() {
  int timer = m_interface->getTimer();
  struct TimerData data = { .totalTime = timer, .remainingTme = 1 };
  m_ring->updateTimerData(data);
}

void DeviceManager::setTurnSequenceMode() {
  m_deviceState = DeviceState::AwaitingTurn;
  updateTurnSequence();
  updateRingMode();
}

void DeviceManager::setAwaitGameStart() {
  if (m_deviceState == DeviceState::AwaitingGameStart) {
    return;
  }
  logger.info("Setting device state to: AwaitingGameStart");
  m_deviceState = DeviceState::AwaitingGameStart;
  updateRingMode();
}

void DeviceManager::setWaitingForConnection() {
  if (m_deviceState == DeviceState::AwaitingConnecion) {
    return;
  }
  logger.info("Setting device state to: AwaitingConnecion");
  m_deviceState = DeviceState::AwaitingConnecion;
  updateRingMode();}


void DeviceManager::updateTurnSequence() {
  int totalPlayers = m_interface->getTotalPlayers();
  int myPlayer = m_interface->getMyPlayer();
  int currentPlayer = m_interface->getCurrentPlayer();

  struct TurnSequenceData data = { .totalPlayers = totalPlayers, .myPlayerIndex = myPlayer, .currentPlayerIndex = currentPlayer };
  m_ring->updateTurnSequenceData(data);
}

void DeviceManager::updateRingMode() {
  m_ring->setLightMode(m_deviceState);
}

void DeviceManager::update() {
  // Get all information about device inputs and device interface
  unsigned long currentTime = millis();
  unsigned long deltaTime = currentTime - lastUpdate;
  lastUpdate = currentTime;

  if (m_started) {
    logger.error("Attempting to update the device manager before running `start()`");
    while(1);
  }

  if (!m_interface->isConnected()) {
    setWaitingForConnection();
    m_ring->update();
    return;
  }

  bool isGameActive = m_interface->isGameActive();
  if (!isGameActive) {
    setAwaitGameStart();
    m_ring->update();
    return;
  }

  ButtonInputType buttonAction = m_buttonMonitor->getAction();
  bool interfaceTurn = m_interface->isTurn();
  int currentPlayer = m_interface->getCurrentPlayer();
  int totalPlayers = m_interface->getTotalPlayers();
  bool isSkipped = m_interface->getSkipped();
  bool isTurn = m_deviceState == DeviceState::ActiveTurn;
  logger.info("String message");
  // Game has started and device state needs to be updated
  if (m_deviceState == DeviceState::AwaitingGameStart) {
    // This means we are the first player to go in the game. Immediately start the turn
    if (interfaceTurn) {
      startTurn();
    } else {
      // Otherwise go into Awaiting Turn mode
      setTurnSequenceMode();
    }
  }

  // Log data from the interface
  if (millis() % 2000 == 0 && m_interface->isConnected()) {
    m_interface->readData();
  }

  if (buttonAction == ButtonInputType::DoubleButtonPress) {
    // If a double press action was detected
    if (isSkipped) {
      unsetSkipped();
    } else {
      if (isActiveTurn()) {
        endTurn();
      }
      setSkipped();
    }
  }

  if (buttonAction == ButtonInputType::ButtonPress && isTurn) {
    // If a button was pressed and it is the person's turn
    endTurn();
  }

  if (interfaceTurn != isTurn && !isTurn) {
    // If the turn just started
    startTurn();
  } else if (m_interface->isTurn() != isTurn && isTurn) {
    // If the turn just ended
    endTurn();
  }
  m_ring->update();
}
