#include "hg_display_manager.h"

void HourglassDisplayManager::addDisplayInterface(HGDisplayInterface *displayInterface)
{
    interfaces.push_back(displayInterface);
}

uint8_t HourglassDisplayManager::count()
{
    return interfaces.size();
}

void HourglassDisplayManager::setColorBlindMode(bool colorBlindMode)
{
    applyToAllInterfaces([colorBlindMode](HGDisplayInterface *interface)
                         { interface->setColorBlindMode(colorBlindMode); });
}

void HourglassDisplayManager::setAbsoluteOrientation(bool orientation)
{
    applyToAllInterfaces([orientation](HGDisplayInterface *interface)
                         { interface->setAbsoluteOrientation(orientation); });
}

void HourglassDisplayManager::setDisplayMode(DeviceState::State state)
{
    applyToAllInterfaces([state](HGDisplayInterface *interface)
                         { interface->setDisplayMode(state); });
}

void HourglassDisplayManager::update(bool force)
{
    applyToAllInterfaces([force](HGDisplayInterface *interface)
                         { interface->update(force); });
}

void HourglassDisplayManager::setUp()
{
    applyToAllInterfaces([](HGDisplayInterface *interface)
                         { interface->setUp(); });
}

void HourglassDisplayManager::updateTimerData(TimerData data)
{
    applyToAllInterfaces([data](HGDisplayInterface *interface)
                         { interface->updateTimerData(data); });
}

void HourglassDisplayManager::updateTurnSequenceData(TurnSequenceData data)
{
    applyToAllInterfaces([data](HGDisplayInterface *interface)
                         { interface->updateTurnSequenceData(data); });
}

void HourglassDisplayManager::updateAwaitingGameStartData(GameStartData data)
{
    applyToAllInterfaces([data](HGDisplayInterface *interface)
                         { interface->updateAwaitingGameStartData(data); });
}

void HourglassDisplayManager::updatePrimaryColor(uint32_t color)
{
    applyToAllInterfaces([color](HGDisplayInterface *interface)
                         { interface->updatePrimaryColor(color); });
}

void HourglassDisplayManager::updateAccentColor(uint32_t accentColor)
{
    applyToAllInterfaces([accentColor](HGDisplayInterface *interface)
                         { interface->updateAccentColor(accentColor); });
}


void HourglassDisplayManager::applyToAllInterfaces(std::function<void(HGDisplayInterface *)> func)
{
    for (HGDisplayInterface *interface : interfaces)
    {
        func(interface);
    }
}
