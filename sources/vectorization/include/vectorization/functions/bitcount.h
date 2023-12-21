#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  const ASizeT bitCount(const bool v) noexcept;

  const ASizeT bitCount(const Int_8 v) noexcept;

  const ASizeT bitCount(const UInt_8 v) noexcept;

  const ASizeT bitCount(const Int_16 v) noexcept;

  const ASizeT bitCount(const UInt_16 v) noexcept;

  const ASizeT bitCount(const Int_32 v) noexcept;

  const ASizeT bitCount(const UInt_32 v) noexcept;

  const ASizeT bitCount(const Int_64 v) noexcept;

  const ASizeT bitCount(const UInt_64 v) noexcept;

  const ASizeT bitCount(const Float_32 v) noexcept;

  const ASizeT bitCount(const Float_64 c) noexcept;

  const ASizeT bitCount(const PackedInts_128 &v) noexcept;

  const ASizeT bitCount(const PackedFloat2_128 &v) noexcept;

  const ASizeT bitCount(const PackedFloat4_128 &v) noexcept;

  const ASizeT bitCount(const PackedInts_256 &v) noexcept;

  const ASizeT bitCount(const PackedFloat8_256 &v) noexcept;

  const ASizeT bitCount(const PackedFloat4_256 &v) noexcept;
}
