#include "vectorization/v_f32_4/type.h"

#include "vectorization/constants.h"

#include <cassert>

namespace vectorization
{
    v_f32_4::v_f32_4() noexcept
        : components(Zero<v_f32_4::PackedType>()) { }

    v_f32_4::v_f32_4(const v_f32_4::PackedType & v) noexcept
        : components(v) { }

    v_f32_4::v_f32_4(const v_f32_4::ValueType v) noexcept
        : components(_mm_set_ps1(v)) { }

    v_f32_4::v_f32_4(const v_f32_4::ValueType x, const v_f32_4::ValueType y) noexcept
        : components(_mm_set_ps(Zero<v_f32_4::ValueType>(), Zero<v_f32_4::ValueType>(), y, x)) { }

    v_f32_4::v_f32_4(const v_f32_4::ValueType x, const v_f32_4::ValueType y, const v_f32_4::ValueType z) noexcept
        : components(_mm_set_ps(Zero<v_f32_4::ValueType>(), z, y, x)) { }

    v_f32_4::v_f32_4(const v_f32_4::ValueType x, const v_f32_4::ValueType y, const v_f32_4::ValueType z, const v_f32_4::ValueType w) noexcept
        : components(_mm_set_ps(w, z, y, x)) { }

    v_f32_4::v_f32_4(const v_f32_4::PackedType * const v) noexcept
        : v_f32_4(reinterpret_cast<const v_f32_4::ValueType * const>(v)) { }

    v_f32_4::v_f32_4(const v_f32_4::VectorType * const v) noexcept
        : v_f32_4(reinterpret_cast<const v_f32_4::ValueType * const>(v)) { }

    v_f32_4::v_f32_4(const v_f32_4::ValueType * const v) noexcept
        : components(_mm_load_ps(v)) { }

    v_f32_4 & v_f32_4::operator=(const v_f32_4::PackedType & v) noexcept {
        components = v;
        return *this;
    }

    v_f32_4::ValueType & v_f32_4::operator[](const int index) noexcept {
        assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < SIZE);
        return reinterpret_cast<v_f32_4::ValueType * const>(this)[index];
    }

    const v_f32_4::ValueType & v_f32_4::operator[](const int index) const noexcept {
        assert(static_cast<int>(VectorIndices::X) <= index && static_cast<ASizeT>(index) < SIZE);
        return reinterpret_cast<const v_f32_4::ValueType * const>(this)[index];
    }

    v_f32_4::ValueType & v_f32_4::operator[](const ASizeT index) noexcept {
        assert(index < SIZE);
        return reinterpret_cast<v_f32_4::ValueType * const>(this)[index];
    }

    const v_f32_4::ValueType & v_f32_4::operator[](const ASizeT index) const noexcept {
        assert(index < SIZE);
        return reinterpret_cast<const v_f32_4::ValueType * const>(this)[index];
    }

    void store(const v_f32_4 & v, v_f32_4 * const targetMemory) noexcept {
        store(v, reinterpret_cast<v_f32_4::ValueType * const>(targetMemory));
    }

    void store(const v_f32_4 & v, v_f32_4::PackedType * const targetMemory) noexcept {
        store(v, reinterpret_cast<v_f32_4::ValueType * const>(targetMemory));
    }

    void store(const v_f32_4 & v, v_f32_4::ValueType * const targetMemory) noexcept {
        _mm_store_ps(targetMemory, v.components);
    }
}