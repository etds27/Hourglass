#pragma once
#include "hg_display_interface.h"
#include "logger.h"
#include <vector>
#include <functional>

class HourglassDisplayManager {
    public:
    /// @brief Add a display interface to the collection
    void addDisplayInterface(HGDisplayInterface *displayInterface);

    /// @brief Get the number of display interfaces managed
    /// @return Number of display interfaces
    uint8_t count();
    
    // Sets the current colorblind mode for the ringlight
    void setColorBlindMode(bool colorBlindMode);

    /// @brief Sets the current absolute/player orientation for the displays
    /// @param orientation Set to `true` for absolute orientation and `false` for player orientation
    void setAbsoluteOrientation(bool orientation);

    // Sets the ring light to the appropriate lighting mode associated with the provided device state
    virtual void setDisplayMode(DeviceState::State state);

    // Updates the current light display according to the known device state and state properties
    // params:
    //    - force: (bool) Allow the user to refresh the ring light even if within the refresh rate limit
    void update(bool force = false);

    void setUp();

    // Update the data for the timer prior to updating the display
    void updateTimerData(struct TimerData data);
    void updateTurnSequenceData(struct TurnSequenceData data);
    void updateAwaitingGameStartData(struct GameStartData data);
    
    void updatePrimaryColor(uint32_t color);
    void updateAccentColor(uint32_t accentColor);

    private:
        std::vector<HGDisplayInterface*> interfaces;

        void applyToAllInterfaces(std::function<void(HGDisplayInterface*)> func);
};