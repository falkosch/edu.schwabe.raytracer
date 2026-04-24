#pragma once

#include <cstdint>

namespace logging {

  enum class LogLevel : std::uint8_t {
    Error,
    Warn,
    Info,
    Debug
  };

}
