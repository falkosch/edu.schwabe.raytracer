#include "vectorization/accessors/replaceComponent_256d.h"

#include "vectorization/blends.h"

namespace vectorization
{
    template <>
    const PackedFloat4_256 replaceComponent<VectorIndices::X>(const PackedFloat4_256 & v, const Float_64 s) noexcept {
        auto packs = _mm256_set1_pd(s);
        return blend<true, false, false, false>(v, packs);
    }

    template <>
    const PackedFloat4_256 replaceComponent<VectorIndices::Y>(const PackedFloat4_256 & v, const Float_64 s) noexcept {
        auto packs = _mm256_set1_pd(s);
        return blend<false, true, false, false>(v, packs);
    }

    template <>
    const PackedFloat4_256 replaceComponent<VectorIndices::Z>(const PackedFloat4_256 & v, const Float_64 s) noexcept {
        auto packs = _mm256_set1_pd(s);
        return blend<false, false, true, false>(v, packs);
    }

    template <>
    const PackedFloat4_256 replaceComponent<VectorIndices::W>(const PackedFloat4_256 & v, const Float_64 s) noexcept {
        auto packs = _mm256_set1_pd(s);
        return blend<false, false, false, true>(v, packs);
    }

    const PackedFloat4_256 replaceX(const PackedFloat4_256 & v, const Float_64 s) noexcept {
        return replaceComponent<VectorIndices::X>(v, s);
    }

    const PackedFloat4_256 replaceY(const PackedFloat4_256 & v, const Float_64 s) noexcept {
        return replaceComponent<VectorIndices::Y>(v, s);
    }

    const PackedFloat4_256 replaceZ(const PackedFloat4_256 & v, const Float_64 s) noexcept {
        return replaceComponent<VectorIndices::Z>(v, s);
    }

    const PackedFloat4_256 replaceW(const PackedFloat4_256 & v, const Float_64 s) noexcept {
        return replaceComponent<VectorIndices::W>(v, s);
    }

    const PackedFloat4_256 replaceX1(const PackedFloat4_256 & v, const Float_64 s) noexcept {
        return replaceComponent<VectorIndices::X1>(v, s);
    }

    const PackedFloat4_256 replaceX2(const PackedFloat4_256 & v, const Float_64 s) noexcept {
        return replaceComponent<VectorIndices::X2>(v, s);
    }

    const PackedFloat4_256 replaceX3(const PackedFloat4_256 & v, const Float_64 s) noexcept {
        return replaceComponent<VectorIndices::X3>(v, s);
    }

    const PackedFloat4_256 replaceX4(const PackedFloat4_256 & v, const Float_64 s) noexcept {
        return replaceComponent<VectorIndices::X4>(v, s);
    }
}