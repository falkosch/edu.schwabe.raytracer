#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  // a * b with alternating sub/add of c: even-indexed = add, odd-indexed = sub
  // 128s: {a[0]*b[0]+c[0], a[1]*b[1]-c[1], a[2]*b[2]+c[2], a[3]*b[3]-c[3]}
  // 128d: {a[0]*b[0]+c[0], a[1]*b[1]-c[1]}
  PackedFloat4_128 multiplySubAdd(const PackedFloat4_128 &a, const PackedFloat4_128 &b, const PackedFloat4_128 &c) noexcept;
  PackedFloat2_128 multiplySubAdd(const PackedFloat2_128 &a, const PackedFloat2_128 &b, const PackedFloat2_128 &c) noexcept;

  PackedFloat8_256 multiplySubAdd(const PackedFloat8_256 &a, const PackedFloat8_256 &b, const PackedFloat8_256 &c) noexcept;
  PackedFloat4_256 multiplySubAdd(const PackedFloat4_256 &a, const PackedFloat4_256 &b, const PackedFloat4_256 &c) noexcept;
}
