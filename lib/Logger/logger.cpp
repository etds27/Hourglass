#include "logger.h"

// Fully qualified names
Logging::Logger logger;
Logging::LoggerLevel loggerLevel = Logging::LoggerLevel::INFO;

#ifdef SIMULATOR
#include "simulator_tools.h"

/*
void Logging::Logger::debug(const LogString& message) {
  log(message, Logging::LoggerLevel::DEBUG);
}

void Logging::Logger::info(const LogString& message) {
  log(message, Logging::LoggerLevel::INFO);
}

void Logging::Logger::warning(const LogString& message) {
  log(message, Logging::LoggerLevel::WARNING);
}

void Logging::Logger::error(const LogString& message) {
  log(message, Logging::LoggerLevel::FAILURE);
}
*/

void Logging::Logger::log(const LogString& message, Logging::LoggerLevel level) {
  if (level >= loggerLevel) {
    std::cout << millis();
    switch (level) {
      case Logging::LoggerLevel::DEBUG: std::cout << ": (DEBUG)   "; break;
      case Logging::LoggerLevel::INFO: std::cout << ": (INFO)    "; break;
      case Logging::LoggerLevel::WARNING: std::cout << ": (WARNING) "; break;
      case Logging::LoggerLevel::FAILURE: std::cout << ": (ERROR)   "; break;
      default: break;
    }
    std::cout << message << std::endl;
  }
}

#else // Arduino
/*
void Logging::Logger::debug(const LogString& message) {
  log(message, Logging::LoggerLevel::DEBUG);
}

void Logging::Logger::info(const LogString& message) {
  log(message, Logging::LoggerLevel::INFO);
}

void Logging::Logger::warning(const LogString& message) {
  log(message, Logging::LoggerLevel::WARNING);
}

void Logging::Logger::error(const LogString& message) {
  log(message, Logging::LoggerLevel::FAILURE);
}
*/
void Logging::Logger::log(const LogString& message, Logging::LoggerLevel level) {
  if (level >= loggerLevel) {
    String messageBuffer = String(millis(), DEC);
    switch (level) {
      case Logging::LoggerLevel::DEBUG: 
      messageBuffer += ": (DEBUG)    "; break;
      case Logging::LoggerLevel::INFO: 
      messageBuffer += ": (INFO)     "; break;
      case Logging::LoggerLevel::WARNING:
       messageBuffer += ": (WARNING) "; break;
      case Logging::LoggerLevel::FAILURE: 
      messageBuffer += ": (FAILURE)  "; break;
      default: break;
    }
    messageBuffer += String(message.c_str());
    Serial.println(messageBuffer);
  }
}

#endif