#pragma once

#include "utvector.h"

namespace vectorization
{

    // packed SSE-vector of four int32 scalars
    struct v_i32_4
    {
        // Metas
        static const ASizeT SIZE = VectorSizes::W;
        typedef Int_32 ValueType;
        typedef ValueType BoolType;
        typedef v_i32_4 VectorType;
        typedef VectorType VectorBoolType;
        typedef PackedTypes<ValueType, SIZE>::Type PackedType;

        // the actual data
        PackedType components;

        v_i32_4();

        v_i32_4(const PackedType & v);

        explicit v_i32_4(const ValueType s);

        explicit v_i32_4(const ValueType x, const ValueType y);

        explicit v_i32_4(const ValueType x, const ValueType y, const ValueType z);

        explicit v_i32_4(const ValueType x, const ValueType y, const ValueType z, const ValueType w);

        ALIGNED_ALLOCATORS(__alignof(PackedType));

        operator const PackedType()
        {
            return components;
        }

        operator const PackedType() const
        {
            return components;
        }

        v_i32_4 & operator=(const PackedType & v);

        ValueType & operator[](const int index);

        const ValueType & operator[](const int index) const;

        ValueType & operator[](const ASizeT index);

        const ValueType & operator[](const ASizeT index) const;

        void setX(const ValueType v);

        void setY(const ValueType v);

        void setZ(const ValueType v);

        void setW(const ValueType v);

    }; // v_i32_4

#pragma region Constant generators
    //{Constant generators

    template <>
    const v_i32_4 MaskAll<v_i32_4>();

    template <>
    const v_i32_4 MaskX<v_i32_4>();

    template <>
    const v_i32_4 MaskXY<v_i32_4>();

    template <>
    const v_i32_4 MaskXYZ<v_i32_4>();

    template <>
    const v_i32_4 MaskXYZW<v_i32_4>();

    template <>
    const v_i32_4 MaskY<v_i32_4>();

    template <>
    const v_i32_4 MaskYZ<v_i32_4>();

    template <>
    const v_i32_4 MaskYZW<v_i32_4>();

    template <>
    const v_i32_4 MaskZ<v_i32_4>();

    template <>
    const v_i32_4 MaskZW<v_i32_4>();

    template <>
    const v_i32_4 MaskW<v_i32_4>();

    template <>
    const v_i32_4 OneW<v_i32_4>();

    template <>
    const v_i32_4 Zero<v_i32_4>();

    template <>
    const v_i32_4 One<v_i32_4>();

    template <>
    const v_i32_4 Two<v_i32_4>();

    template <>
    const v_i32_4 NegOne<v_i32_4>();

    template <>
    const v_i32_4 NegTwo<v_i32_4>();

    //}
#pragma endregion

#pragma region Commonly used swizzle accessors
    // Commonly used swizzle accessors

    const v_i32_4 xxxx(const v_i32_4 & v);

    const v_i32_4 yyyy(const v_i32_4 & v);

    const v_i32_4 zzzz(const v_i32_4 & v);

    const v_i32_4 wwww(const v_i32_4 & v);

    const v_i32_4 xxyy(const v_i32_4 & v);

    const v_i32_4 xyxy(const v_i32_4 & v);

    const v_i32_4 yxwz(const v_i32_4 & v);

    const v_i32_4 zzww(const v_i32_4 & v);

    const v_i32_4 zwzw(const v_i32_4 & v);

    const v_i32_4 xy_xy(const v_i32_4 & a, const v_i32_4 & b);

    const v_i32_4 zw_zw(const v_i32_4 & a, const v_i32_4 & b);

    //}
#pragma endregion

#pragma region Operators
    //{Operators

    const v_i32_4 operator-(const v_i32_4 & v);

    const v_i32_4 operator!(const v_i32_4 & v);

    const v_i32_4 operator~(const v_i32_4 & v);

    const v_i32_4 operator+(const v_i32_4 & a, const v_i32_4 & b);

    const v_i32_4 operator-(const v_i32_4 & a, const v_i32_4 & b);

