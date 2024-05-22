#include "vectorization/functions/modPad.h"

#include "vectorization/functions/modulo.h"

#include "vectorization/constants.h"

namespace vectorization {
  Int_8 modPad(const Int_8 value, const Int_8 padding) noexcept {
    const auto overPadded = value + padding - One<Int_32>();
    return static_cast<Int_8>(overPadded - modulo(overPadded, static_cast<Int_32>(padding)));
  }

  UInt_8 modPad(const UInt_8 value, const UInt_8 padding) noexcept {
    const auto overPadded = value + padding - One<Int_32>();
    return static_cast<UInt_8>(overPadded - modulo(overPadded, static_cast<Int_32>(padding)));
  }

  Int_16 modPad(const Int_16 value, const Int_16 padding) noexcept {
    auto overPadded = value + padding - One<Int_32>();
    return static_cast<Int_16>(overPadded - modulo(overPadded, static_cast<Int_32>(padding)));
  }

  UInt_16 modPad(const UInt_16 value, const UInt_16 padding) noexcept {
    const auto overPadded = value + padding - One<Int_32>();
    return static_cast<UInt_16>(overPadded - modulo(overPadded, static_cast<Int_32>(padding)));
  }

  Int_32 modPad(const Int_32 value, const Int_32 padding) noexcept {
    const auto overPadded = value + padding - One<Int_32>();
    return overPadded - modulo(overPadded, padding);
  }

  UInt_32 modPad(const UInt_32 value, const UInt_32 padding) noexcept {
    auto overPadded = value + padding - One<UInt_32>();
    return overPadded - modulo(overPadded, padding);
  }

  Int_64 modPad(const Int_64 value, const Int_64 padding) noexcept {
    const auto overPadded = value + padding - One<Int_64>();
    return overPadded - modulo(overPadded, padding);
  }

  UInt_64 modPad(const UInt_64 value, const UInt_64 padding) noexcept {
    const auto overPadded = value + padding - One<UInt_64>();
    return overPadded - modulo(overPadded, padding);
  }
}
