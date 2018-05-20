#pragma once

#include "v_f32_4.h"
#include "m_i32_4x4.h"

namespace vectorization
{

    // Row-major matrix, 4x4, internally using v_f32_4 as alignment
    struct m_f32_4x4
    {
        // Internal representation
        typedef m_f32_4x4 MatrixType, TransposeType;
        typedef v_f32_4 RowVectorType, ColumnVectorType;
        typedef RowVectorType::PackedType PackedType;
        typedef RowVectorType::ValueType ValueType;
        typedef BoolTypes<ValueType>::Type BoolType;

        static const ASizeT SIZE = RowVectorType::SIZE * ColumnVectorType::SIZE;
        static const ASizeT ROWS = RowVectorType::SIZE;
        static const ASizeT COLUMNS = ColumnVectorType::SIZE;

        typedef m_i32_4x4 MatrixBoolType;

        // Members
        RowVectorType row0, row1, row2, row3;

        m_f32_4x4();

        explicit m_f32_4x4(
            const RowVectorType & row0,
            const RowVectorType & row1,
            const RowVectorType & row2,
            const RowVectorType & row3);

        explicit m_f32_4x4(const RowVectorType::PackedType & row);

        explicit m_f32_4x4(const RowVectorType & row);

        explicit m_f32_4x4(const std::array<ValueType, SIZE> & m);

        explicit m_f32_4x4(const ValueType * const m);

        ALIGNED_ALLOCATORS(__alignof(PackedType));

        MatrixType & operator=(const m_f32_4x4::RowVectorType::PackedType & row);

        ValueType & operator[](const int index);

        const ValueType & operator[](const int index) const;

        ValueType & operator[](const ASizeT index);

        const ValueType & operator[](const ASizeT index) const;

        void setX(const m_f32_4x4::RowVectorType & v);

        void setY(const m_f32_4x4::RowVectorType & v);

        void setZ(const m_f32_4x4::RowVectorType & v);

        void setW(const m_f32_4x4::RowVectorType & v);

    }; // m_f32_4x4

#pragma region Constant generators
    //{ Constant generators

    template <>
    const m_f32_4x4 Zero<m_f32_4x4>();

    template <>
    const m_f32_4x4 Identity<m_f32_4x4>();

    //}
#pragma endregion

#pragma region Operators
    //{ Binary math operators

    const m_f32_4x4 operator*(const m_f32_4x4 & m, const m_f32_4x4::ValueType v);

    const m_f32_4x4 operator*(const m_f32_4x4::ValueType v, const m_f32_4x4 & m);

    const m_f32_4x4 operator+(const m_f32_4x4 & a, const m_f32_4x4 & b);

    const m_f32_4x4 operator*(const m_f32_4x4 & a, const m_f32_4x4 & b);

    const m_f32_4x4::RowVectorType operator*(const m_f32_4x4 & m, const m_f32_4x4::RowVectorType & v);

    const m_f32_4x4::ColumnVectorType operator*(const m_f32_4x4::ColumnVectorType & v, const m_f32_4x4 & m);

    //}
#pragma endregion

#pragma region setRow()
    //{ setRow()

    template <ASizeT RowIndex>
    void setRow(m_f32_4x4 & m, const m_f32_4x4::RowVectorType & v);

    template <>
    void setRow<VectorIndices::X>(m_f32_4x4 & m, const m_f32_4x4::RowVectorType & v);

    template <>
    void setRow<VectorIndices::Y>(m_f32_4x4 & m, const m_f32_4x4::RowVectorType & v);

    template <>
    void setRow<VectorIndices::Z>(m_f32_4x4 & m, const m_f32_4x4::RowVectorType & v);

    template <>
    void setRow<VectorIndices::W>(m_f32_4x4 & m, const m_f32_4x4::RowVectorType & v);

    //}
#pragma endregion

#pragma region setColumn()
    //{ setColumn()

    template <ASizeT ColumnIndex>
    void setColumn(m_f32_4x4 & m, const m_f32_4x4::ColumnVectorType & v);

    template <>
    void setColumn<VectorIndices::X>(m_f32_4x4 & m, const m_f32_4x4::ColumnVectorType & v);

    template <>
    void setColumn<VectorIndices::Y>(m_f32_4x4 & m, const m_f32_4x4::ColumnVectorType & v);

    template <>
    void setColumn<VectorIndices::Z>(m_f32_4x4 & m, const m_f32_4x4::ColumnVectorType & v);

    template <>
    void setColumn<VectorIndices::W>(m_f32_4x4 & m, const m_f32_4x4::ColumnVectorType & v);

    //}
#pragma endregion

#pragma region row()
    //{ row()

    template <ASizeT RowIndex>
    const m_f32_4x4::RowVectorType row(const m_f32_4x4 & m);

    template <>
    const m_f32_4x4::RowVectorType row<VectorIndices::X>(const m_f32_4x4 & m);

    template <>
    const m_f32_4x4::RowVectorType row<VectorIndices::Y>(const m_f32_4x4 & m);