    const v_i32_4 operator*(const v_i32_4 & a, const v_i32_4 & b);

    const v_i32_4 operator/(const v_i32_4 & a, const v_i32_4 & b);

    const v_i32_4 operator%(const v_i32_4 & a, const v_i32_4 & b);

    const v_i32_4 operator&(const v_i32_4 & a, const v_i32_4 & b);

    const v_i32_4 operator|(const v_i32_4 & a, const v_i32_4 & b);

    const v_i32_4 operator^(const v_i32_4 & a, const v_i32_4 & b);

    const v_i32_4 operator<<(const v_i32_4 & a, const v_i32_4 & b);

    const v_i32_4 operator>>(const v_i32_4 & a, const v_i32_4 & b);

    const v_i32_4 operator<(const v_i32_4 & a, const v_i32_4 & b);

    const v_i32_4 operator>(const v_i32_4 & a, const v_i32_4 & b);

    const v_i32_4 operator<=(const v_i32_4 & a, const v_i32_4 & b);

    const v_i32_4 operator>=(const v_i32_4 & a, const v_i32_4 & b);

    const v_i32_4 operator==(const v_i32_4 & a, const v_i32_4 & b);

    const v_i32_4 operator!=(const v_i32_4 & a, const v_i32_4 & b);

    v_i32_4 & operator+=(v_i32_4 & a, const v_i32_4 & b);

    v_i32_4 & operator-=(v_i32_4 & a, const v_i32_4 & b);

    v_i32_4 & operator*=(v_i32_4 & a, const v_i32_4 & b);

    v_i32_4 & operator/=(v_i32_4 & a, const v_i32_4 & b);

    v_i32_4 & operator%=(v_i32_4 & a, const v_i32_4 & b);

    v_i32_4 & operator&=(v_i32_4 & a, const v_i32_4 & b);

    v_i32_4 & operator|=(v_i32_4 & a, const v_i32_4 & b);

    v_i32_4 & operator^=(v_i32_4 & a, const v_i32_4 & b);

    v_i32_4 & operator<<=(v_i32_4 & a, const v_i32_4 & b);

    v_i32_4 & operator>>=(v_i32_4 & a, const v_i32_4 & b);

    //}
#pragma endregion

#pragma region swizzle()
    //{ swizzle()

    template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
    // generic swizzle
    inline const v_i32_4 swizzle(const v_i32_4 & v)
    {
        static_assert(X < v_i32_4::SIZE, "Index is out of range");
        static_assert(Y < v_i32_4::SIZE, "Index is out of range");
        static_assert(Z < v_i32_4::SIZE, "Index is out of range");
        static_assert(W < v_i32_4::SIZE, "Index is out of range");
        return _mm_shuffle_epi32(v.components, _MM_SHUFFLE(W, Z, Y, X));
    }

    template <>
    // passthrough swizzle
    const v_i32_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_i32_4 & v);

    template <>
    // special unpack low-values swizzle
    const v_i32_4 swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(const v_i32_4 & v);

    template <>
    // special unpack high-values swizzle
    const v_i32_4 swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(const v_i32_4 & v);

    template <>
    // special unpack XY-values swizzle
    const v_i32_4 swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const v_i32_4 & v);

    template <>
    // special unpack ZW-values swizzle
    const v_i32_4 swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const v_i32_4 & v);

    //}
#pragma endregion

#pragma region blend()
    //{ blend()

    template <bool SelectX, bool SelectY, bool SelectZ, bool SelectW>
    // generic blend of two vectors
    inline const v_i32_4 blend(const v_i32_4 & a, const v_i32_4 & b)
    {
        return _mm_blend_epi16(a.components, b.components, BLEND_MASK_i4(SelectX, SelectY, SelectZ, SelectW));
    }

    template <>
    // passtrough blend
    const v_i32_4 blend<false, false, false, false>(const v_i32_4 & a, const v_i32_4 &);

    template <>
    // passthrough blend other
    const v_i32_4 blend<true, true, true, true>(const v_i32_4 &, const v_i32_4 & b);

    //}
