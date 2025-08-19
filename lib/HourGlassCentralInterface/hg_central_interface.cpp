#include "hg_central_interface.h"
#ifdef SIMULATOR
#include "simulator_tools.h"
#else
#include <Arduino.h>
#endif

HGCentralInterface::~HGCentralInterface() {}

int HGCentralInterface::getElapsedTime()
{
    m_lastElapsedTimePoll = millis();
    return m_lastElapsedTime;
}

unsigned long HGCentralInterface::getExpectedElapsedTime()
{
    return (unsigned long)getElapsedTime() - (millis() - m_lastElapsedTimePoll);
}

void HGCentralInterface::registerDeviceNameChangedCallback(std::function<void(char *name)> callback)
{
        m_deviceNameChangeCallback = callback;

}

void HGCentralInterface::registerDeviceColorChangedCallback(std::function<void(uint32_t color)> callback)
{
        m_deviceColorChangeCallback = callback;

}

void HGCentralInterface::registerDeviceAccentColorChangedCallback(std::function<void(uint32_t accentColor)> callback)
{
        m_deviceAccentColorChangeCallback = callback;

}
