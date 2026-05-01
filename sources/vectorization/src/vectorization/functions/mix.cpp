#include "vectorization/functions/mix.h"

#include "vectorization/functions/broadcast.h"
#include "vectorization/functions/multiply_add.h"
#include "vectorization/functions/subtract.h"

namespace vectorization {
  Float_32 mix(const Float_32 x, const Float_32 y, const Float_32 factor) noexcept {
    return x + (y - x) * factor;
  }

  Float_64 mix(const Float_64 x, const Float_64 y, const Float_64 factor) noexcept {
    return x + (y - x) * factor;
  }

  PackedFloat4_128 mix(const PackedFloat4_128 &x, const PackedFloat4_128 &y, const Float_32 factor) noexcept {
    return mix(x, y, broadcast(factor));
  }

  PackedFloat2_128 mix(const PackedFloat2_128 &x, const PackedFloat2_128 &y, const Float_64 factor) noexcept {
    return mix(x, y, broadcast(factor));
  }

  PackedFloat4_128 mix(const PackedFloat4_128 &x, const PackedFloat4_128 &y, const PackedFloat4_128 &factor) noexcept {
    return multiplyAdd(subtract(y, x), factor, x);
  }

  PackedFloat2_128 mix(const PackedFloat2_128 &x, const PackedFloat2_128 &y, const PackedFloat2_128 &factor) noexcept {
    return multiplyAdd(subtract(y, x), factor, x);
  }

}
