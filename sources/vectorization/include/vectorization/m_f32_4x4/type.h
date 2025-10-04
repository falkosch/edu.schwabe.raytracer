#pragma once

#include "api.h"

#include "vectorization/v_f32_4.h"

#include "vectorization/architecture.h"

#include <array>

namespace vectorization {
  struct m_f32_4x4 {
    using MatrixType = m_f32_4x4;
    using TransposeType = m_f32_4x4;
    using RowVectorType = v_f32_4;
    using ColumnVectorType = v_f32_4;
    using PackedType = RowVectorType::PackedType;
    using ValueType = RowVectorType::ValueType;
    using BoolType = BoolTypes<ValueType>::Type;

    static constexpr ASizeT ROWS = RowVectorType::SIZE;
    static constexpr ASizeT COLUMNS = ColumnVectorType::SIZE;
    static constexpr ASizeT SIZE = ROWS * COLUMNS;

    RowVectorType row0, row1, row2, row3;

    m_f32_4x4() noexcept;
    explicit m_f32_4x4(
        const RowVectorType &row0In, const RowVectorType &row1In, const RowVectorType &row2In,
        const RowVectorType &row3In
    ) noexcept;
    explicit m_f32_4x4(const RowVectorType::PackedType &row) noexcept;
    explicit m_f32_4x4(const RowVectorType &row) noexcept;
    explicit m_f32_4x4(const std::array<ValueType, SIZE> &data) noexcept;
    explicit m_f32_4x4(const ValueType *values) noexcept;

    MatrixType &operator=(const RowVectorType::PackedType &row) noexcept;

    ValueType &operator[](int index) noexcept;
    const ValueType &operator[](int index) const noexcept;
    ValueType &operator[](ASizeT index) noexcept;
    const ValueType &operator[](ASizeT index) const noexcept;
  };
}
