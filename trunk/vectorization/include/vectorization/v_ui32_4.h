#pragma once

#include "constants.h"
#include "functions.h"

namespace vectorization
{
    // packed SSE-vector of four uint32 scalars
    struct v_ui32_4
    {
        // Metas
        static const ASizeT SIZE = VectorSizes::W;
        typedef UInt_32 ValueType;
        typedef ValueType BoolType;
        typedef v_ui32_4 VectorType;
        typedef VectorType VectorBoolType;
        typedef PackedTypes<ValueType, SIZE>::Type PackedType;

        // the actual data
        PackedType components;

        v_ui32_4() noexcept;

        v_ui32_4(const PackedType & v) noexcept;

        explicit v_ui32_4(const ValueType s) noexcept;

        explicit v_ui32_4(const ValueType x, const ValueType y) noexcept;

        explicit v_ui32_4(const ValueType x, const ValueType y, const ValueType z) noexcept;

        explicit v_ui32_4(const ValueType x, const ValueType y, const ValueType z, const ValueType w) noexcept;

        v_ui32_4 & operator=(const PackedType & v) noexcept;

        ValueType & operator[](const int index) noexcept;

        const ValueType & operator[](const int index) const noexcept;

        ValueType & operator[](const ASizeT index) noexcept;

        const ValueType & operator[](const ASizeT index) const noexcept;
    }; // v_ui32_4

#pragma region Constant generators
    //{Constant generators

    template <>
    const v_ui32_4 MaskAll<v_ui32_4>() noexcept;

    template <>
    const v_ui32_4 MaskX<v_ui32_4>() noexcept;

    template <>
    const v_ui32_4 MaskXY<v_ui32_4>() noexcept;

    template <>
    const v_ui32_4 MaskXYZ<v_ui32_4>() noexcept;

    template <>
    const v_ui32_4 MaskXYZW<v_ui32_4>() noexcept;

    template <>
    const v_ui32_4 MaskY<v_ui32_4>() noexcept;

    template <>
    const v_ui32_4 MaskYZ<v_ui32_4>() noexcept;

    template <>
    const v_ui32_4 MaskYZW<v_ui32_4>() noexcept;

    template <>
    const v_ui32_4 MaskZ<v_ui32_4>() noexcept;

    template <>
    const v_ui32_4 MaskZW<v_ui32_4>() noexcept;

    template <>
    const v_ui32_4 MaskW<v_ui32_4>() noexcept;

    template <>
    const v_ui32_4 OneW<v_ui32_4>() noexcept;

    template <>
    const v_ui32_4 Zero<v_ui32_4>() noexcept;

    template <>
    const v_ui32_4 One<v_ui32_4>() noexcept;

    template <>
    const v_ui32_4 Two<v_ui32_4>() noexcept;

    //}
#pragma endregion

#pragma region Commonly used swizzle accessors
    // Commonly used swizzle accessors

    const v_ui32_4 xxxx(const v_ui32_4 & v) noexcept;

    const v_ui32_4 yyyy(const v_ui32_4 & v) noexcept;

    const v_ui32_4 zzzz(const v_ui32_4 & v) noexcept;

    const v_ui32_4 wwww(const v_ui32_4 & v) noexcept;

    const v_ui32_4 xxyy(const v_ui32_4 & v) noexcept;

    const v_ui32_4 xyxy(const v_ui32_4 & v) noexcept;

    const v_ui32_4 yxwz(const v_ui32_4 & v) noexcept;

    const v_ui32_4 zzww(const v_ui32_4 & v) noexcept;

    const v_ui32_4 zwzw(const v_ui32_4 & v) noexcept;

