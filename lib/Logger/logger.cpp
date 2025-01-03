#include "logger.h"

Logger logger = Logger();
LoggerLevel loggerLevel = LoggerLevel::INFO;

#ifdef SIMULATOR
#include "simulator_tools.h"

void Logger::debug(const LogString message)
{
  Logger::log(message, LoggerLevel::DEBUG);
}

void Logger::info(const LogString message)
{
  Logger::log(message, LoggerLevel::INFO);
}

void Logger::warning(const LogString message)
{
  Logger::log(message, LoggerLevel::WARNING);
}

void Logger::error(const LogString message)
{
  Logger::log(message, LoggerLevel::ERROR);
}

void Logger::log(const LogString message, LoggerLevel level)
{

  if (level >= loggerLevel)
  {
    std::cout << millis();
    switch (level)
    {
    case LoggerLevel::DEBUG:
      std::cout << ": (DEBUG)   ";
      break;
    case LoggerLevel::INFO:
      std::cout << ": (INFO)    ";
      break;
    case LoggerLevel::WARNING:
      std::cout << ": (WARNING) ";
      break;
    case LoggerLevel::ERROR:
      std::cout << ": (ERROR)   ";
      break;
    default:
      break;
    }
    std::cout << message << std::endl;
  }
}

#else

void Logger::debug(const LogString message)
{
  Logger::log(String(millis(), DEC) + ": (DEBUG)   " + message, LoggerLevel::DEBUG);
}

void Logger::info(const LogString message)
{
  Logger::log(String(millis(), DEC) + ": (INFO)    " + message, LoggerLevel::INFO);
}

void Logger::warning(const LogString message)
{
  Logger::log(String(millis(), DEC) + ": (WARNING) " + message, LoggerLevel::WARNING);
}

void Logger::error(const LogString message)
{
  Logger::log(String(millis(), DEC) + ": (ERROR)   " + message, LoggerLevel::ERROR);
}

void Logger::log(const LogString message, LoggerLevel level)
{
  if (level >= loggerLevel)
  {
    Serial.println(message);
  }
}

#endif