#pragma endregion

#pragma region swizzledBlend()
    //{ swizzledBlend()

    template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W, bool SelectX, bool SelectY, bool SelectZ, bool SelectW>
    // generic swizzled blend
    inline const v_i32_4 swizzledBlend(const v_i32_4 & a, const v_i32_4 & b)
    {
        return blend<SelectX, SelectY, SelectZ, SelectW>(swizzle<X, Y, Z, W>(a), swizzle<X, Y, Z, W>(b));
    }

    template <>
    const v_i32_4 swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, false, true, false, true>(const v_i32_4 & a, const v_i32_4 & b);

    template <>
    const v_i32_4 swizzledBlend<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y, true, false, true, false>(const v_i32_4 & a, const v_i32_4 & b);

    template <>
    const v_i32_4 swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, false, true, false, true>(const v_i32_4 & a, const v_i32_4 & b);

    template <>
    const v_i32_4 swizzledBlend<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W, true, false, true, false>(const v_i32_4 & a, const v_i32_4 & b);

    template <>
    const v_i32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, false, false, true, true>(const v_i32_4 & a, const v_i32_4 & b);

    template <>
    const v_i32_4 swizzledBlend<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y, true, true, false, false>(const v_i32_4 & a, const v_i32_4 & b);

    template <>
    const v_i32_4 swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, false, false, true, true>(const v_i32_4 & a, const v_i32_4 & b);

    template <>
    const v_i32_4 swizzledBlend<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W, true, true, false, false>(const v_i32_4 & a, const v_i32_4 & b);

    template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
    // Special case swizzled blend of two vectors, in which only the first
    // two components of this vector and the last two components of the
    // other vector are taken for blending after swizzling.
    inline const v_i32_4 swizzledBlend_2x2(const v_i32_4 & a, const v_i32_4 & b)
    {
        return swizzledBlend_2x2<X, Y, Z, W, false, false, true, true>(a, b);
    }

    template <>
    const v_i32_4 swizzledBlend_2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(const v_i32_4 & a, const v_i32_4 & b);

    template <>
    const v_i32_4 swizzledBlend_2x2<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(const v_i32_4 & a, const v_i32_4 & b);

    template <>
    const v_i32_4 swizzledBlend_2x2<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(const v_i32_4 & a, const v_i32_4 & b);

    //}
#pragma endregion

#pragma region blendMasked()
    //{ blendMasked()

    const v_i32_4 blendMasked(const v_i32_4 & a, const v_i32_4 & b, const v_i32_4 & mask);

    //}
#pragma endregion

#pragma region swizzledBlendMasked()
    //{ swizzledBlendMasked()

    template <ASizeT X, ASizeT Y, ASizeT Z, ASizeT W>
    inline const v_i32_4 swizzledBlendMasked(const v_i32_4 & a, const v_i32_4 & b, const v_i32_4 & mask)
    {
        return blendMasked(swizzle<X, Y, Z, W>(a), swizzle<X, Y, Z, W>(b), mask);
    }

    //}
#pragma endregion

#pragma region setComponent()
    //{ setComponent()

    template <ASizeT Index>
    void setComponent(v_i32_4 & v, const v_i32_4::ValueType s);

    template <>
    void setComponent<VectorIndices::X>(v_i32_4 & v, const v_i32_4::ValueType s);

    template <>
    void setComponent<VectorIndices::Y>(v_i32_4 & v, const v_i32_4::ValueType s);

    template <>
    void setComponent<VectorIndices::Z>(v_i32_4 & v, const v_i32_4::ValueType s);

    template <>
    void setComponent<VectorIndices::W>(v_i32_4 & v, const v_i32_4::ValueType s);

    //}
#pragma endregion

