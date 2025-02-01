#pragma once
#include "device_state.h"
#include <stdint.h>

class HGCentralInterface
{
public:
    virtual void readData() = 0;
    virtual bool isConnected() = 0;
    virtual bool isTurn() = 0;
    virtual void endTurn() = 0;
    virtual void toggleSkippedState() = 0;


    virtual int getTimer() = 0;
    virtual int getElapsedTime();

    /// @brief Get the expected elapsed time using the last elapsed time report and the time difference between that report and now
    /// @return Expected elapsed time
    unsigned long getExpectedElapsedTime();
    virtual int getCurrentPlayer() = 0;
    virtual int getTotalPlayers() = 0;

    // Gets the current device's player index
    virtual int getMyPlayer() = 0;

    // Check if a game is active
    virtual bool isGameActive() = 0;

    // Check if a game is paused
    virtual bool isGamePaused() = 0;

    virtual DeviceState::State getCommandedDeviceState() = 0;


    /// @brief Get the current skipped players in the turn sequence
    /// Each bit represents the skipped status of each player
    /// The first player in the sequence is the LSB
    /// @return A 16-bit integer representing all skipped players in the game
    virtual uint16_t getSkippedPlayers() = 0;


    // Check if the turn timer should be enforced
    virtual bool isTurnTimerEnforced() = 0;

    virtual void setService() = 0;


protected:
    virtual ~HGCentralInterface();

    char *m_deviceName;
    unsigned int lastPoll;

    int m_lastTimer;
    int m_lastElapsedTime;
    int m_lastCurrentPlayer;
    int m_lastTotalPlayers;
    int m_lastMyPlayer;
    bool m_lastMyTurn;
    bool m_lastSkipped;
    bool m_lastGameActive;
    bool m_lastGamePaused;
    bool m_lastEnforceTurnTimer;
    unsigned long m_lastElapsedTimePoll;
    DeviceState::State m_deviceState;
};