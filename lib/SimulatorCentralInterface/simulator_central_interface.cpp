#ifdef SIMULATOR
#include "simulator_central_interface.h"
#include "simulator_tools.h"
#include "logger.h"

const int BLE_POLL_RATE = 50;

SimulatorCentralInterface::SimulatorCentralInterface(char *deviceName)
{
    m_deviceName = deviceName;
    lastPoll = millis();

    m_currentPlayer = 4;
    m_myPlayerNumber = 2;
    m_numberOfPlayers = 8;
    m_skippedPlayers = 2;
    m_skippedPlayers = m_skippedPlayers | (1 << 7);
    m_skippedPlayers = m_skippedPlayers | (1 << 5);
};

bool SimulatorCentralInterface::isConnected()
{
    return true;
}

bool SimulatorCentralInterface::isTurn()
{
    return m_lastMyTurn;
}

void SimulatorCentralInterface::endTurn()
{
    m_myTurn = false;
}

int SimulatorCentralInterface::getTimer()
{
    m_lastTimer = m_timer;
    return m_lastTimer;
}

int SimulatorCentralInterface::getElapsedTime()
{
    m_lastElapsedTime = m_elapsedTime;
    return m_lastElapsedTime;
}

int SimulatorCentralInterface::getMyPlayer()
{
    m_lastMyPlayer = m_myPlayerNumber;
    return m_lastMyPlayer;
}

int SimulatorCentralInterface::getCurrentPlayer()
{
    m_lastCurrentPlayer = m_currentPlayer;
    return m_lastCurrentPlayer;
}

int SimulatorCentralInterface::getTotalPlayers()
{
    m_lastTotalPlayers = m_numberOfPlayers;
    return m_lastTotalPlayers;
}

bool SimulatorCentralInterface::isSkipped()
{
    return m_skipped;
}

void SimulatorCentralInterface::setSkipped()
{
    m_skipped = true;
}

void SimulatorCentralInterface::unsetSkipped()
{
    m_skipped = false;
}

bool SimulatorCentralInterface::isGameActive()
{
    return m_gameActive;
}

bool SimulatorCentralInterface::isGamePaused()
{
    return m_gamePaused;
}

bool SimulatorCentralInterface::isTurnTimerEnforced()
{
    return m_enforceTurnTimer;
}

uint16_t SimulatorCentralInterface::getSkippedPlayers()
{
    return m_skippedPlayers;
}

void SimulatorCentralInterface::setService()
{
}

void SimulatorCentralInterface::poll()
{
}

void SimulatorCentralInterface::toggleSkippedState() {
    m_skipped = !m_skipped;
}

DeviceState::State SimulatorCentralInterface::getCommandedDeviceState() {
    return DeviceState::State::AwaitingTurn;
}

void SimulatorCentralInterface::readData()
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
    logger.info("My Turn:        ", isTurn());
    logger.info("My number:      ", getMyPlayer());
    logger.info("Skipped:        ", isSkipped());
    logger.info("Game Active:    ", isGameActive());
    logger.info("Game Paused:    ", isGamePaused());
    logger.info("\n");
}

#endif