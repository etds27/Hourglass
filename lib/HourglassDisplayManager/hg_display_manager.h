#pragma once
#include "hg_display_interface.h"
#include "logger.h"
#include <vector>
#include <functional>

class HourglassDisplayManager {
    public:
    void addDisplayInterface(HGDisplayInterface *inteface);
    
    // Sets the current colorblind mode for the ringlight
    void setColorBlindMode(bool colorBlindMode);

    // Sets the ring light to the appropriate lighting mode associated with the provided device state
    virtual void setDisplayMode(DeviceState state);

    // Updates the current light display according to the known device state and state properties
    // params:
    //    - force: (bool) Allow the user to refresh the ring light even if within the refresh rate limit
    void update(bool force = false);

    void setUp();

    // Update the data for the timer prior to updating the display
    void updateTimerData(struct TimerData data);
    void updateTurnSequenceData(struct TurnSequenceData data);
    void updateAwaitingGameStartData(struct GameStartData data);

    private:
        std::vector<HGDisplayInterface*> interfaces;

        void applyToAllInterfaces(std::function<void(HGDisplayInterface*)> func);
};