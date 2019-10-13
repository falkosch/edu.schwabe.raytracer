#pragma once

#include "constants.h"
#include "functions.h"

namespace vectorization
{
    // packed SSE-vector of two uint64 scalars
    struct v_ui64_2
    {
        // Metas
        static const ASizeT SIZE = VectorSizes::Y;
        typedef UInt_64 ValueType;
        typedef ValueType BoolType;
        typedef v_ui64_2 VectorType;
        typedef VectorType VectorBoolType;
        typedef PackedTypes<ValueType, SIZE>::Type PackedType;

        // the actual data
        PackedType components;

        v_ui64_2() noexcept;

        v_ui64_2(const PackedType & v) noexcept;

        explicit v_ui64_2(const ValueType s) noexcept;

        explicit v_ui64_2(const ValueType x, const ValueType y) noexcept;

        v_ui64_2 & operator=(const PackedType & v) noexcept;

        ValueType & operator[](const int index) noexcept;

        const ValueType & operator[](const int index) const noexcept;

        ValueType & operator[](const ASizeT index) noexcept;

        const ValueType & operator[](const ASizeT index) const noexcept;
    }; // v_ui64_2

#pragma region Constant generators
    //{Constant generators

    template <>
    const v_ui64_2 MaskAll<v_ui64_2>() noexcept;

    template <>
    const v_ui64_2 MaskX<v_ui64_2>() noexcept;

    template <>
    const v_ui64_2 MaskXY<v_ui64_2>() noexcept;

    template <>
    const v_ui64_2 MaskY<v_ui64_2>() noexcept;

    template <>
    const v_ui64_2 Zero<v_ui64_2>() noexcept;

    template <>
    const v_ui64_2 One<v_ui64_2>() noexcept;

    template <>
    const v_ui64_2 Two<v_ui64_2>() noexcept;

    //}
#pragma endregion

#pragma region Commonly used swizzle accessors
    // Commonly used swizzle accessors

    const v_ui64_2 xx(const v_ui64_2 & v) noexcept;

    const v_ui64_2 xy(const v_ui64_2 & v) noexcept;

    const v_ui64_2 yx(const v_ui64_2 & v) noexcept;

    const v_ui64_2 yy(const v_ui64_2 & v) noexcept;

