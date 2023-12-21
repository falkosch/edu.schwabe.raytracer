#include "raytracing/common/Tools.h"
#include "../../stdafx.h"

//  override byte to prevent clashes with <cstddef>
#define byte win_byte_override

#include <windows.h>

#undef byte

namespace raytracer {
  const UInt_64 Tools::seed() {
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return static_cast<UInt_64>(counter.HighPart ^ counter.LowPart);
  }
}