    template <>
    const m_f32_4x4::RowVectorType row<VectorIndices::Z>(const m_f32_4x4 & m);

    template <>
    const m_f32_4x4::RowVectorType row<VectorIndices::W>(const m_f32_4x4 & m);

    //}
#pragma endregion

#pragma region replaceRow()
    //{ replaceRow()

    template <ASizeT RowIndex>
    const m_f32_4x4 replaceRow(const m_f32_4x4 & m, const m_f32_4x4::RowVectorType & v);

    template <>
    const m_f32_4x4 replaceRow<VectorIndices::X>(const m_f32_4x4 & m, const m_f32_4x4::RowVectorType & v);

    template <>
    const m_f32_4x4 replaceRow<VectorIndices::Y>(const m_f32_4x4 & m, const m_f32_4x4::RowVectorType & v);

    template <>
    const m_f32_4x4 replaceRow<VectorIndices::Z>(const m_f32_4x4 & m, const m_f32_4x4::RowVectorType & v);

    template <>
    const m_f32_4x4 replaceRow<VectorIndices::W>(const m_f32_4x4 & m, const m_f32_4x4::RowVectorType & v);

    //}
#pragma endregion

#pragma region column()
    //{ column()

    template <ASizeT ColumnIndex>
    const m_f32_4x4::ColumnVectorType column(const m_f32_4x4 & m);

    template <>
    const m_f32_4x4::ColumnVectorType column<VectorIndices::X>(const m_f32_4x4 & m);

    template <>
    const m_f32_4x4::ColumnVectorType column<VectorIndices::Y>(const m_f32_4x4 & m);

    template <>
    const m_f32_4x4::ColumnVectorType column<VectorIndices::Z>(const m_f32_4x4 & m);

    template <>
    const m_f32_4x4::ColumnVectorType column<VectorIndices::W>(const m_f32_4x4 & m);

    //}
#pragma endregion

#pragma region replaceColumn()
    //{ replaceColumn()

    template <ASizeT ColumnIndex>
    const m_f32_4x4 replaceColumn(const m_f32_4x4 & m, const m_f32_4x4::ColumnVectorType & v);

    template <>
    const m_f32_4x4 replaceColumn<VectorIndices::X>(const m_f32_4x4 & m, const m_f32_4x4::ColumnVectorType & v);

    template <>
    const m_f32_4x4 replaceColumn<VectorIndices::Y>(const m_f32_4x4 & m, const m_f32_4x4::ColumnVectorType & v);

    template <>
    const m_f32_4x4 replaceColumn<VectorIndices::Z>(const m_f32_4x4 & m, const m_f32_4x4::ColumnVectorType & v);

    template <>
    const m_f32_4x4 replaceColumn<VectorIndices::W>(const m_f32_4x4 & m, const m_f32_4x4::ColumnVectorType & v);

    //}
#pragma endregion

#pragma region inverse()
    //{ inverse()

    const m_f32_4x4 inverse(const m_f32_4x4 & m);

    //}
#pragma endregion

#pragma region transpose()
    //{ transpose()

    const m_f32_4x4 transpose(const m_f32_4x4 & m);

    //}
#pragma endregion

#pragma region translate()
    //{ translate()

    const m_f32_4x4 translate(const m_f32_4x4 & m, const m_f32_4x4::RowVectorType & translation);

    //}
#pragma endregion

#pragma region scale()
    //{ scale()

    const m_f32_4x4 scale(const m_f32_4x4 & m, const m_f32_4x4::RowVectorType & scale);

    //}
#pragma endregion

#pragma region rotate()
    //{ rotate()

    const m_f32_4x4 rotate(
        const m_f32_4x4 & m,
        const m_f32_4x4::ValueType angle,
        const m_f32_4x4::RowVectorType & rotationAxis);

    //}
#pragma endregion

#pragma region unproject()
    //{ unproject()

    // Optimized variation of glm:unproject: Doesn't take the projection and
    // view matrix seperately but requires a precomputed inverse of the
    // view-projection-matrix. Also assumes that in viewport.z is "one over
    // viewport-width" and in viewport.w is "one over viewport-height" stored.
    const v_f32_4 unproject(
        const v_f32_4 & screenPosition,
        const m_f32_4x4 & inverseViewProjectionMatrix,
        const v_f32_4 & viewport);

    //}
#pragma endregion

#pragma region ortho()
    //{ ortho()

    const m_f32_4x4 ortho(
        const m_f32_4x4::ValueType left,
        const m_f32_4x4::ValueType right,
        const m_f32_4x4::ValueType top,
        const m_f32_4x4::ValueType bottom);

    //}
#pragma endregion

#pragma region perspectiveFov()
    //{ perspectiveFov()

    const m_f32_4x4 perspectiveFov(
        const m_f32_4x4::ValueType fov,
        const m_f32_4x4::ValueType width,
        const m_f32_4x4::ValueType height,
        const m_f32_4x4::ValueType near,
        const m_f32_4x4::ValueType far);

    //}
#pragma endregion

}
