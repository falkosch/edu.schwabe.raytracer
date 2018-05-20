#pragma once

#include "v_i32_4.h"

namespace vectorization
{

    // Row-major matrix, 4x4, internally using v_i32_4 as alignment
    struct m_i32_4x4
    {
        // Internal representation
        typedef m_i32_4x4 MatrixType, TransposeType;
        typedef v_i32_4 RowVectorType, ColumnVectorType;
        typedef RowVectorType::PackedType PackedType;
        typedef RowVectorType::ValueType ValueType;
        typedef BoolTypes<ValueType>::Type BoolType;

        static const ASizeT SIZE = RowVectorType::SIZE * ColumnVectorType::SIZE;
        static const ASizeT ROWS = RowVectorType::SIZE;
        static const ASizeT COLUMNS = ColumnVectorType::SIZE;

        typedef MatrixType MatrixBoolType;

        // Members
        RowVectorType row0, row1, row2, row3;

        m_i32_4x4();

        explicit m_i32_4x4(
            const RowVectorType & row0,
            const RowVectorType & row1,
            const RowVectorType & row2,
            const RowVectorType & row3);

        explicit m_i32_4x4(const RowVectorType::PackedType & row);

        explicit m_i32_4x4(const RowVectorType & row);

        explicit m_i32_4x4(const std::array<ValueType, SIZE> & m);

        explicit m_i32_4x4(const ValueType * const m);

        ALIGNED_ALLOCATORS(__alignof(PackedType));

        MatrixType & operator=(const m_i32_4x4::RowVectorType::PackedType & row);

        ValueType & operator[](const int index);

        const ValueType & operator[](const int index) const;

        ValueType & operator[](const ASizeT index);

        const ValueType & operator[](const ASizeT index) const;

    }; // m_i32_4x4

#pragma region Constant generators
    //{ Constant generators

    template <>
    const m_i32_4x4 Zero<m_i32_4x4>();

    //}
#pragma endregion

#pragma region Operators
    //{ Binary math operators

    const m_i32_4x4 operator*(const m_i32_4x4 & m, const m_i32_4x4::ValueType v);

    const m_i32_4x4 operator+(const m_i32_4x4 & a, const m_i32_4x4 & b);

    const m_i32_4x4 operator*(const m_i32_4x4 & a, const m_i32_4x4 & b);

    const m_i32_4x4::RowVectorType operator*(const m_i32_4x4 & m, const m_i32_4x4::RowVectorType & v);

    //}
#pragma endregion

#pragma region setRow()
    //{ setRow()

    template <ASizeT RowIndex>
    void setRow(m_i32_4x4 & m, const m_i32_4x4::RowVectorType & v);

    template <>
    void setRow<VectorIndices::X>(m_i32_4x4 & m, const m_i32_4x4::RowVectorType & v);

    template <>
    void setRow<VectorIndices::Y>(m_i32_4x4 & m, const m_i32_4x4::RowVectorType & v);

    template <>
    void setRow<VectorIndices::Z>(m_i32_4x4 & m, const m_i32_4x4::RowVectorType & v);

    template <>
    void setRow<VectorIndices::W>(m_i32_4x4 & m, const m_i32_4x4::RowVectorType & v);

    //}
#pragma endregion

#pragma region setColumn()
    //{ setColumn()

    template <ASizeT ColumnIndex>
    void setColumn(m_i32_4x4 & m, const m_i32_4x4::ColumnVectorType & v);

    template <>
    void setColumn<VectorIndices::X>(m_i32_4x4 & m, const m_i32_4x4::ColumnVectorType & v);

    template <>
    void setColumn<VectorIndices::Y>(m_i32_4x4 & m, const m_i32_4x4::ColumnVectorType & v);

    template <>
    void setColumn<VectorIndices::Z>(m_i32_4x4 & m, const m_i32_4x4::ColumnVectorType & v);

    template <>
    void setColumn<VectorIndices::W>(m_i32_4x4 & m, const m_i32_4x4::ColumnVectorType & v);

    //}
#pragma endregion

#pragma region row()
    //{ row()

    template <ASizeT RowIndex>
    const m_i32_4x4::RowVectorType row(const m_i32_4x4 & m);

    template <>
    const m_i32_4x4::RowVectorType row<VectorIndices::X>(const m_i32_4x4 & m);

    template <>
    const m_i32_4x4::RowVectorType row<VectorIndices::Y>(const m_i32_4x4 & m);

    template <>
    const m_i32_4x4::RowVectorType row<VectorIndices::Z>(const m_i32_4x4 & m);

    template <>
    const m_i32_4x4::RowVectorType row<VectorIndices::W>(const m_i32_4x4 & m);

    //}
#pragma endregion

#pragma region column()
    //{ column()

    template <ASizeT ColumnIndex>
    const m_i32_4x4::ColumnVectorType column(const m_i32_4x4 & m);

    template <>
    const m_i32_4x4::ColumnVectorType column<VectorIndices::X>(const m_i32_4x4 & m);

    template <>
    const m_i32_4x4::ColumnVectorType column<VectorIndices::Y>(const m_i32_4x4 & m);

    template <>
    const m_i32_4x4::ColumnVectorType column<VectorIndices::Z>(const m_i32_4x4 & m);

    template <>
    const m_i32_4x4::ColumnVectorType column<VectorIndices::W>(const m_i32_4x4 & m);

    //}
#pragma endregion

#pragma region transpose()
    //{ transpose()

    const m_i32_4x4 transpose(const m_i32_4x4 & m);

    //}
#pragma endregion

}
