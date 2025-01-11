#pragma once
#ifdef SIMULATOR
#include <iostream>
#include <string>
#else
#include <Arduino.h>
#endif

namespace Logging {
  enum LoggerLevel {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    FAILURE = 3,
    OFF = 4
  };

  class Logger {
  public:
    void debug(const LogString& message);
    void info(const LogString& message);
    void warning(const LogString& message);
    void error(const LogString& message);
  private:
    void log(const LogString& message, Logging::LoggerLevel level);
  };
}

// Fully qualify the types here
extern Logging::Logger logger;
extern Logging::LoggerLevel loggerLevel;