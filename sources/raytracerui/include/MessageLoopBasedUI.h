#pragma once

#include <windows.h>

namespace raytracerui {
  class MessageLoopBasedUI {
  public:
    virtual ~MessageLoopBasedUI();

    virtual WPARAM run() = 0;
  };
}
