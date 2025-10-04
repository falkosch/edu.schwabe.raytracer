#include "vectorization/blends/blendMasked_scalar.h"

#include "vectorization/blends/blendMasked_128d.h"
#include "vectorization/blends/blendMasked_128s.h"

#include "vectorization/accessors.h"

#ifndef ARCH_X64
#include <array>
#endif

namespace vectorization {
  const bool blendMasked(const bool onBitNotSet, const bool onBitSet, const bool mask) noexcept {
    return mask ? onBitSet : onBitNotSet;
  }

  const Int_8 blendMasked(const Int_8 onBitNotSet, const Int_8 onBitSet, const Int_8 mask) noexcept {
    return static_cast<Int_8>((mask & onBitSet) | (~mask & onBitNotSet));
  }

  const UInt_8 blendMasked(const UInt_8 onBitNotSet, const UInt_8 onBitSet, const UInt_8 mask) noexcept {
    return static_cast<UInt_8>((mask & onBitSet) | (~mask & onBitNotSet));
  }

  const Int_16 blendMasked(const Int_16 onBitNotSet, const Int_16 onBitSet, const Int_16 mask) noexcept {
    return static_cast<Int_16>((mask & onBitSet) | (~mask & onBitNotSet));
  }

  const UInt_16 blendMasked(const UInt_16 onBitNotSet, const UInt_16 onBitSet, const UInt_16 mask) noexcept {
    return static_cast<UInt_16>((mask & onBitSet) | (~mask & onBitNotSet));
  }

  const Int_32 blendMasked(const Int_32 onBitNotSet, const Int_32 onBitSet, const Int_32 mask) noexcept {
    return (mask & onBitSet) | (~mask & onBitNotSet);
  }

  const UInt_32 blendMasked(const UInt_32 onBitNotSet, const UInt_32 onBitSet, const UInt_32 mask) noexcept {
    return (mask & onBitSet) | (~mask & onBitNotSet);
  }

  const Int_64 blendMasked(const Int_64 onBitNotSet, const Int_64 onBitSet, const Int_64 mask) noexcept {
    return (mask & onBitSet) | (~mask & onBitNotSet);
  }

  const UInt_64 blendMasked(const UInt_64 onBitNotSet, const UInt_64 onBitSet, const UInt_64 mask) noexcept {
    return (mask & onBitSet) | (~mask & onBitNotSet);
  }

  const Float_32
  blendMasked(const Float_32 onBitNotSet, const Float_32 onBitSet, const BoolTypes<Float_32>::Type mask) noexcept {
    return x(blendMasked(_mm_set_ss(onBitNotSet), _mm_set_ss(onBitSet), _mm_set1_epi32(static_cast<int>(mask))));
  }

  const Float_64
  blendMasked(const Float_64 onBitNotSet, const Float_64 onBitSet, const BoolTypes<Float_64>::Type mask) noexcept {
    PackedInts_128 m;
#ifdef ARCH_X64
    m = _mm_set1_epi64x(mask);
#else
    const std::array<BoolTypes<Float_64>::Type, 2> maskComponents{mask, mask};
    std::memcpy(&m, maskComponents.data(), sizeof(PackedInts_128));
#endif
    return x(blendMasked(_mm_set_sd(onBitNotSet), _mm_set_sd(onBitSet), m));
  }
}