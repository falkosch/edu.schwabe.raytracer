#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  const bool min(const bool a, const bool b) noexcept;

  const Int_8 min(const Int_8 a, const Int_8 b) noexcept;

  const UInt_8 min(const UInt_8 a, const UInt_8 b) noexcept;

  const Int_16 min(const Int_16 a, const Int_16 b) noexcept;

  const UInt_16 min(const UInt_16 a, const UInt_16 b) noexcept;

  const Int_32 min(const Int_32 a, const Int_32 b) noexcept;

  const UInt_32 min(const UInt_32 a, const UInt_32 b) noexcept;

  const Int_64 min(const Int_64 a, const Int_64 b) noexcept;

  const UInt_64 min(const UInt_64 a, const UInt_64 b) noexcept;

  const Float_32 min(const Float_32 a, const Float_32 b) noexcept;

  const Float_64 min(const Float_64 a, const Float_64 b) noexcept;

  const PackedFloat4_128 min(const PackedFloat4_128 &a, const PackedFloat4_128 &b) noexcept;

  const PackedFloat2_128 min(const PackedFloat2_128 &a, const PackedFloat2_128 &b) noexcept;

  const PackedFloat8_256 min(const PackedFloat8_256 &a, const PackedFloat8_256 &b) noexcept;

  const PackedFloat4_256 min(const PackedFloat4_256 &a, const PackedFloat4_256 &b) noexcept;
}
