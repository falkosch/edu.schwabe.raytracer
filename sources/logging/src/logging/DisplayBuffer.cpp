#include "logging/DisplayBuffer.h"

namespace logging {

  void DisplayBuffer::push(DisplayLine line) {
    std::lock_guard lock(mutex_);
    if (lines_.size() >= MAX_LINES) {
      lines_.erase(lines_.begin(), lines_.begin() + static_cast<std::ptrdiff_t>(MAX_LINES / 4));
    }
    lines_.push_back(std::move(line));
  }

  std::vector<DisplayLine> DisplayBuffer::snapshot() const {
    std::lock_guard lock(mutex_);
    return lines_;
  }

  void DisplayBuffer::clear() {
    std::lock_guard lock(mutex_);
    lines_.clear();
  }

}
