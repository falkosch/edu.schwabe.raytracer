#include "vectorization/accessors/component_256d.h"

namespace vectorization
{
    template <>
    const Float_64 component<VectorIndices::X>(const PackedFloat4_256 & v) noexcept {
        return _mm256_cvtsd_f64(v);
    }

    template <>
    const Float_64 component<VectorIndices::Y>(const PackedFloat4_256 & v) noexcept {
        return _mm256_cvtsd_f64(_mm256_permute_pd(v, 0b0001));
    }

    template <>
    const Float_64 component<VectorIndices::Z>(const PackedFloat4_256 & v) noexcept {
        auto zwzw = _mm256_permute2f128_pd(v, v, 0b00010001);
        return _mm256_cvtsd_f64(zwzw);
    }

    template <>
    const Float_64 component<VectorIndices::W>(const PackedFloat4_256 & v) noexcept {
        auto zwzw = _mm256_permute2f128_pd(v, v, 0b00010001);
        return _mm256_cvtsd_f64(_mm256_permute_pd(zwzw, 0b0001));
    }

    const Float_64 x(const PackedFloat4_256 & v) noexcept {
        return component<VectorIndices::X>(v);
    }

    const Float_64 y(const PackedFloat4_256 & v) noexcept {
        return component<VectorIndices::Y>(v);
    }

    const Float_64 z(const PackedFloat4_256 & v) noexcept {
        return component<VectorIndices::Z>(v);
    }

    const Float_64 w(const PackedFloat4_256 & v) noexcept {
        return component<VectorIndices::W>(v);
    }

    const Float_64 x1(const PackedFloat4_256 & v) noexcept {
        return component<VectorIndices::X1>(v);
    }

    const Float_64 x2(const PackedFloat4_256 & v) noexcept {
        return component<VectorIndices::X2>(v);
    }

    const Float_64 x3(const PackedFloat4_256 & v) noexcept {
        return component<VectorIndices::X3>(v);
    }

    const Float_64 x4(const PackedFloat4_256 & v) noexcept {
        return component<VectorIndices::X4>(v);
    }
}