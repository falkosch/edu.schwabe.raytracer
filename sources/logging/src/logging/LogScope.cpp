#include "logging/LogScope.h"
#include "logging/Logger.h"

#include <chrono>

namespace logging {

  LogScope::LogScope(std::string scopeName)
      : scopeName_(std::move(scopeName)) {
  }

  void LogScope::enqueue(const LogLevel level, std::function<std::string()> messageFactory) const {
    Logger::instance().enqueue(LogEntry{
        level,
        scopeName_,
        std::move(messageFactory),
        std::chrono::steady_clock::now()
    });
  }

  void LogScope::error(std::function<std::string()> messageFactory) const {
    enqueue(LogLevel::Error, std::move(messageFactory));
  }

  void LogScope::warn(std::function<std::string()> messageFactory) const {
    enqueue(LogLevel::Warn, std::move(messageFactory));
  }

  void LogScope::info(std::function<std::string()> messageFactory) const {
    enqueue(LogLevel::Info, std::move(messageFactory));
  }

  void LogScope::debug(std::function<std::string()> messageFactory) const {
    enqueue(LogLevel::Debug, std::move(messageFactory));
  }

  LogScope scope(const std::string &name) {
    return LogScope(name);
  }

}
