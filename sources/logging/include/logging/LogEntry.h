#pragma once

#include "LogLevel.h"

#include <chrono>
#include <functional>
#include <string>

namespace logging {

  struct LogEntry {
    LogLevel level;
    std::string scope;
    std::function<std::string()> messageFactory;
    std::chrono::steady_clock::time_point timestamp;
  };

}
