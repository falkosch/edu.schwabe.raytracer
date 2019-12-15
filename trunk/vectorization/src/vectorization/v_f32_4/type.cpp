#include "vectorization/v_f32_4/type.h"

#include "vectorization/constants.h"

#include <array>
#include <cassert>
#include <cstring>

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
        : components() {
        std::memcpy(&(this->components), v, sizeof v_f32_4::PackedType);
    }

    v_f32_4::v_f32_4(const v_f32_4::VectorType * const v) noexcept
        : components() {
        std::memcpy(&(this->components), &(v->components), sizeof v_f32_4::PackedType);
    }

    v_f32_4::v_f32_4(const v_f32_4::ValueType * const v) noexcept
        : components(_mm_load_ps(v)) { }

    v_f32_4 & v_f32_4::operator=(const v_f32_4::PackedType & v) noexcept {
        components = v;
        return *this;
    }

    const v_f32_4::ValueType & v_f32_4::operator[](const ASizeT index) const noexcept {
        assert(index < SIZE);
        return reinterpret_cast<const v_f32_4::ValueType * const>(&(this->components))[index];
    }

    void store(const v_f32_4 & src, v_f32_4 * const dst) noexcept {
        std::memcpy(&(dst->components), &src.components, sizeof v_f32_4::PackedType);
    }

    void store(const v_f32_4 & src, v_f32_4::PackedType * const dst) noexcept {
        std::memcpy(dst, &src.components, sizeof v_f32_4::PackedType);
    }

    void store(const v_f32_4 & src, v_f32_4::ValueType * const dst) noexcept {
        _mm_store_ps(dst, src.components);
    }
}