#pragma once
#include "device_state.h"
#include "constants.h"
#include <stdint.h>

// All required data for any display interface to show the Awaiting Game Start state
struct GameStartData
{
    int totalPlayers;
};

// All required data for any display interface to show the Active Turn state
struct TimerData
{
    int totalTime;
    int elapsedTime;
    bool isTurnTimeEnforced;
};

// All required data for any display interface to show the Awaiting Turn state
struct TurnSequenceData
{
    int totalPlayers;
    int myPlayerIndex;
    int currentPlayerIndex;
};

class HGDisplayInterface
{
protected:
    /// @brief Clears the current display before showing the updated view
    bool m_clearBeforeUpdate = true;
    virtual bool getClearBeforeUpdate() const;

    /// @brief Minimum time (in ms) to wait before the display will redraw
    uint32_t m_refreshRate = DISPLAY_REFRESH_RATE;

    DeviceState::State m_state = DeviceState::State::Off;
    bool m_colorBlindMode = false;
    bool m_absoluteOrientation = true;

    unsigned long m_startTime;
    struct TimerData m_timerData;
    struct TurnSequenceData m_turnSequenceData;
    struct GameStartData m_gameStartData;

    unsigned long m_lastUpdate;

    // MARK: Light Modes
    virtual void updateLightModeActiveTurnTimer() = 0;
    virtual void updateLightModeActiveTurnNoTimer() = 0;
    virtual void updateLightModeSkipped() = 0;
    virtual void updateLightModeTurnSequence() = 0;
    virtual void updateLightModeAwaitGameStart() = 0;
    virtual void updateLightModeAwaitConnection() = 0;
    virtual void updateGameDebug() = 0;
    // Light ring mode for when the game is paused by the companion app
    virtual void updateGamePaused() = 0;

public:
    virtual ~HGDisplayInterface();

    // Sets the current colorblind mode for the ringlight
    void setColorBlindMode(bool colorBlindMode);

    /// @brief Sets the current absolute/player orientation for the display
    /// @param orientation Set to `true` for absolute orientation and `false` for player orientation
    void setAbsoluteOrientation(bool orientation);

    // Sets the ring light to the appropriate lighting mode associated with the provided device state
    virtual void setDisplayMode(DeviceState::State state);

    // Updates the current light display according to the known device state and state properties
    // params:
    //    - force: (bool) Allow the user to refresh the ring light even if within the refresh rate limit
    void update(bool force = false);

    void setUp();

    virtual void begin() = 0;
    virtual void clear() = 0;
    virtual void show() = 0;

    // Update the data for the timer prior to updating the display
    void updateTimerData(struct TimerData data);
    void updateTurnSequenceData(struct TurnSequenceData data);
    void updateAwaitingGameStartData(struct GameStartData data);
};