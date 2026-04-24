#pragma once

#include "LogLevel.h"

#include <functional>
#include <string>

namespace logging {

  class LogScope {
  public:
    explicit LogScope(std::string scopeName);

    void error(std::function<std::string()> messageFactory) const;
    void warn(std::function<std::string()> messageFactory) const;
    void info(std::function<std::string()> messageFactory) const;
    void debug(std::function<std::string()> messageFactory) const;

  private:
    std::string scopeName_;

    void enqueue(LogLevel level, std::function<std::string()> messageFactory) const;
  };

  LogScope scope(const std::string &name);

}
