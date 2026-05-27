#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  PackedFloat4_128 compareLess(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat2_128 compareLess(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;
  PackedFloat8_256 compareLess(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept;
  PackedFloat4_256 compareLess(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;

  PackedFloat4_128 compareGreater(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat2_128 compareGreater(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;
  PackedFloat8_256 compareGreater(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept;
  PackedFloat4_256 compareGreater(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;

  PackedFloat4_128 compareLessEqual(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat2_128 compareLessEqual(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;
  PackedFloat8_256 compareLessEqual(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept;
  PackedFloat4_256 compareLessEqual(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;

  PackedFloat4_128 compareGreaterEqual(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat2_128 compareGreaterEqual(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;
  PackedFloat8_256 compareGreaterEqual(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept;
  PackedFloat4_256 compareGreaterEqual(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;

  PackedFloat4_128 compareEqual(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat2_128 compareEqual(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;
  PackedFloat8_256 compareEqual(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept;
  PackedFloat4_256 compareEqual(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;

  PackedFloat4_128 compareNotEqual(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat2_128 compareNotEqual(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;
  PackedFloat8_256 compareNotEqual(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept;
  PackedFloat4_256 compareNotEqual(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;
}
