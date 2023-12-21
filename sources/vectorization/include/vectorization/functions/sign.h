#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  const Int_8 sign(const Int_8 v) noexcept;

  const Int_16 sign(const Int_16 v) noexcept;

  const Int_32 sign(const Int_32 v) noexcept;

  const Int_64 sign(const Int_64 v) noexcept;

  const Float_32 sign(const Float_32 v) noexcept;

  const Float_64 sign(const Float_64 v) noexcept;

  const PackedFloat4_128 sign(const PackedFloat4_128 &v) noexcept;

  const PackedFloat2_128 sign(const PackedFloat2_128 &v) noexcept;

  const PackedFloat8_256 sign(const PackedFloat8_256 &v) noexcept;

  const PackedFloat4_256 sign(const PackedFloat4_256 &v) noexcept;
}
