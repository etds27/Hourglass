#ifdef SIMULATOR
#include "simulator_central_interface.h"
#include "simulator_tools.h"
#include "logger.h"

const int BLE_POLL_RATE = 50;

SimulatorCentralInterface::SimulatorCentralInterface(char *deviceName)
{
    m_deviceName = deviceName;
    lastPoll = millis();
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

void SimulatorCentralInterface::setService()
{
}

void SimulatorCentralInterface::poll()
{
}

void SimulatorCentralInterface::readData()
{
    if (!isConnected())
    {
        logger.warning("BLE interface not connected");
        return;
    }
    // logger.info("Dummy: " + String(m_dummy->valueLength()));
    logger.info("Num Players:    " + std::to_string(getTotalPlayers()));
    logger.info("Current Player: " + std::to_string(getCurrentPlayer()));
    logger.info("Timer:          " + std::to_string(getTimer()));
    logger.info("Elapsed Time:   " + std::to_string(getElapsedTime()));
    logger.info("My Turn:        " + std::to_string(isTurn()));
    logger.info("My number:      " + std::to_string(getMyPlayer()));
    logger.info("Skipped:        " + std::to_string(isSkipped()));
    logger.info("Game Active:    " + std::to_string(isGameActive()));
    logger.info("Game Paused:    " + std::to_string(isGamePaused()));
    logger.info("\n");
}

#endif