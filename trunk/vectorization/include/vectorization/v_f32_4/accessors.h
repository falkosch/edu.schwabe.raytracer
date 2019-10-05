#pragma once

#include "type.h"

namespace vectorization
{
    template <ASizeT Index>
    const v_f32_4::ValueType component(const v_f32_4 & v) noexcept;

    template <>
    const v_f32_4::ValueType component<VectorIndices::X>(const v_f32_4 & v) noexcept;

    template <>
    const v_f32_4::ValueType component<VectorIndices::Y>(const v_f32_4 & v) noexcept;

    template <>
    const v_f32_4::ValueType component<VectorIndices::Z>(const v_f32_4 & v) noexcept;

    template <>
    const v_f32_4::ValueType component<VectorIndices::W>(const v_f32_4 & v) noexcept;

    const v_f32_4::ValueType x(const v_f32_4 & v) noexcept;

    const v_f32_4::ValueType y(const v_f32_4 & v) noexcept;

    const v_f32_4::ValueType z(const v_f32_4 & v) noexcept;

    const v_f32_4::ValueType w(const v_f32_4 & v) noexcept;

    template <ASizeT Index>
    const v_f32_4 replaceComponent(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

    template <>
    const v_f32_4 replaceComponent<VectorIndices::X>(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

    template <>
    const v_f32_4 replaceComponent<VectorIndices::Y>(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

    template <>
    const v_f32_4 replaceComponent<VectorIndices::Z>(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

    template <>
    const v_f32_4 replaceComponent<VectorIndices::W>(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

    const v_f32_4 replaceX(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

    const v_f32_4 replaceY(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

    const v_f32_4 replaceZ(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

    const v_f32_4 replaceW(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

    template <ASizeT Index>
    void setComponent(v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

    template <>
    void setComponent<VectorIndices::X>(v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

    template <>
    void setComponent<VectorIndices::Y>(v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

    template <>
    void setComponent<VectorIndices::Z>(v_f32_4 & v, const v_f32_4::ValueType s) noexcept;

    template <>
    void setComponent<VectorIndices::W>(v_f32_4 & v, const v_f32_4::ValueType s) noexcept;
}