#pragma region replaceComponent()
    //{ replaceComponent()

    template <ASizeT Index>
    const v_i32_4 replaceComponent(const v_i32_4 & v, const v_i32_4::ValueType s);

    template <>
    const v_i32_4 replaceComponent<VectorIndices::X>(const v_i32_4 & v, const v_i32_4::ValueType s);

    template <>
    const v_i32_4 replaceComponent<VectorIndices::Y>(const v_i32_4 & v, const v_i32_4::ValueType s);

    template <>
    const v_i32_4 replaceComponent<VectorIndices::Z>(const v_i32_4 & v, const v_i32_4::ValueType s);

    template <>
    const v_i32_4 replaceComponent<VectorIndices::W>(const v_i32_4 & v, const v_i32_4::ValueType s);

    //}
#pragma endregion

#pragma region component()
    //{ component()

    template <ASizeT Index>
    const v_i32_4::ValueType component(const v_i32_4 & v);

    template <>
    const v_i32_4::ValueType component<VectorIndices::X>(const v_i32_4 & v);

    template <>
    const v_i32_4::ValueType component<VectorIndices::Y>(const v_i32_4 & v);

    template <>
    const v_i32_4::ValueType component<VectorIndices::Z>(const v_i32_4 & v);

    template <>
    const v_i32_4::ValueType component<VectorIndices::W>(const v_i32_4 & v);

    //}
#pragma endregion

#pragma region _()
    //{ _()

    const v_i32_4::ValueType x(const v_i32_4 & v);

    const v_i32_4::ValueType y(const v_i32_4 & v);

    const v_i32_4::ValueType z(const v_i32_4 & v);

    const v_i32_4::ValueType w(const v_i32_4 & v);

    //}
#pragma endregion

#pragma region replace_()
    //{ replace_()

    const v_i32_4 replaceX(const v_i32_4 & v, const v_i32_4::ValueType s);

    const v_i32_4 replaceY(const v_i32_4 & v, const v_i32_4::ValueType s);

    const v_i32_4 replaceZ(const v_i32_4 & v, const v_i32_4::ValueType s);

    const v_i32_4 replaceW(const v_i32_4 & v, const v_i32_4::ValueType s);

    //}
#pragma endregion

#pragma region min()
    //{ min()

    const v_i32_4 min(const v_i32_4 & a, const v_i32_4 & b);

    const v_i32_4::ValueType min(const v_i32_4 & v);

    const v_i32_4::ValueType min3(const v_i32_4 & v);

    //}
#pragma endregion

#pragma region max()
    //{ max()

    const v_i32_4 max(const v_i32_4 & a, const v_i32_4 & b);

    const v_i32_4::ValueType max(const v_i32_4 & v);

    const v_i32_4::ValueType max3(const v_i32_4 & v);

    //}
#pragma endregion

#pragma region Special vector operations
    //{ Special vector operations

    const v_i32_4 zeroW(const v_i32_4 & v);

    // returns ((~a) & b)
    const v_i32_4 andnot(const v_i32_4 & a, const v_i32_4 & b);

    // Returns a bitmask indicating components with negative values. A set bit indicates a value
    // less than "0".
    const v_i32_4::BoolType isNegative(const v_i32_4 & v);

    const v_i32_4 abs(const v_i32_4 & v);

    const v_i32_4 lengthv(const v_i32_4 & v);

    const v_i32_4 length3v(const v_i32_4 & v);

    const v_i32_4::ValueType length(const v_i32_4 & v);

    const v_i32_4::ValueType length3(const v_i32_4 & v);

    const v_i32_4::ValueType distance(const v_i32_4 & a, const v_i32_4 & b);

    const v_i32_4::ValueType distance3(const v_i32_4 & a, const v_i32_4 & b);

    const v_i32_4 clamp(const v_i32_4 & v, const v_i32_4 & l, const v_i32_4 & m);

    const bool anyTrue(const v_i32_4 & v);

    const bool anyTrue3(const v_i32_4 & v);

    const bool anyFalse(const v_i32_4 & v);

    const bool anyFalse3(const v_i32_4 & v);

    const bool allTrue(const v_i32_4 & v);

    const bool allTrue3(const v_i32_4 & v);

    const bool allFalse(const v_i32_4 & v);

    const bool allFalse3(const v_i32_4 & v);

    //}
#pragma endregion

}
