#include <Arduino.h>

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
  void debug(String message);
  void info(String message);
  void warning(String message);
  void error(String message);

private:
  void log(String message, LoggerLevel level);
};

extern Logger logger;
extern LoggerLevel loggerLevel;