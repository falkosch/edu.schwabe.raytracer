#pragma once

#include "vectorization/architecture.h"

namespace vectorization {
  // 32-bit lane shifts — per-lane
  PackedInts_128 shiftLeft32(const PackedInts_128 &a, const PackedInts_128 &b) noexcept;
  PackedInts_256 shiftLeft32(const PackedInts_256 &a, const PackedInts_256 &b) noexcept;

  PackedInts_128 shiftRightLogical32(const PackedInts_128 &a, const PackedInts_128 &b) noexcept;
  PackedInts_256 shiftRightLogical32(const PackedInts_256 &a, const PackedInts_256 &b) noexcept;

  PackedInts_128 shiftRightArithmetic32(const PackedInts_128 &a, const PackedInts_128 &b) noexcept;
  PackedInts_256 shiftRightArithmetic32(const PackedInts_256 &a, const PackedInts_256 &b) noexcept;

  // 32-bit lane shifts — scalar uniform
  PackedInts_128 shiftLeft32(const PackedInts_128 &a, Int_32 count) noexcept;
  PackedInts_128 shiftLeft32(const PackedInts_128 &a, UInt_32 count) noexcept;
  PackedInts_256 shiftLeft32(const PackedInts_256 &a, Int_32 count) noexcept;
  PackedInts_256 shiftLeft32(const PackedInts_256 &a, UInt_32 count) noexcept;

  PackedInts_128 shiftRightLogical32(const PackedInts_128 &a, Int_32 count) noexcept;
  PackedInts_128 shiftRightLogical32(const PackedInts_128 &a, UInt_32 count) noexcept;
  PackedInts_256 shiftRightLogical32(const PackedInts_256 &a, Int_32 count) noexcept;
  PackedInts_256 shiftRightLogical32(const PackedInts_256 &a, UInt_32 count) noexcept;

  PackedInts_128 shiftRightArithmetic32(const PackedInts_128 &a, Int_32 count) noexcept;
  PackedInts_128 shiftRightArithmetic32(const PackedInts_128 &a, UInt_32 count) noexcept;
  PackedInts_256 shiftRightArithmetic32(const PackedInts_256 &a, Int_32 count) noexcept;
  PackedInts_256 shiftRightArithmetic32(const PackedInts_256 &a, UInt_32 count) noexcept;

  // 64-bit lane shifts — per-lane
  PackedInts_128 shiftLeft64(const PackedInts_128 &a, const PackedInts_128 &b) noexcept;
  PackedInts_256 shiftLeft64(const PackedInts_256 &a, const PackedInts_256 &b) noexcept;

  PackedInts_128 shiftRightLogical64(const PackedInts_128 &a, const PackedInts_128 &b) noexcept;
  PackedInts_256 shiftRightLogical64(const PackedInts_256 &a, const PackedInts_256 &b) noexcept;

  PackedInts_128 shiftRightArithmetic64(const PackedInts_128 &a, const PackedInts_128 &b) noexcept;
  PackedInts_256 shiftRightArithmetic64(const PackedInts_256 &a, const PackedInts_256 &b) noexcept;

  // 64-bit lane shifts — scalar uniform
  PackedInts_128 shiftLeft64(const PackedInts_128 &a, Int_64 count) noexcept;
  PackedInts_128 shiftLeft64(const PackedInts_128 &a, UInt_64 count) noexcept;
  PackedInts_256 shiftLeft64(const PackedInts_256 &a, Int_64 count) noexcept;
  PackedInts_256 shiftLeft64(const PackedInts_256 &a, UInt_64 count) noexcept;

  PackedInts_128 shiftRightLogical64(const PackedInts_128 &a, Int_64 count) noexcept;
  PackedInts_128 shiftRightLogical64(const PackedInts_128 &a, UInt_64 count) noexcept;
  PackedInts_256 shiftRightLogical64(const PackedInts_256 &a, Int_64 count) noexcept;
  PackedInts_256 shiftRightLogical64(const PackedInts_256 &a, UInt_64 count) noexcept;

  PackedInts_128 shiftRightArithmetic64(const PackedInts_128 &a, Int_64 count) noexcept;
  PackedInts_128 shiftRightArithmetic64(const PackedInts_128 &a, UInt_64 count) noexcept;
  PackedInts_256 shiftRightArithmetic64(const PackedInts_256 &a, Int_64 count) noexcept;
  PackedInts_256 shiftRightArithmetic64(const PackedInts_256 &a, UInt_64 count) noexcept;
}
