#include "vectorization/accessors/replaceComponent_256s.h"

#include "vectorization/blends.h"

#include <cassert>

namespace vectorization
{
    template <>
    const PackedFloat8_256 replaceComponent<VectorIndices::X1>(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        auto packs = _mm256_set1_ps(s);
        return blend<true, false, false, false, false, false, false, false>(v, packs);
    }

    template <>
    const PackedFloat8_256 replaceComponent<VectorIndices::X2>(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        auto packs = _mm256_set1_ps(s);
        return blend<false, true, false, false, false, false, false, false>(v, packs);
    }

    template <>
    const PackedFloat8_256 replaceComponent<VectorIndices::X3>(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        auto packs = _mm256_set1_ps(s);
        return blend<false, false, true, false, false, false, false, false>(v, packs);
    }

    template <>
    const PackedFloat8_256 replaceComponent<VectorIndices::X4>(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        auto packs = _mm256_set1_ps(s);
        return blend<false, false, false, true, false, false, false, false>(v, packs);
    }

    template <>
    const PackedFloat8_256 replaceComponent<VectorIndices::X5>(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        auto packs = _mm256_set1_ps(s);
        return blend<false, false, false, false, true, false, false, false>(v, packs);
    }

    template <>
    const PackedFloat8_256 replaceComponent<VectorIndices::X6>(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        auto packs = _mm256_set1_ps(s);
        return blend<false, false, false, false, false, true, false, false>(v, packs);
    }

    template <>
    const PackedFloat8_256 replaceComponent<VectorIndices::X7>(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        auto packs = _mm256_set1_ps(s);
        return blend<false, false, false, false, false, false, true, false>(v, packs);
    }

    template <>
    const PackedFloat8_256 replaceComponent<VectorIndices::X8>(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        auto packs = _mm256_set1_ps(s);
        return blend<false, false, false, false, false, false, false, true>(v, packs);
    }

    const PackedFloat8_256 replaceX(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        return replaceComponent<VectorIndices::X>(v, s);
    }

    const PackedFloat8_256 replaceY(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        return replaceComponent<VectorIndices::Y>(v, s);
    }

    const PackedFloat8_256 replaceZ(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        return replaceComponent<VectorIndices::Z>(v, s);
    }

    const PackedFloat8_256 replaceW(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        return replaceComponent<VectorIndices::W>(v, s);
    }

    const PackedFloat8_256 replaceX1(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        return replaceComponent<VectorIndices::X1>(v, s);
    }

    const PackedFloat8_256 replaceX2(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        return replaceComponent<VectorIndices::X2>(v, s);
    }

    const PackedFloat8_256 replaceX3(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        return replaceComponent<VectorIndices::X3>(v, s);
    }

    const PackedFloat8_256 replaceX4(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        return replaceComponent<VectorIndices::X4>(v, s);
    }

    const PackedFloat8_256 replaceX5(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        return replaceComponent<VectorIndices::X5>(v, s);
    }

    const PackedFloat8_256 replaceX6(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        return replaceComponent<VectorIndices::X6>(v, s);
    }

    const PackedFloat8_256 replaceX7(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        return replaceComponent<VectorIndices::X7>(v, s);
    }

    const PackedFloat8_256 replaceX8(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        return replaceComponent<VectorIndices::X8>(v, s);
    }

    const PackedFloat8_256 replaceComponent(const PackedFloat8_256 & v, const PackedFloat8_256 & replacement, const ASizeT index) noexcept {
        assert(index < VectorSizes::Y);
        auto indexBlendMask = _mm256_castsi256_ps(
            _mm256_cmpeq_epi32(
                _mm256_set1_epi32(static_cast<int>(index)),
                _mm256_set_epi32(7, 6, 5, 4, 3, 2, 1, 0)
            )
        );
        return _mm256_blendv_ps(v, replacement, indexBlendMask);
    }

    const PackedFloat8_256 replaceComponent(const PackedFloat8_256 & v, const Float_32 replacement, const ASizeT index) noexcept {
        return replaceComponent(v, _mm256_set1_ps(replacement), index);
    }
}