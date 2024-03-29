#include "vectorization/functions/modPad.h"

#include "vectorization/functions/modulo.h"

#include "vectorization/constants.h"

namespace vectorization {
  const Int_8 modPad(const Int_8 unpadded, const Int_8 modPadding) noexcept {
    const Int_32 overPadded = unpadded + modPadding - One<Int_32>();
    return static_cast<Int_8>(overPadded - modulo(overPadded, static_cast<Int_32>(modPadding)));
  }

  const UInt_8 modPad(const UInt_8 unpadded, const UInt_8 modPadding) noexcept {
    const Int_32 overPadded = unpadded + modPadding - One<Int_32>();
    return static_cast<UInt_8>(overPadded - modulo(overPadded, static_cast<Int_32>(modPadding)));
  }

  const Int_16 modPad(const Int_16 unpadded, const Int_16 modPadding) noexcept {
    const Int_32 overPadded = unpadded + modPadding - One<Int_32>();
    return static_cast<Int_16>(overPadded - modulo(overPadded, static_cast<Int_32>(modPadding)));
  }

  const UInt_16 modPad(const UInt_16 unpadded, const UInt_16 modPadding) noexcept {
    const Int_32 overPadded = unpadded + modPadding - One<Int_32>();
    return static_cast<UInt_16>(overPadded - modulo(overPadded, static_cast<Int_32>(modPadding)));
  }

  const Int_32 modPad(const Int_32 unpadded, const Int_32 modPadding) noexcept {
    const Int_32 overPadded = unpadded + modPadding - One<Int_32>();
    return overPadded - modulo(overPadded, modPadding);
  }

  const UInt_32 modPad(const UInt_32 unpadded, const UInt_32 modPadding) noexcept {
    const UInt_32 overPadded = unpadded + modPadding - One<UInt_32>();
    return overPadded - modulo(overPadded, modPadding);
  }

  const Int_64 modPad(const Int_64 unpadded, const Int_64 modPadding) noexcept {
    const Int_64 overPadded = unpadded + modPadding - One<Int_64>();
    return overPadded - modulo(overPadded, modPadding);
  }

  const UInt_64 modPad(const UInt_64 unpadded, const UInt_64 modPadding) noexcept {
    const UInt_64 overPadded = unpadded + modPadding - One<UInt_64>();
    return overPadded - modulo(overPadded, modPadding);
  }
}