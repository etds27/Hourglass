#pragma once

#ifdef SIMULATOR
#include <iostream>
#include <string>
using LogString = std::string;
#else
#include <Arduino.h>
using LogString = String;
#endif

enum LoggerLevel {
  DEBUG = 0,
  INFO = 1,
  WARNING = 2,
  ERROR = 3,
  OFF = 4
};

// General purpose logging class to use within the project
class Logger {
public:
  void debug(LogString message);
  void info(LogString message);
  void warning(LogString message);
  void error(LogString message);

private:
  void log(LogString message, LoggerLevel level);
};

extern Logger logger;
extern LoggerLevel loggerLevel;