    const v_ui32_4 xy_xy(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    const v_ui32_4 zw_zw(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    //}
#pragma endregion

#pragma region Operators
    //{Operators

    const v_ui32_4 operator!(const v_ui32_4 & v) noexcept;

    const v_ui32_4 operator~(const v_ui32_4 & v) noexcept;

    const v_ui32_4 operator+(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    const v_ui32_4 operator-(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    const v_ui32_4 operator*(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    const v_ui32_4 operator/(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    const v_ui32_4 operator%(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    const v_ui32_4 operator&(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    const v_ui32_4 operator|(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    const v_ui32_4 operator^(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    const v_ui32_4 operator<<(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    const v_ui32_4 operator>>(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    const v_ui32_4 operator<(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    const v_ui32_4 operator>(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    const v_ui32_4 operator<=(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    const v_ui32_4 operator>=(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    const v_ui32_4 operator==(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    const v_ui32_4 operator!=(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    v_ui32_4 & operator+=(v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    v_ui32_4 & operator-=(v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    v_ui32_4 & operator*=(v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    v_ui32_4 & operator/=(v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    v_ui32_4 & operator%=(v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    v_ui32_4 & operator&=(v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    v_ui32_4 & operator|=(v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    v_ui32_4 & operator^=(v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    v_ui32_4 & operator<<=(v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    v_ui32_4 & operator>>=(v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    //}
#pragma endregion

#pragma region swizzle()
    //{ swizzle()

    template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
    // generic swizzle
    inline const v_ui32_4 swizzle(const v_ui32_4 & v) noexcept {
        static_assert(X < v_ui32_4::SIZE, "Index is out of range");
        static_assert(Y < v_ui32_4::SIZE, "Index is out of range");
        static_assert(Z < v_ui32_4::SIZE, "Index is out of range");
        static_assert(W < v_ui32_4::SIZE, "Index is out of range");
        return _mm_shuffle_epi32(v.components, _MM_SHUFFLE(W, Z, Y, X));
    }

    template <>
    // passthrough swizzle
    const v_ui32_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_ui32_4 & v) noexcept;

    template <>
    // special unpack low-values swizzle
    const v_ui32_4 swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(const v_ui32_4 & v) noexcept;

    template <>
    // special unpack high-values swizzle
    const v_ui32_4 swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(const v_ui32_4 & v) noexcept;

    template <>
    // special unpack XY-values swizzle
    const v_ui32_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const v_ui32_4 & v) noexcept;

    template <>
    // special unpack ZW-values swizzle
    const v_ui32_4 swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const v_ui32_4 & v) noexcept;

    //}
#pragma endregion

#pragma region blend()
    //{ blend()

    template <bool SelectX, bool SelectY, bool SelectZ, bool SelectW>
    // generic blend of two vectors
    inline const v_ui32_4 blend(const v_ui32_4 & a, const v_ui32_4 & b) noexcept {
        return _mm_blend_epi16(
            a.components,
            b.components,
            BLEND_MASK_INT4_32(SelectX, SelectY, SelectZ, SelectW)
        );
    }

    template <>
    // passtrough blend
    const v_ui32_4 blend<false, false, false, false>(const v_ui32_4 & a, const v_ui32_4 &) noexcept;

    template <>
    // passthrough blend other
    const v_ui32_4 blend<true, true, true, true>(const v_ui32_4 &, const v_ui32_4 & b) noexcept;

    //}
#pragma endregion

#pragma region swizzledBlend()
    //{ swizzledBlend()

    template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W, bool SelectX, bool SelectY, bool SelectZ, bool SelectW>
    // generic swizzled blend
    inline const v_ui32_4 swizzledBlend(const v_ui32_4 & a, const v_ui32_4 & b) noexcept {
        return blend<SelectX, SelectY, SelectZ, SelectW>(swizzle<X, Y, Z, W>(a), swizzle<X, Y, Z, W>(b));
    }

    template <>
    const v_ui32_4 swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, false, true, false, true>(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    template <>
    const v_ui32_4 swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, true, false, true, false>(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    template <>
    const v_ui32_4 swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, false, true, false, true>(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    template <>
    const v_ui32_4 swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, true, false, true, false>(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    template <>
    const v_ui32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, false, false, true, true>(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    template <>
    const v_ui32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, true, true, false, false>(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    template <>
    const v_ui32_4 swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, false, false, true, true>(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    template <>
    const v_ui32_4 swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, true, true, false, false>(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
    // Special case swizzled blend of two vectors, in which only the first
    // two components of this vector and the last two components of the
    // other vector are taken for blending after swizzling.
    inline const v_ui32_4 swizzledBlend2x2(const v_ui32_4 & a, const v_ui32_4 & b) noexcept {
        return swizzledBlend2x2<X, Y, Z, W, false, false, true, true>(a, b);
    }

    template <>
    const v_ui32_4 swizzledBlend2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    template <>
    const v_ui32_4 swizzledBlend2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    template <>
    const v_ui32_4 swizzledBlend2x2<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    //}
#pragma endregion

#pragma region blendMasked()
    //{ blendMasked()

    const v_ui32_4 blendMasked(const v_ui32_4 & a, const v_ui32_4 & b, const v_ui32_4 & mask) noexcept;

    //}
#pragma endregion

#pragma region swizzledBlendMasked()
    //{ swizzledBlendMasked()

    template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
    inline const v_ui32_4 swizzledBlendMasked(const v_ui32_4 & a, const v_ui32_4 & b, const v_ui32_4 & mask) noexcept {
        return blendMasked(swizzle<X, Y, Z, W>(a), swizzle<X, Y, Z, W>(b), mask);
    }

    //}
#pragma endregion

#pragma region replaceComponent()
    //{ replaceComponent()

    template <ASizeT Index>
    const v_ui32_4 replaceComponent(const v_ui32_4 & v, const v_ui32_4::ValueType s) noexcept;

    template <>
    const v_ui32_4 replaceComponent<VectorIndices::X>(const v_ui32_4 & v, const v_ui32_4::ValueType s) noexcept;

    template <>
    const v_ui32_4 replaceComponent<VectorIndices::Y>(const v_ui32_4 & v, const v_ui32_4::ValueType s) noexcept;

    template <>
    const v_ui32_4 replaceComponent<VectorIndices::Z>(const v_ui32_4 & v, const v_ui32_4::ValueType s) noexcept;

    template <>
    const v_ui32_4 replaceComponent<VectorIndices::W>(const v_ui32_4 & v, const v_ui32_4::ValueType s) noexcept;

    //}
#pragma endregion

#pragma region component()
    //{ component()

    template <ASizeT Index>
    const v_ui32_4::ValueType component(const v_ui32_4 & v) noexcept;

    template <>
    const v_ui32_4::ValueType component<VectorIndices::X>(const v_ui32_4 & v) noexcept;

    template <>
    const v_ui32_4::ValueType component<VectorIndices::Y>(const v_ui32_4 & v) noexcept;

    template <>
    const v_ui32_4::ValueType component<VectorIndices::Z>(const v_ui32_4 & v) noexcept;

    template <>
    const v_ui32_4::ValueType component<VectorIndices::W>(const v_ui32_4 & v) noexcept;

    //}
#pragma endregion

#pragma region _()
    //{ _()

    const v_ui32_4::ValueType x(const v_ui32_4 & v) noexcept;

    const v_ui32_4::ValueType y(const v_ui32_4 & v) noexcept;

    const v_ui32_4::ValueType z(const v_ui32_4 & v) noexcept;

    const v_ui32_4::ValueType w(const v_ui32_4 & v) noexcept;

    //}
#pragma endregion

#pragma region replace_()
    //{ replace_()

    const v_ui32_4 replaceX(const v_ui32_4 & v, const v_ui32_4::ValueType s) noexcept;

    const v_ui32_4 replaceY(const v_ui32_4 & v, const v_ui32_4::ValueType s) noexcept;

    const v_ui32_4 replaceZ(const v_ui32_4 & v, const v_ui32_4::ValueType s) noexcept;

    const v_ui32_4 replaceW(const v_ui32_4 & v, const v_ui32_4::ValueType s) noexcept;

    //}
#pragma endregion

#pragma region Special vector operations
    //{ Special vector operations

    const v_ui32_4 zeroW(const v_ui32_4 & v) noexcept;

    const v_ui32_4 lengthv(const v_ui32_4 & v) noexcept;

    const v_ui32_4 length3v(const v_ui32_4 & v) noexcept;

    const v_ui32_4::ValueType length(const v_ui32_4 & v) noexcept;

    const v_ui32_4::ValueType length3(const v_ui32_4 & v) noexcept;

    const v_ui32_4::ValueType distance(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    const v_ui32_4::ValueType distance3(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    const v_ui32_4 min(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    const v_ui32_4::ValueType min(const v_ui32_4 & v) noexcept;

    const v_ui32_4::ValueType min3(const v_ui32_4 & v) noexcept;

    const v_ui32_4 max(const v_ui32_4 & a, const v_ui32_4 & b) noexcept;

    const v_ui32_4::ValueType max(const v_ui32_4 & v) noexcept;

    const v_ui32_4::ValueType max3(const v_ui32_4 & v) noexcept;

    const v_ui32_4 clamp(const v_ui32_4 & v, const v_ui32_4 & l, const v_ui32_4 & m) noexcept;

    const bool anyTrue(const v_ui32_4 & v) noexcept;

    const bool anyTrue3(const v_ui32_4 & v) noexcept;

    const bool anyFalse(const v_ui32_4 & v) noexcept;

    const bool anyFalse3(const v_ui32_4 & v) noexcept;

    const bool allTrue(const v_ui32_4 & v) noexcept;

    const bool allTrue3(const v_ui32_4 & v) noexcept;

    const bool allFalse(const v_ui32_4 & v) noexcept;

    const bool allFalse3(const v_ui32_4 & v) noexcept;

    //}
#pragma endregion
}
