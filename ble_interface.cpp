#include "ble_interface.h"
#include "logger.h"


BLEInterface::BLEInterface(char* deviceName) {
  m_deviceName = deviceName;
};

bool BLEInterface::isConnected() {
  return BLE.connected();
}

bool BLEInterface::isTurn() {
  return m_myTurn->value();
}

void BLEInterface::endTurn() {
  m_myTurn->writeValue(false);
}

int BLEInterface::getTimer() {
  return m_timer->value();
}

int BLEInterface::getMyPlayer() {
  return m_myPlayerNumber->value();
}

int BLEInterface::getCurrentPlayer() {
  return m_currentPlayer->value();
}

int BLEInterface::getTotalPlayers() {
  return m_numberOfPlayers->value();
}

bool BLEInterface::getSkipped() {
  return m_skipped->value();
}

void BLEInterface::setSkipped() {
  m_skipped->writeValue(true);
}

void BLEInterface::unsetSkipped() {
  m_skipped->writeValue(false);
}

bool BLEInterface::isGameActive() {
  return m_gameActive->value();
}



void BLEInterface::setService(uint8_t serviceIndex) {

  char* serviceUuid = "d7560343-51d4-4c24-a0fe-118fd9078144";
  logger.info(String(serviceUuid));
  m_service = new BLEService(serviceUuid);
  BLE.setLocalName(m_deviceName);
  BLE.setAdvertisedService(*m_service);

  // Number of players
  char* numPlayerUuid = "418a";
  m_numberOfPlayers = new BLEIntCharacteristic(numPlayerUuid, BLERead | BLEWrite);
  m_numberOfPlayers->writeValue(10);
  // BLEDescriptor numPlayersDescriptor = BLEDescriptor("6137", "Number of players attached to the host");
  // m_numberOfPlayers->addDescriptor(numPlayersDescriptor);
  m_service->addCharacteristic(*m_numberOfPlayers);

  // Current player
  char* currentPlayerUuid = "69a1";
  m_currentPlayer = new BLEIntCharacteristic(currentPlayerUuid, BLEWrite | BLERead);
  m_currentPlayer->writeValue(2);
  m_service->addCharacteristic(*m_currentPlayer);

  // Timer
  char* timerUuid = "45e2";
  m_timer = new BLEIntCharacteristic(timerUuid, BLEWrite | BLERead);
  m_service->addCharacteristic(*m_timer);
  m_timer->writeValue(10000);

  // Device's player number
  char* myPlayerUuid = "ae98";
  m_myPlayerNumber = new BLEIntCharacteristic(myPlayerUuid, BLEWrite | BLERead);
  m_currentPlayer->writeValue(4);
  m_service->addCharacteristic(*m_myPlayerNumber);

  // My turn bool
  char* myTurnUuid = "ef79";
  m_myTurn = new BLEBoolCharacteristic(myTurnUuid, BLEWrite | BLENotify | BLERead);
  BLEDescriptor myTurnDescriptor = BLEDescriptor("1394", "Bool to indicate that it is the player's turn");
  m_myTurn->addDescriptor(myTurnDescriptor);
  m_service->addCharacteristic(*m_myTurn);

  // Device is skipped bool
  char* skipUuid = "f71a";
  m_skipped = new BLEBoolCharacteristic(skipUuid, BLERead | BLEWrite | BLENotify | BLERead);
  m_skipped->writeValue(false);
  m_service->addCharacteristic(*m_skipped);

  // Game is currently active
  char* gameActiveUuid = "ab38";
  m_gameActive = new BLEBoolCharacteristic(gameActiveUuid, BLEWrite | BLERead);
  m_service->addCharacteristic(*m_gameActive);

  logger.info("Advertising");
  BLE.addService(*m_service);
  BLE.advertise();
  logger.info("Connected");
}

void BLEInterface::readData() {
  logger.info("Num Players:    " + String(m_numberOfPlayers->value()));
  logger.info("Current Player: " + String(m_currentPlayer->value()));
  logger.info("Timer:          " + String(m_timer->value()));
  logger.info("My Turn:        " + String(m_myTurn->value()));
  logger.info("My number:      " + String(m_myPlayerNumber->value()));
  logger.info("Skipped:        " + String(m_skipped->value()));
  logger.info("\n");
}
