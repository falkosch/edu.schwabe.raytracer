#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  template <typename T>
  T Half() noexcept;

  template <>
  Float_32 Half<Float_32>() noexcept;

  template <>
  Float_64 Half<Float_64>() noexcept;

  template <>
  PackedFloat4_128 Half<PackedFloat4_128>() noexcept;

  template <>
  PackedFloat2_128 Half<PackedFloat2_128>() noexcept;

  template <>
  PackedFloat8_256 Half<PackedFloat8_256>() noexcept;

  template <>
  PackedFloat4_256 Half<PackedFloat4_256>() noexcept;
}
