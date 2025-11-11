#pragma once
#ifdef SIMULATOR
#include <iostream>
#include <string>
#include <sstream>

using LogString = std::string;
#else
#include <Arduino.h>
using LogString = String;
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
    // void debug(const LogString& message);
    // void info(const LogString& message);
    // void warning(const LogString& message);
    // void error(const LogString& message);

    template<typename... Args>
    void debug(const LogString& tag, Args&&... args) {
      log(stringConcat(LogString(), std::forward<Args>(args)...), LoggerLevel::DEBUG, tag);
    }

    template<typename... Args>
    void info(const LogString& tag, Args&&... args) {
      log(stringConcat(LogString(), std::forward<Args>(args)...), LoggerLevel::INFO, tag);
    }

    template<typename... Args>
    void warning(const LogString& tag, Args&&... args) {
      log(stringConcat(LogString(), std::forward<Args>(args)...), LoggerLevel::WARNING, tag);
    }

    template<typename... Args>
    void error(const LogString& tag, Args&&... args) {
      log(stringConcat(LogString(), std::forward<Args>(args)...), LoggerLevel::FAILURE, tag);
    }


  private:
    void log(const LogString& message, Logging::LoggerLevel level, const LogString& tag = "");

    #ifdef SIMULATOR
    template<typename T, typename... Args>
    LogString stringConcat(LogString blank, T&& next, Args&&... rest) {
      std::ostringstream str;
      return stringConcatRecursive(str, std::forward<T>(next), std::forward<Args>(rest)...);
    }

    LogString stringConcat(LogString str) {
      return str;
    }

    template<typename T, typename... Args>
    LogString stringConcatRecursive(std::ostringstream& str, T&& next, Args&&... rest) {
      str << std::forward<T>(next);
      return stringConcatRecursive(str, std::forward<Args>(rest)...);
    }

    LogString stringConcatRecursive(std::ostringstream& str) {
      return str.str();
    }

    #else
    template<typename T, typename... Args>
    LogString stringConcat(String str, T next, Args... rest)
    {
      return stringConcat(str + String(next), rest...);
    }
    LogString stringConcat(String str) {
      return str;
    }
    #endif
  };
}

// Fully qualify the types here
extern Logging::Logger logger;
extern Logging::LoggerLevel loggerLevel;