#include "hg_central_interface.h"
#include <Arduino.h>

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
