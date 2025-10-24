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

void HGCentralInterface::registerDeviceNameWriteCallback(std::function<void(bool write)> callback)
{
        m_deviceNameWriteChangeCallback = callback;
}

void HGCentralInterface::registerDeviceColorConfigChangedCallback(std::function<void(ColorConfig config)> callback)
{
        m_deviceColorConfigChangeCallback = callback;
}

void HGCentralInterface::registerDeviceColorConfigStateChangeCallback(std::function<void(DeviceState::State state)> callback)
{
        m_deviceColorConfigStateChangeCallback = callback;
}

void HGCentralInterface::registerDeviceColorConfigWriteCallback(std::function<void(bool write)> callback)
{
        m_deviceColorConfigWriteChangeCallback = callback;
}