    const v_ui64_2 x_x(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    const v_ui64_2 x_y(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    const v_ui64_2 y_x(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    const v_ui64_2 y_y(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    //}
#pragma endregion

#pragma region Operators
    //{ Operators

    const v_ui64_2 operator-(const v_ui64_2 & v) noexcept;

    const v_ui64_2 operator!(const v_ui64_2 & v) noexcept;

    const v_ui64_2 operator~(const v_ui64_2 & v) noexcept;

    const v_ui64_2 operator+(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    const v_ui64_2 operator-(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    const v_ui64_2 operator*(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    const v_ui64_2 operator/(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    const v_ui64_2 operator%(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    const v_ui64_2 operator&(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    const v_ui64_2 operator|(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    const v_ui64_2 operator^(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    const v_ui64_2 operator<<(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    const v_ui64_2 operator>>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    const v_ui64_2 operator<(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    const v_ui64_2 operator>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    const v_ui64_2 operator<=(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    const v_ui64_2 operator>=(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    const v_ui64_2 operator==(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    const v_ui64_2 operator!=(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    v_ui64_2 & operator+=(v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    v_ui64_2 & operator-=(v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    v_ui64_2 & operator*=(v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    v_ui64_2 & operator/=(v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    v_ui64_2 & operator%=(v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    v_ui64_2 & operator&=(v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    v_ui64_2 & operator|=(v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    v_ui64_2 & operator^=(v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    v_ui64_2 & operator<<=(v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    v_ui64_2 & operator>>=(v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    //}
#pragma endregion

#pragma region swizzle()
    //{ swizzle()

    template <ASizeT X, ASizeT Y>
    // generic swizzle
    const v_ui64_2 swizzle(const v_ui64_2 & v) noexcept;

    template <>
    const v_ui64_2 swizzle<VectorIndices::X, VectorIndices::X>(const v_ui64_2 & v) noexcept;

    template <>
    const v_ui64_2 swizzle<VectorIndices::X, VectorIndices::Y>(const v_ui64_2 & v) noexcept;

    template <>
    const v_ui64_2 swizzle<VectorIndices::Y, VectorIndices::X>(const v_ui64_2 & v) noexcept;

    template <>
    const v_ui64_2 swizzle<VectorIndices::Y, VectorIndices::Y>(const v_ui64_2 & v) noexcept;

    //}
#pragma endregion

#pragma region blend()
    //{ blend()

    template <bool SelectX, bool SelectY>
    // generic blend of two vectors
    const v_ui64_2 blend(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    template <>
    const v_ui64_2 blend<false, false>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    template <>
    const v_ui64_2 blend<false, true>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    template <>
    const v_ui64_2 blend<true, false>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    template <>
    const v_ui64_2 blend<true, true>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    //}
#pragma endregion

#pragma region swizzledBlend()
    //{ swizzledBlend()

    template <ASizeT X, ASizeT Y, bool SelectX, bool SelectY>
    // generic swizzled blend
    inline const v_ui64_2 swizzledBlend(const v_ui64_2 & a, const v_ui64_2 & b) noexcept {
        return blend<SelectX, SelectY>(swizzle<X, Y>(a), swizzle<X, Y>(b));
    }

    template <>
    const v_ui64_2 swizzledBlend<VectorIndices::X, VectorIndices::X, false, true>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    template <>
    const v_ui64_2 swizzledBlend<VectorIndices::X, VectorIndices::X, true, false>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    template <>
    const v_ui64_2 swizzledBlend<VectorIndices::Y, VectorIndices::Y, false, true>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    template <>
    const v_ui64_2 swizzledBlend<VectorIndices::Y, VectorIndices::Y, true, false>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    template <ASizeT X, ASizeT Y>
    // Special case swizzled blend of two vectors, in which only the first
    // two components of this vector and the last two components of the
    // other vector are taken for blending after swizzling.
    const v_ui64_2 swizzledBlend_1x1(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    template <>
    const v_ui64_2 swizzledBlend_1x1<VectorIndices::X, VectorIndices::X>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    template <>
    const v_ui64_2 swizzledBlend_1x1<VectorIndices::X, VectorIndices::Y>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    template <>
    const v_ui64_2 swizzledBlend_1x1<VectorIndices::Y, VectorIndices::X>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    template <>
    const v_ui64_2 swizzledBlend_1x1<VectorIndices::Y, VectorIndices::Y>(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    //}
#pragma endregion

#pragma region blendMasked()
    //{ blendMasked()

    // varidiac blend
    const v_ui64_2 blendMasked(const v_ui64_2 & a, const v_ui64_2 & b, const v_ui64_2 & mask) noexcept;

    //}
#pragma endregion

#pragma region swizzledBlendMasked()
    //{ swizzledBlendMasked()

    template <ASizeT X, ASizeT Y>
    // generic swizzle with variadic mask for blend
    inline const v_ui64_2 swizzledBlendMasked(const v_ui64_2 & a, const v_ui64_2 & b, const v_ui64_2 & mask) noexcept {
        return blendMasked(swizzle<X, Y>(a), swizzle<X, Y>(b), mask);
    }

    template <>
    // passthrough
    const v_ui64_2 swizzledBlendMasked<VectorIndices::X, VectorIndices::Y>(const v_ui64_2 & a, const v_ui64_2 & b, const v_ui64_2 & mask) noexcept;

    //}
#pragma endregion

#pragma region setComponent()
    //{ setComponent()

    template <ASizeT Index>
    void setComponent(v_ui64_2 & v, const v_ui64_2::ValueType s) noexcept;

    template <>
    void setComponent<VectorIndices::X>(v_ui64_2 & v, const v_ui64_2::ValueType s) noexcept;

    template <>
    void setComponent<VectorIndices::Y>(v_ui64_2 & v, const v_ui64_2::ValueType s) noexcept;

    //}
#pragma endregion

#pragma region replaceComponent()
    //{ replaceComponent()

    template <ASizeT Index>
    const v_ui64_2 replaceComponent(const v_ui64_2 & v, const v_ui64_2::ValueType s) noexcept;

    template <>
    const v_ui64_2 replaceComponent<VectorIndices::X>(const v_ui64_2 & v, const v_ui64_2::ValueType s) noexcept;

    template <>
    const v_ui64_2 replaceComponent<VectorIndices::Y>(const v_ui64_2 & v, const v_ui64_2::ValueType s) noexcept;

    //}
#pragma endregion

#pragma region component()
    //{ component()

    template <ASizeT Index>
    const v_ui64_2::ValueType component(const v_ui64_2 & v) noexcept;

    template <>
    const v_ui64_2::ValueType component<VectorIndices::X>(const v_ui64_2 & v) noexcept;

    template <>
    const v_ui64_2::ValueType component<VectorIndices::Y>(const v_ui64_2 & v) noexcept;

    //}
#pragma endregion

#pragma region _()
    //{ _()

    const v_ui64_2::ValueType x(const v_ui64_2 & v) noexcept;

    const v_ui64_2::ValueType y(const v_ui64_2 & v) noexcept;

    //}
#pragma endregion

#pragma region replace_()
    //{ replace_()

    const v_ui64_2 replaceX(const v_ui64_2 & v, const v_ui64_2::ValueType s) noexcept;

    const v_ui64_2 replaceY(const v_ui64_2 & v, const v_ui64_2::ValueType s) noexcept;

    //}
#pragma endregion

#pragma region min()
    //{ min()

    const v_ui64_2 min(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    const v_ui64_2 minv(const v_ui64_2 & v) noexcept;

    const v_ui64_2::ValueType min(const v_ui64_2 & v) noexcept;

    //}
#pragma endregion

#pragma region max()
    //{ min()

    const v_ui64_2 max(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    const v_ui64_2 maxv(const v_ui64_2 & v) noexcept;

    const v_ui64_2::ValueType max(const v_ui64_2 & v) noexcept;

    //}
#pragma endregion

#pragma region Special vector operations
    //{ Special vector operations

    const v_ui64_2 lengthv(const v_ui64_2 & v) noexcept;

    const v_ui64_2::ValueType length(const v_ui64_2 & v) noexcept;

    const v_ui64_2::ValueType distance(const v_ui64_2 & a, const v_ui64_2 & b) noexcept;

    const v_ui64_2 clamp(const v_ui64_2 & v, const v_ui64_2 & l, const v_ui64_2 & m) noexcept;

    const bool anyTrue(const v_ui64_2 & v) noexcept;

    const bool anyFalse(const v_ui64_2 & v) noexcept;

    const bool allTrue(const v_ui64_2 & v) noexcept;

    const bool allFalse(const v_ui64_2 & v) noexcept;

    //}
#pragma endregion
}
