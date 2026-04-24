#pragma once

#include "LogLevel.h"

#include <cstddef>
#include <mutex>
#include <string>
#include <vector>

namespace logging {

  struct DisplayLine {
    LogLevel level;
    std::string text;
  };

  class DisplayBuffer {
  public:
    static constexpr std::size_t MAX_LINES = 10000;

    void push(DisplayLine line);

    std::vector<DisplayLine> snapshot() const;

    void clear();

  private:
    mutable std::mutex mutex_;
    std::vector<DisplayLine> lines_;
  };

}
