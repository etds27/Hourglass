#include "ble_interface.h"
#include "logger.h"

const char *SERVICE_UUID = "d7560343-51d4-4c24-a0fe-118fd9078144";
const char *DUMMY_UUID = "22a80d5a-0c4f-4019-8515-1708df13747d";
const char *TOTAL_PLAYERS_UUID = "d776071e-9584-42db-b095-798a90049ee0";
const char *CURRENT_PLAYER_UUID = "6efe0bd2-ad04-49bb-8436-b7e1d1902fea";
const char *MY_PLAYER_UUID = "f1223124-c708-4b98-a486-48515fa59d3d";
const char *MY_TURN_UUID = "c27802ab-425e-4b15-8296-4a937da7125f";
const char *REMAINING_TIME_UUID = "4e1c05f6-c128-4bca-96c3-29c014e00eb6";
const char *SKIPPED_UUID = "c1ed8823-7eb1-44b2-ac01-351e8c6a693c";
const char *TIMER_UUID = "4661b4c1-093d-4db7-bb80-5b5fe3eae519";
const char *GAME_ACTIVE_UUID = "33280653-4d71-4714-a03c-83111b886aa7";
const char *GAME_PAUSED_UUID = "643fda83-0c6b-4e8e-9829-cbeb20b70b8d";

const int BLE_POLL_RATE = 50;

BLEInterface::BLEInterface(char *deviceName)
{
  if (!BLE.begin())
  {
    Serial.println("Starting BLE failed!");
    while (1)
      ; // Stay here if initialization fails
  }
  m_deviceName = deviceName;
  lastPoll = millis();
};

bool BLEInterface::isConnected()
{
  BLE.poll();
  return BLE.connected();
}

bool BLEInterface::isTurn()
{
  m_lastMyTurn = m_myTurn->value();
  return m_lastMyTurn;
}

void BLEInterface::endTurn()
{
  m_myTurn->writeValue(false);
}

int BLEInterface::getTimer()
{
  m_lastTimer = m_timer->value();
  return m_lastTimer;
}

int BLEInterface::getElapsedTime()
{
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

bool BLEInterface::getSkipped()
{
  m_lastSkipped = m_skipped->value();
  return m_lastSkipped;
}

void BLEInterface::setSkipped()
{
  m_skipped->writeValue(true);
}

void BLEInterface::unsetSkipped()
{
  m_skipped->writeValue(false);
}

bool BLEInterface::isGameActive()
{
  return m_gameActive->value();
}

bool BLEInterface::isGamePaused()
{
  return m_gamePaused->value();
}

void BLEInterface::poll()
{
  if (millis() - lastPoll > BLE_POLL_RATE)
  {
    BLE.poll();
  }
}

void BLEInterface::setService(uint8_t serviceIndex)
{

  logger.info(String(SERVICE_UUID));
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

  // My turn bool
  m_myTurn = new BLEBoolCharacteristic(MY_TURN_UUID, BLEWrite | BLENotify | BLERead);
  BLEDescriptor myTurnDescriptor = BLEDescriptor("1394", "Bool to indicate that it is the player's turn");
  m_myTurn->addDescriptor(myTurnDescriptor);
  m_service->addCharacteristic(*m_myTurn);

  // Device is skipped bool
  m_skipped = new BLEBoolCharacteristic(SKIPPED_UUID, BLERead | BLEWrite | BLENotify);
  m_service->addCharacteristic(*m_skipped);
  m_skipped->writeValue(false);

  // Game is currently active
  m_gameActive = new BLEBoolCharacteristic(GAME_ACTIVE_UUID, BLEWrite | BLERead);
  m_service->addCharacteristic(*m_gameActive);

  // Game is currently paused
  m_gamePaused = new BLEBoolCharacteristic(GAME_PAUSED_UUID, BLEWrite | BLERead);
  m_service->addCharacteristic(*m_gamePaused);

  logger.info("Advertising with name: " + String(m_deviceName));
  BLE.setLocalName(m_deviceName);
  BLE.setAdvertisedService(*m_service);
  BLE.addService(*m_service);

  m_currentPlayer->writeValue(2);
  m_timer->writeValue(10000);
  m_myPlayerNumber->writeValue(4);

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
  // logger.info("Dummy: " + String(m_dummy->valueLength()));
  logger.info("Num Players:    " + String(getTotalPlayers()) + " " + String(m_numberOfPlayers->valueLength()));
  logger.info("Current Player: " + String(getCurrentPlayer()) + " " + String(m_currentPlayer->valueLength()));
  logger.info("Timer:          " + String(getTimer()) + " " + String(m_timer->valueLength()));
  logger.info("My Turn:        " + String(isTurn()) + " " + String(m_myTurn->valueLength()));
  logger.info("My number:      " + String(getMyPlayer()) + " " + String(m_myPlayerNumber->valueLength()));
  logger.info("Skipped:        " + String(getSkipped()) + " " + String(m_skipped->valueLength()));
  logger.info("Game Active:    " + String(isGameActive()) + " " + String(m_skipped->valueLength()));
  logger.info("Game Paused:    " + String(isGamePaused()) + " " + String(m_gamePaused->valueLength()));
  logger.info("\n");
}
