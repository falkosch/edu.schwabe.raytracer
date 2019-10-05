#pragma once

#include "../architecture.h"

namespace vectorization
{
    template <typename U>
    const U convert(const bool) noexcept;

    template <typename U>
    const U convert(const bool, const bool) noexcept;

    template <typename U>
    const U convert(const Int_32 v) noexcept;

    template <>
    const Int_8 convert<Int_8>(const Int_32 v) noexcept;

    template <>
    const UInt_8 convert<UInt_8>(const Int_32 v) noexcept;

    template <>
    const Int_16 convert<Int_16>(const Int_32 v) noexcept;

    template <>
    const UInt_16 convert<UInt_16>(const Int_32 v) noexcept;

    template <>
    const Int_32 convert<Int_32>(const Int_32 v) noexcept;

    template <>
    const UInt_32 convert<UInt_32>(const Int_32 v) noexcept;

    template <>
    const Int_64 convert<Int_64>(const Int_32 v) noexcept;

    template <>
    const UInt_64 convert<UInt_64>(const Int_32 v) noexcept;

    template <>
    const Float_32 convert<Float_32>(const Int_32 v) noexcept;

    template <>
    const Float_64 convert<Float_64>(const Int_32 v) noexcept;

    template <typename U>
    const U convert(const UInt_32 v) noexcept;

    template <>
    const Int_32 convert<Int_32>(const UInt_32 v) noexcept;

    template <>
    const Int_64 convert<Int_64>(const UInt_32 v) noexcept;

    template <>
    const UInt_64 convert<UInt_64>(const UInt_32 v) noexcept;

    template <>
    const Float_32 convert<Float_32>(const UInt_32 v) noexcept;

    template <>
    const Float_64 convert<Float_64>(const UInt_32 v) noexcept;

    template <typename U>
    const U convert(const Int_64 v) noexcept;

    template <>
    const Int_32 convert<Int_32>(const Int_64 v) noexcept;

    template <>
    const UInt_32 convert<UInt_32>(const Int_64 v) noexcept;

    template <>
    const UInt_64 convert<UInt_64>(const Int_64 v) noexcept;

    template <>
    const Float_32 convert<Float_32>(const Int_64 v) noexcept;

    template <>
    const Float_64 convert<Float_64>(const Int_64 v) noexcept;

    template <typename U>
    const U convert(const UInt_64 v) noexcept;

    template <>
    const Int_32 convert<Int_32>(const UInt_64 v) noexcept;

    template <>
    const UInt_32 convert<UInt_32>(const UInt_64 v) noexcept;

    template <>
    const Int_64 convert<Int_64>(const UInt_64 v) noexcept;

    template <>
    const Float_32 convert<Float_32>(const UInt_64 v) noexcept;

    template <>
    const Float_64 convert<Float_64>(const UInt_64 v) noexcept;

    template <typename U>
    const U convert(const Float_32 v) noexcept;

    template <>
    const Int_32 convert<Int_32>(const Float_32 v) noexcept;

    template <>
    const UInt_32 convert<UInt_32>(const Float_32 v) noexcept;

    template <>
    const Int_64 convert<Int_64>(const Float_32 v) noexcept;

    template <>
    const UInt_64 convert<UInt_64>(const Float_32 v) noexcept;

    template <>
    const Float_64 convert<Float_64>(const Float_32 v) noexcept;

    template <typename U>
    const U convert(const Float_64 v) noexcept;

    template <>
    const Int_32 convert<Int_32>(const Float_64 v) noexcept;

    template <>
    const UInt_32 convert<UInt_32>(const Float_64 v) noexcept;

    template <>
    const Int_64 convert<Int_64>(const Float_64 v) noexcept;

    template <>
    const UInt_64 convert<UInt_64>(const Float_64 v) noexcept;

    template <>
    const Float_32 convert<Float_32>(const Float_64 v) noexcept;
}
