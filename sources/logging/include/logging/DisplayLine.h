#pragma once

#include "LogLevel.h"

#include <string>

namespace logging {

  struct DisplayLine {
    LogLevel level;
    std::string text;
  };

}
