#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  PackedFloat4_128 bitwiseNot(const PackedFloat4_128 &v) noexcept;
  PackedFloat2_128 bitwiseNot(const PackedFloat2_128 &v) noexcept;
  PackedFloat8_256 bitwiseNot(const PackedFloat8_256 &v) noexcept;
  PackedFloat4_256 bitwiseNot(const PackedFloat4_256 &v) noexcept;
  PackedInts_128 bitwiseNot(const PackedInts_128 &v) noexcept;
  PackedInts_256 bitwiseNot(const PackedInts_256 &v) noexcept;

  PackedFloat4_128 bitwiseAnd(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat2_128 bitwiseAnd(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;
  PackedFloat8_256 bitwiseAnd(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept;
  PackedFloat4_256 bitwiseAnd(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;
  PackedInts_128 bitwiseAnd(const PackedInts_128 &a, const PackedInts_128 &b) noexcept;
  PackedInts_256 bitwiseAnd(const PackedInts_256 &a, const PackedInts_256 &b) noexcept;

  PackedFloat4_128 bitwiseOr(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat2_128 bitwiseOr(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;
  PackedFloat8_256 bitwiseOr(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept;
  PackedFloat4_256 bitwiseOr(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;
  PackedInts_128 bitwiseOr(const PackedInts_128 &a, const PackedInts_128 &b) noexcept;
  PackedInts_256 bitwiseOr(const PackedInts_256 &a, const PackedInts_256 &b) noexcept;

  PackedFloat4_128 bitwiseXor(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat2_128 bitwiseXor(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;
  PackedFloat8_256 bitwiseXor(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept;
  PackedFloat4_256 bitwiseXor(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;
  PackedInts_128 bitwiseXor(const PackedInts_128 &a, const PackedInts_128 &b) noexcept;
  PackedInts_256 bitwiseXor(const PackedInts_256 &a, const PackedInts_256 &b) noexcept;

  PackedFloat4_128 bitwiseAndNot(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;
  PackedFloat2_128 bitwiseAndNot(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;
  PackedFloat8_256 bitwiseAndNot(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept;
  PackedFloat4_256 bitwiseAndNot(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;
  PackedInts_128 bitwiseAndNot(const PackedInts_128 &a, const PackedInts_128 &b) noexcept;
  PackedInts_256 bitwiseAndNot(const PackedInts_256 &a, const PackedInts_256 &b) noexcept;
}
