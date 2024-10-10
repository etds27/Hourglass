#include "Arduino.h"
#include "logger.h"

Logger logger = Logger();
LoggerLevel loggerLevel = LoggerLevel::INFO;

void Logger::debug(String message) {
  Logger::log(String(millis(), DEC) + ": (DEBUG)   " + message, LoggerLevel::DEBUG);
}

void Logger::info(String message) {
  Logger::log(String(millis(), DEC) + ": (INFO)    " + message, LoggerLevel::INFO);
}

void Logger::warning(String message) {
  Logger::log(String(millis(), DEC) + ": (WARNING) " + message, LoggerLevel::WARNING);
}

void Logger::error(String message) {
  Logger::log(String(millis(), DEC) + ": (ERROR)   " + message, LoggerLevel::ERROR);
}

void Logger::log(String message, LoggerLevel level) {
  if (level >= loggerLevel) {
    Serial.println(message);
  }
}

