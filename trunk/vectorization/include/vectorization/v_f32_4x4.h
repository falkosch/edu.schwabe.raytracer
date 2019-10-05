#pragma once

#include "v_f32_4.h"

namespace vectorization
{
    // Structure of arrays of four Vector4s like this:
    // this->soaX = x1, x2, x3, x4
    // this->soaY = y1, y2, y3, y4
    // this->soaZ = z1, z2, z3, z4
    // this->soaW = w1, w2, w3, w4
    struct v_f32_4x4
    {
        // Internal representation
        typedef v_f32_4 SOAVectorType, AOSVectorType;
        typedef SOAVectorType::PackedType PackedType;
        typedef SOAVectorType::ValueType ValueType;
        typedef BoolTypes<ValueType>::Type BoolType;

        // four vectors packed in this SOA
        static const ASizeT SIZE_SOA = SOAVectorType::SIZE;
        static const ASizeT SIZE_AOS = AOSVectorType::SIZE;
        static const ASizeT SIZE = SIZE_SOA * SIZE_AOS;

        // Members
        SOAVectorType soaX, soaY, soaZ, soaW;

        v_f32_4x4() noexcept;

        explicit v_f32_4x4(
            const SOAVectorType::PackedType & soaX,
            const SOAVectorType::PackedType & soaY,
            const SOAVectorType::PackedType & soaZ,
            const SOAVectorType::PackedType & soaW
        ) noexcept;

        explicit v_f32_4x4(
            const SOAVectorType & soaX,
            const SOAVectorType & soaY,
            const SOAVectorType & soaZ,
            const SOAVectorType & soaW
        ) noexcept;

        // broadcasting aos vector into its soa vectors
        explicit v_f32_4x4(const AOSVectorType & aos) noexcept;

        explicit v_f32_4x4(const ValueType * const m) noexcept;

        ALIGNED_ALLOCATORS(__alignof(PackedType));
    }; // v_f32_4x4

#pragma region Accessors
    //{ Accessors

    template <ASizeT Index>
    const v_f32_4x4::SOAVectorType soa(const v_f32_4x4 & m) noexcept;

    template <ASizeT Index>
    void soa(v_f32_4x4 & m, const v_f32_4x4::SOAVectorType & v) noexcept;

    template <ASizeT Index>
    inline const v_f32_4x4::AOSVectorType aos(const v_f32_4x4 & m) noexcept {
        static_assert(Index < v_f32_4x4::SIZE_AOS, "AOS index is out of range");
        return v_f32_4x4::AOSVectorType(
            component<Index>(m.soaX),
            component<Index>(m.soaY),
            component<Index>(m.soaZ),
            component<Index>(m.soaW)
        );
    }

    template <ASizeT Index>
    inline void aos(v_f32_4x4 & m, const v_f32_4x4::AOSVectorType & v) noexcept {
        static_assert(Index < v_f32_4x4::SIZE_AOS, "AOS index is out of range");
        component<Index>(m.soaX, component<VectorIndices::X>(v));
        component<Index>(m.soaY, component<VectorIndices::Y>(v));
        component<Index>(m.soaZ, component<VectorIndices::Z>(v));
        component<Index>(m.soaW, component<VectorIndices::W>(v));
    }

    template <>
    const v_f32_4x4::SOAVectorType soa<VectorIndices::X>(const v_f32_4x4 & m) noexcept;

    template <>
    const v_f32_4x4::SOAVectorType soa<VectorIndices::Y>(const v_f32_4x4 & m) noexcept;

    template <>
    const v_f32_4x4::SOAVectorType soa<VectorIndices::Z>(const v_f32_4x4 & m) noexcept;

    template <>
    const v_f32_4x4::SOAVectorType soa<VectorIndices::W>(const v_f32_4x4 & m) noexcept;

    template <>
    void soa<VectorIndices::X>(v_f32_4x4 & m, const v_f32_4x4::SOAVectorType & v) noexcept;

    template <>
    void soa<VectorIndices::Y>(v_f32_4x4 & m, const v_f32_4x4::SOAVectorType & v) noexcept;

    template <>
    void soa<VectorIndices::Z>(v_f32_4x4 & m, const v_f32_4x4::SOAVectorType & v) noexcept;

    template <>
    void soa<VectorIndices::W>(v_f32_4x4 & m, const v_f32_4x4::SOAVectorType & v) noexcept;

    //}
#pragma endregion

#pragma region Operators
    //{ Operators

    const v_f32_4x4 operator+(const v_f32_4x4 & a, const v_f32_4x4 & b) noexcept;

    const v_f32_4x4 operator-(const v_f32_4x4 & a, const v_f32_4x4 & b) noexcept;

    const v_f32_4x4 operator*(const v_f32_4x4 & a, const v_f32_4x4 & b) noexcept;

    const v_f32_4x4 operator/(const v_f32_4x4 & a, const v_f32_4x4 & b) noexcept;

    const v_f32_4x4 operator%(const v_f32_4x4 & a, const v_f32_4x4 & b) noexcept;

    const v_f32_4x4 operator&(const v_f32_4x4 & a, const v_f32_4x4 & b) noexcept;

    const v_f32_4x4 operator|(const v_f32_4x4 & a, const v_f32_4x4 & b) noexcept;

    const v_f32_4x4 operator^(const v_f32_4x4 & a, const v_f32_4x4 & b) noexcept;

    const v_f32_4x4 operator+=(v_f32_4x4 & a, const v_f32_4x4 & b) noexcept;

    const v_f32_4x4 operator-=(v_f32_4x4 & a, const v_f32_4x4 & b) noexcept;

    const v_f32_4x4 operator*=(v_f32_4x4 & a, const v_f32_4x4 & b) noexcept;

    const v_f32_4x4 operator/=(v_f32_4x4 & a, const v_f32_4x4 & b) noexcept;

    const v_f32_4x4 operator%=(v_f32_4x4 & a, const v_f32_4x4 & b) noexcept;

    const v_f32_4x4 operator&=(v_f32_4x4 & a, const v_f32_4x4 & b) noexcept;

    const v_f32_4x4 operator|=(v_f32_4x4 & a, const v_f32_4x4 & b) noexcept;

    const v_f32_4x4 operator^=(v_f32_4x4 & a, const v_f32_4x4 & b) noexcept;

    //}
#pragma endregion

#pragma region Special vector operations
    //{ Special vector operations

    const v_f32_4x4 transpose(const v_f32_4x4 & m) noexcept;

    //}
#pragma endregion
}
