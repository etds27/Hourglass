#include "hg_display_manager.h"

void HourglassDisplayManager::addDisplayInterface(HGDisplayInterface *interface)
{
    interfaces.push_back(interface);
}

void HourglassDisplayManager::setColorBlindMode(bool colorBlindMode)
{
    applyToAllInterfaces([colorBlindMode](HGDisplayInterface *interface)
                         { interface->setColorBlindMode(colorBlindMode); });
}

void HourglassDisplayManager::setDisplayMode(DeviceState state)
{
    applyToAllInterfaces([state](HGDisplayInterface* interface) {
        interface->setDisplayMode(state);
    });
}

void HourglassDisplayManager::update(bool force)
{
        applyToAllInterfaces([force](HGDisplayInterface* interface) {
        interface->update(force);
    });
}

void HourglassDisplayManager::setUp()
{
        applyToAllInterfaces([](HGDisplayInterface* interface) {
        interface->setUp();
    });
}

void HourglassDisplayManager::updateTimerData(TimerData data)
{
        applyToAllInterfaces([data](HGDisplayInterface* interface) {
        interface->updateTimerData(data);
    });
}

void HourglassDisplayManager::updateTurnSequenceData(TurnSequenceData data)
{
        applyToAllInterfaces([data](HGDisplayInterface* interface) {
        interface->updateTurnSequenceData(data);
    });
}

void HourglassDisplayManager::updateAwaitingGameStartData(GameStartData data)
{
        applyToAllInterfaces([data](HGDisplayInterface* interface) {
        interface->updateAwaitingGameStartData(data);
    });
}

void HourglassDisplayManager::applyToAllInterfaces(std::function<void(HGDisplayInterface *)> func)
{
    for (HGDisplayInterface *interface : interfaces)
    {
        func(interface);
    }
}
