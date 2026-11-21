#pragma once

#include "DisplayLine.h"

#include <cstddef>
#include <mutex>
#include <vector>

namespace logging {

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
