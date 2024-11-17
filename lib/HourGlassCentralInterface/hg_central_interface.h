#pragma once

class HGCentralInterface
{
public:
    virtual void readData() = 0;
    virtual bool isConnected() = 0;
    virtual bool isTurn() = 0;
    virtual void endTurn() = 0;

    virtual int getTimer() = 0;
    virtual int getElapsedTime() = 0;
    virtual int getCurrentPlayer() = 0;
    virtual int getTotalPlayers() = 0;

    // Gets the current device's player index
    virtual int getMyPlayer() = 0;

    // Gets the current status of if the device is being skipped in the queue
    virtual bool isSkipped() = 0;
    // Lets the device tell the server that it should be skipped for future turns
    virtual void setSkipped() = 0;
    // Lets the device tell the server that it should not be skipped for future turns
    virtual void unsetSkipped() = 0;

    // Check if a game is active
    virtual bool isGameActive() = 0;

    // Check if a game is paused
    virtual bool isGamePaused() = 0;

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
};