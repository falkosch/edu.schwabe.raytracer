#pragma once

#include "vectorization/v_f32_4/type.h"
#include "vectorization/v_i32_8/type.h"

#include "vectorization/architecture.h"

namespace vectorization
{
    struct v_f32_8
    {
        static constexpr ASizeT SIZE = VectorSizes::X8;
        static constexpr ASizeT HALF_SIZE = VectorSizes::W;
        using ValueType = Float_32;
        using BoolType = v_i32_8::ValueType;
        using VectorType = v_f32_8;
        using HalfVectorType = v_f32_4;
        using VectorBoolType = v_i32_8;
        using HalfPackedType = PackedTypes<ValueType, HALF_SIZE>::Type;

#if VECTORIZATION_INTRINSICS_LEVEL >= VECTORIZATION_AVX
        using PackedType = PackedTypes<ValueType, SIZE>::Type;
#else
        struct PackedType
        {
            HalfPackedType lo;
            HalfPackedType hi;
        };
#endif

        PackedType components{};

        v_f32_8() noexcept;
        v_f32_8(const PackedType& vector) noexcept;
        // Broadcasts 4-lanes packed type to span the 8 lanes (xmm -> xmm x xmm = ymm).
        explicit v_f32_8(const HalfPackedType& half_to_broadcast) noexcept;
        // Broadcasts half vector type to span the full vector, so that v[i] == v[i+HALF_SIZE].
        explicit v_f32_8(const HalfVectorType& half_to_broadcast) noexcept;
        explicit v_f32_8(ValueType scalar) noexcept;
        explicit v_f32_8(ValueType x1, ValueType x2, ValueType x3, ValueType x4) noexcept;
        explicit v_f32_8(
            ValueType x1, ValueType x2, ValueType x3, ValueType x4,
            ValueType x5, ValueType x6, ValueType x7, ValueType x8
        ) noexcept;
        explicit v_f32_8(const ValueType* values) noexcept;

        VectorType& operator=(const PackedType& vector) noexcept;

        ValueType& operator[](int index) noexcept;
        const ValueType& operator[](int index) const noexcept;
        ValueType& operator[](ASizeT index) noexcept;
        const ValueType& operator[](ASizeT index) const noexcept;
    };

    void store(const v_f32_8& src, v_f32_8* dst) noexcept;

    void store(const v_f32_8& src, v_f32_8::PackedType* dst) noexcept;

    void store(const v_f32_8& src, v_f32_8::ValueType* dst) noexcept;
}
