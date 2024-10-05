#include "BLEProperty.h"
#include "BLEDescriptor.h"
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

int BLEInterface::getTimer()  {
  return m_timer->value();
}

void BLEInterface::setService(uint8_t serviceIndex) {
  char* serviceUuid = "d7560343-51d4-4c24-a0fe-118fd9078144";
  char* endTurnUuid = "c265";
  char* numPlayerUuid = "7b52";
  char* currentPlayerUuid = "69a1";
  char* timerUuid = "45e2";
  char* myPlayerUuid = "ae98";
  char* myTurnUuid = "ef79";

  
  logger.info(String(serviceUuid));
  m_service = new BLEService(serviceUuid);
  m_endTurn = new BLEBoolCharacteristic(endTurnUuid, BLERead | BLEWrite | BLENotify);
  m_numberOfPlayers = new BLEIntCharacteristic(numPlayerUuid, BLERead | BLEWrite | BLENotify);
  m_currentPlayer = new BLEIntCharacteristic(currentPlayerUuid, BLERead | BLEWrite | BLENotify);
  m_timer = new BLEIntCharacteristic(timerUuid, BLERead | BLEWrite);
  m_myPlayerNumber = new BLEIntCharacteristic(myPlayerUuid, BLERead);
  m_myTurn = new BLEBoolCharacteristic(myTurnUuid, BLEWrite | BLENotify);


  BLE.setLocalName(m_deviceName);
  BLE.setAdvertisedService(*m_service);

  BLEDescriptor endTurnDescriptor = BLEDescriptor(endTurnUuid, "Bool to indicate the client has ended their turn");
  m_endTurn->addDescriptor(endTurnDescriptor);

  BLEDescriptor numPlayersDescriptor = BLEDescriptor(numPlayerUuid, "Number of players attached to the host");
  m_numberOfPlayers->addDescriptor(numPlayersDescriptor);

  BLEDescriptor myTurnDescriptor = BLEDescriptor("1394", "Bool to indicate that it is the player's turn");
  m_myTurn->addDescriptor(myTurnDescriptor);

  m_endTurn->writeValue(true);
  m_numberOfPlayers->writeValue(10);
  m_timer->writeValue(10000);

  m_service->addCharacteristic(*m_endTurn);
  m_service->addCharacteristic(*m_numberOfPlayers);
  m_service->addCharacteristic(*m_currentPlayer);
  m_service->addCharacteristic(*m_timer);
  m_service->addCharacteristic(*m_myPlayerNumber);
  m_service->addCharacteristic(*m_myTurn);

  logger.info("Advertising");
  BLE.addService(*m_service);
  BLE.advertise();
  logger.info("Connected");
}

void BLEInterface::readData() {
  bool turnEnd = m_endTurn->value();
  logger.info("End Turn: " + String(turnEnd));
  int numPlayers = m_numberOfPlayers->value();
  logger.info("Num Players: " + String(numPlayers));
  int currentPlayer = m_currentPlayer->value();
  logger.info("Current Player: " + String(currentPlayer));
  int timer = m_timer->value();
  logger.info(String("Timer: " + String(timer)));
  int myTurn = m_myTurn->value();
  logger.info(String("My Turn: " + String(myTurn)));
  logger.info("\n");
}


