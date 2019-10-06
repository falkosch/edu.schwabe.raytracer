#include "vectorization/v_f32_4/accessors.h"

#include "vectorization/v_f32_4/swizzles.h"

namespace vectorization
{
    template <>
    const v_f32_4::ValueType component<VectorIndices::X>(const v_f32_4 & v) noexcept {
        return _mm_cvtss_f32(v.components);
    }

    template <>
    const v_f32_4::ValueType component<VectorIndices::Y>(const v_f32_4 & v) noexcept {
        return _mm_cvtss_f32(yyww(v).components);
    }

    template <>
    const v_f32_4::ValueType component<VectorIndices::Z>(const v_f32_4 & v) noexcept {
        return _mm_cvtss_f32(zwzw(v).components);
    }

    template <>
    const v_f32_4::ValueType component<VectorIndices::W>(const v_f32_4 & v) noexcept {
        return _mm_cvtss_f32(wwww(v).components);
    }

    const v_f32_4::ValueType x(const v_f32_4 & v) noexcept {
        return component<VectorIndices::X>(v);
    }

    const v_f32_4::ValueType y(const v_f32_4 & v) noexcept {
        return component<VectorIndices::Y>(v);
    }

    const v_f32_4::ValueType z(const v_f32_4 & v) noexcept {
        return component<VectorIndices::Z>(v);
    }

    const v_f32_4::ValueType w(const v_f32_4 & v) noexcept {
        return component<VectorIndices::W>(v);
    }

    template <>
    const v_f32_4 replaceComponent<VectorIndices::X>(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept {
        return _mm_insert_ps(v.components, _mm_set_ss(s), _MM_MK_INSERTPS_NDX(VectorIndices::X, VectorIndices::X, VectorBits::None));
    }

    template <>
    const v_f32_4 replaceComponent<VectorIndices::Y>(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept {
        return _mm_insert_ps(v.components, _mm_set_ss(s), _MM_MK_INSERTPS_NDX(VectorIndices::X, VectorIndices::Y, VectorBits::None));
    }

    template <>
    const v_f32_4 replaceComponent<VectorIndices::Z>(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept {
        return _mm_insert_ps(v.components, _mm_set_ss(s), _MM_MK_INSERTPS_NDX(VectorIndices::X, VectorIndices::Z, VectorBits::None));
    }

    template <>
    const v_f32_4 replaceComponent<VectorIndices::W>(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept {
        return _mm_insert_ps(v.components, _mm_set_ss(s), _MM_MK_INSERTPS_NDX(VectorIndices::X, VectorIndices::W, VectorBits::None));
    }

    const v_f32_4 replaceX(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept {
        return replaceComponent<VectorIndices::X>(v, s);
    }

    const v_f32_4 replaceY(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept {
        return replaceComponent<VectorIndices::Y>(v, s);
    }

    const v_f32_4 replaceZ(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept {
        return replaceComponent<VectorIndices::Z>(v, s);
    }

    const v_f32_4 replaceW(const v_f32_4 & v, const v_f32_4::ValueType s) noexcept {
        return replaceComponent<VectorIndices::W>(v, s);
    }

    template <>
    void setComponent<VectorIndices::X>(v_f32_4 & v, const v_f32_4::ValueType s) noexcept {
        v.components = replaceComponent<VectorIndices::X>(v, s).components;
    }

    template <>
    void setComponent<VectorIndices::Y>(v_f32_4 & v, const v_f32_4::ValueType s) noexcept {
        v.components = replaceComponent<VectorIndices::Y>(v, s).components;
    }

    template <>
    void setComponent<VectorIndices::Z>(v_f32_4 & v, const v_f32_4::ValueType s) noexcept {
        v.components = replaceComponent<VectorIndices::Z>(v, s).components;
    }

    template <>
    void setComponent<VectorIndices::W>(v_f32_4 & v, const v_f32_4::ValueType s) noexcept {
        v.components = replaceComponent<VectorIndices::W>(v, s).components;
    }
}