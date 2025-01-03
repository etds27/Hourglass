#ifdef SIMULATOR
#include <chrono>

unsigned long m_startTime = std::chrono::system_clock::now().time_since_epoch().count();

unsigned long millis() {
    return std::chrono::system_clock::now().time_since_epoch().count() - m_startTime;
}

#endif