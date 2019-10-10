#include "vectorization/accessors/component_256s.h"

namespace vectorization
{
    inline const PackedFloat8_256 hi128_lo128(const PackedFloat8_256 v) {
        return _mm256_permute2f128_ps(v, v, 0b00000001);
    }

    inline const PackedFloat8_256 zxzx_zxzx(const PackedFloat8_256 v) {
        return _mm256_permute_ps(v, 0b00100010);
    }

    inline const PackedFloat8_256 wywy_wywy(const PackedFloat8_256 v) {
        return _mm256_permute_ps(v, 0b01110111);
    }

    template <>
    const Float_32 component<VectorIndices::X1>(const PackedFloat8_256 & v) noexcept {
        return _mm256_cvtss_f32(v);
    }

    template <>
    const Float_32 component<VectorIndices::X2>(const PackedFloat8_256 & v) noexcept {
        return _mm256_cvtss_f32(_mm256_movehdup_ps(v));
    }

    template <>
    const Float_32 component<VectorIndices::X3>(const PackedFloat8_256 & v) noexcept {
        return _mm256_cvtss_f32(zxzx_zxzx(v));
    }

    template <>
    const Float_32 component<VectorIndices::X4>(const PackedFloat8_256 & v) noexcept {
        return _mm256_cvtss_f32(wywy_wywy(v));
    }

    template <>
    const Float_32 component<VectorIndices::X5>(const PackedFloat8_256 & v) noexcept {
        return _mm256_cvtss_f32(hi128_lo128(v));
    }

    template <>
    const Float_32 component<VectorIndices::X6>(const PackedFloat8_256 & v) noexcept {
        return _mm256_cvtss_f32(_mm256_movehdup_ps(hi128_lo128(v)));
    }

    template <>
    const Float_32 component<VectorIndices::X7>(const PackedFloat8_256 & v) noexcept {
        return _mm256_cvtss_f32(zxzx_zxzx(hi128_lo128(v)));
    }

    template <>
    const Float_32 component<VectorIndices::X8>(const PackedFloat8_256 & v) noexcept {
        return _mm256_cvtss_f32(wywy_wywy(hi128_lo128(v)));
    }

    const Float_32 x(const PackedFloat8_256 & v) noexcept {
        return component<VectorIndices::X>(v);
    }

    const Float_32 y(const PackedFloat8_256 & v) noexcept {
        return component<VectorIndices::Y>(v);
    }

    const Float_32 z(const PackedFloat8_256 & v) noexcept {
        return component<VectorIndices::Z>(v);
    }

    const Float_32 w(const PackedFloat8_256 & v) noexcept {
        return component<VectorIndices::W>(v);
    }

    const Float_32 x1(const PackedFloat8_256 & v) noexcept {
        return component<VectorIndices::X1>(v);
    }

    const Float_32 x2(const PackedFloat8_256 & v) noexcept {
        return component<VectorIndices::X2>(v);
    }

    const Float_32 x3(const PackedFloat8_256 & v) noexcept {
        return component<VectorIndices::X3>(v);
    }

    const Float_32 x4(const PackedFloat8_256 & v) noexcept {
        return component<VectorIndices::X4>(v);
    }

    const Float_32 x5(const PackedFloat8_256 & v) noexcept {
        return component<VectorIndices::X5>(v);
    }

    const Float_32 x6(const PackedFloat8_256 & v) noexcept {
        return component<VectorIndices::X6>(v);
    }

    const Float_32 x7(const PackedFloat8_256 & v) noexcept {
        return component<VectorIndices::X7>(v);
    }

    const Float_32 x8(const PackedFloat8_256 & v) noexcept {
        return component<VectorIndices::X8>(v);
    }
}