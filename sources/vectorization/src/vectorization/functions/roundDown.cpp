#include "vectorization/functions/roundDown.h"

#include "vectorization/constants.h"

namespace vectorization
{
    const Int_8 roundDown(const Int_8 value, const Int_8 stepSize) noexcept {
        return static_cast<Int_8>(value & ~(stepSize - One<Int_8>()));
    }

    const UInt_8 roundDown(const UInt_8 value, const UInt_8 stepSize) noexcept {
        return static_cast<UInt_8>(value & ~(stepSize - One<UInt_8>()));
    }

    const Int_16 roundDown(const Int_16 value, const Int_16 stepSize) noexcept {
        return static_cast<Int_16>(value & ~(stepSize - One<Int_16>()));
    }

    const UInt_16 roundDown(const UInt_16 value, const UInt_16 stepSize) noexcept {
        return static_cast<UInt_16>(value & ~(stepSize - One<UInt_16>()));
    }

    const Int_32 roundDown(const Int_32 value, const Int_32 stepSize) noexcept {
        return value & ~(stepSize - One<Int_32>());
    }

    const UInt_32 roundDown(const UInt_32 value, const UInt_32 stepSize) noexcept {
        return value & ~(stepSize - One<UInt_32>());
    }

    const Int_64 roundDown(const Int_64 value, const Int_64 stepSize) noexcept {
        return value & ~(stepSize - One<Int_64>());
    }

    const UInt_64 roundDown(const UInt_64 value, const UInt_64 stepSize) noexcept {
        return value & ~(stepSize - One<UInt_64>());
    }
}