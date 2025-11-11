#pragma once
#include "device_state.h"
#include "constants.h"
#include <stdint.h>

#include "device_config.h"

/// @brief All required data for any display interface to show the Awaiting Game Start state
struct GameStartData
{
    int totalPlayers;
};

/// @brief All required data for any display interface to show the Active Turn state
struct TimerData
{
    /// @brief Total duration of the timer
    int totalTime;
    /// @brief Current time on the timer
    int elapsedTime;
    /// @brief Used to determine which Buzzer winner screen to show the winning player
    bool isTurnTimeEnforced;
};

/// @brief All required data for any display interface to show the Awaiting Turn state
struct TurnSequenceData
{
    int totalPlayers;
    int myPlayerIndex;
    int currentPlayerIndex;
    uint16_t skippedPlayers;
};

/// @brief All required data for any display interface to show the BuzzerTurnEnd state
struct BuzzerResultsData
{
    /// @brief Device's index as known by the central device
    uint8_t myPlayerIndex;
    
    /// @brief Index of the player who pressed the button first
    uint8_t winningPlayerIndex;
};

class HGDisplayInterface
{
protected:
    /// @brief Clears the current display before showing the updated view
    bool m_clearBeforeUpdate = true;
    virtual bool getClearBeforeUpdate() const;

    /// @brief Minimum time (in ms) to wait before the display will redraw
    uint32_t m_refreshRate = DISPLAY_REFRESH_RATE;

    uint32_t m_primaryColor = HOURGLASS_RED;
    uint32_t m_accentColor = HOURGLASS_GREEN;

    DeviceState::State m_state = DeviceState::State::Off;
    bool m_colorBlindMode = false;
    bool m_absoluteOrientation = true;

    unsigned long m_startTime;
    struct TimerData m_timerData;
    struct TurnSequenceData m_turnSequenceData;
    struct GameStartData m_gameStartData;
    struct BuzzerResultsData m_buzzerResultsData;

    unsigned long m_lastUpdate;

    ColorConfig m_colorConfig;

    virtual void loadCurrentColorConfig();

    // MARK: Light Modes

    /// @brief Display looping animation prior to game starting
    /// Must have updated AwaitingGameStartData
    virtual void updateLightModeAwaitGameStart() = 0;

    /// @brief Display looping animation before the central device has connected
    virtual void updateLightModeAwaitConnection() = 0;

    /// @brief Display buffer provided from saved debug information
    /// Must have updated Debug information
    virtual void updateGameDebug() = 0;

    /// @brief Display looping animation while the central device has the game paused
    virtual void updateGamePaused() = 0;

    // MARK: Sequential mode display functions
    /// @brief Display the sequential mode turn timer
    /// Must have updated TimerData
    virtual void updateLightModeActiveTurnTimer() = 0;

    /// @brief Display looping animation while the central device waits for the user to end their turn
    virtual void updateLightModeActiveTurnNoTimer() = 0;

    /// @brief Display looping animation while the user is skipped
    virtual void updateLightModeSkipped() = 0;

    /// @brief Display sequential turn sequence
    /// Must have updated  TurnSequenceData
    virtual void updateLightModeTurnSequence() = 0;


    // MARK: Buzzer mode display functions
    /// @brief Display the looping animation while the central device awaits a button press
    virtual void updateLightModeBuzzerAwaitingBuzz() = 0;
    
    /// @brief Display count down timer while central device awaits a button press
    /// Must have updated TimerData
    virtual void updateLightModeBuzzerAwaitingBuzzTimed() = 0;

    /// @brief Display looping animation while user uses won time
    /// Must have updated BuzzerResults data
    virtual void updateLightModeWinnerPeriod() = 0;

    /// @brief Display count down timer while user uses won time
    /// Must have updated TimerData and BuzzerResults Data
    virtual void updateLightModeWinnerPeriodTimed() = 0;

    /// @brief Display for the players who did not buzz in time
    virtual void updateLightModeBuzzerResults() = 0;

    /// @brief Display the state when waiting for a turn to start
    virtual void updateLightModeAwaitTurnStart() = 0;

    /// @brief Update the display for Device Color Mode (App Colors)
    virtual void updateDeviceColorMode() = 0;


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
    void updateBuzzerResultsData(struct BuzzerResultsData data);

    // Allows us to override the current color configuration for the display interface for live configuration updates
    /// @brief Update the color configuration for the display interface
    void updateColorConfig(ColorConfig config);

    
    /// @brief Update primary color of the display interface
    /// @param color New primary color
    void updatePrimaryColor(uint32_t color);

    /// @brief Update accent color of the display interface
    /// @param accentColor New accent color
    void updateAccentColor(uint32_t accentColor);
};