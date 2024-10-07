#include "ble_interface.h"
#include "logger.h"


BLEInterface::BLEInterface(char* deviceName) {
  m_deviceName = deviceName;
};

bool BLEInterface::isConnected() {
  // logger.info("Checking connection");
  bool connected = BLE.connected();
  // logger.info("Connection " + String(connected));
  return connected;
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

  // Number of players
  char* dummyUuid = "0000";
  BLEIntCharacteristic* dummyCharacteristic = new BLEIntCharacteristic(dummyUuid, BLERead | BLEWrite);
  // BLEDescriptor numPlayersDescriptor = BLEDescriptor("6137", "Number of players attached to the host");
  // m_numberOfPlayers->addDescriptor(numPlayersDescriptor);
  m_service->addCharacteristic(*dummyCharacteristic);

  // Number of players
  char* numPlayerUuid = "0001";
  m_numberOfPlayers = new BLEIntCharacteristic(numPlayerUuid, BLERead | BLEWrite);
  // BLEDescriptor numPlayersDescriptor = BLEDescriptor("6137", "Number of players attached to the host");
  // m_numberOfPlayers->addDescriptor(numPlayersDescriptor);
  m_service->addCharacteristic(*m_numberOfPlayers);

  // Current player
  char* currentPlayerUuid = "0002";
  m_currentPlayer = new BLEIntCharacteristic(currentPlayerUuid, BLEWrite | BLERead);
  m_service->addCharacteristic(*m_currentPlayer);

  // Timer
  char* timerUuid = "0003";
  m_timer = new BLEIntCharacteristic(timerUuid, BLEWrite | BLERead);
  m_service->addCharacteristic(*m_timer);

  // Device's player number
  char* myPlayerUuid = "0004";
  m_myPlayerNumber = new BLEIntCharacteristic(myPlayerUuid, BLEWrite | BLERead);
  m_service->addCharacteristic(*m_myPlayerNumber);

  // My turn bool
  char* myTurnUuid = "0005";
  m_myTurn = new BLEBoolCharacteristic(myTurnUuid, BLEWrite | BLENotify | BLERead);
  BLEDescriptor myTurnDescriptor = BLEDescriptor("1394", "Bool to indicate that it is the player's turn");
  m_myTurn->addDescriptor(myTurnDescriptor);
  m_service->addCharacteristic(*m_myTurn);

  // Device is skipped bool
  char* skipUuid = "0006";
  m_skipped = new BLEBoolCharacteristic(skipUuid, BLERead | BLEWrite | BLENotify | BLERead);
  m_service->addCharacteristic(*m_skipped);
  m_skipped->writeValue(false);

  // Game is currently active
  char* gameActiveUuid = "0007";
  m_gameActive = new BLEBoolCharacteristic(gameActiveUuid, BLEWrite | BLERead);
  m_service->addCharacteristic(*m_gameActive);

  logger.info("Advertising with name: ");
  logger.info(String(m_deviceName));
  BLE.setLocalName(m_deviceName);
  BLE.setAdvertisedService(*m_service);
  BLE.addService(*m_service);

  m_currentPlayer->writeValue(2);
  m_timer->writeValue(10000);
  m_myPlayerNumber->writeValue(4);



  BLE.advertise();
  logger.info("Connected");
}

void BLEInterface::readData() {
  if (!isConnected()) {
    logger.warning("BLE interface not connected");
    return;
  }
  logger.info(String(m_numberOfPlayers->valueLength()));
  logger.info("Num Players:    " + String(getTotalPlayers()) + " " + String(m_numberOfPlayers->valueLength()));
  logger.info("Current Player: " + String(getCurrentPlayer()) + " " + String(m_currentPlayer->valueLength()));
  logger.info("Timer:          " + String(getTimer()) + " " + String(m_timer->valueLength()));
  logger.info("My Turn:        " + String(isTurn()) + " " + String(m_myTurn->valueLength()));
  logger.info("My number:      " + String(getMyPlayer()) + " " + String(m_myPlayerNumber->valueLength()));
  logger.info("Skipped:        " + String(getSkipped()) + " " + String(m_skipped->valueLength()));
  logger.info("\n");
}
