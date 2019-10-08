#include "vectorization/accessors/replaceComponent_256s.h"

namespace vectorization
{
    template <>
    const PackedFloat8_256 replaceComponent<VectorIndices::X1>(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        // TODO
        return _mm256_setzero_ps();
    }

    template <>
    const PackedFloat8_256 replaceComponent<VectorIndices::X2>(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        // TODO
        return _mm256_setzero_ps();
    }

    template <>
    const PackedFloat8_256 replaceComponent<VectorIndices::X3>(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        // TODO
        return _mm256_setzero_ps();
    }

    template <>
    const PackedFloat8_256 replaceComponent<VectorIndices::X4>(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        // TODO
        return _mm256_setzero_ps();
    }

    template <>
    const PackedFloat8_256 replaceComponent<VectorIndices::X5>(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        // TODO
        return _mm256_setzero_ps();
    }

    template <>
    const PackedFloat8_256 replaceComponent<VectorIndices::X6>(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        // TODO
        return _mm256_setzero_ps();
    }

    template <>
    const PackedFloat8_256 replaceComponent<VectorIndices::X7>(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        // TODO
        return _mm256_setzero_ps();
    }

    template <>
    const PackedFloat8_256 replaceComponent<VectorIndices::X8>(const PackedFloat8_256 & v, const Float_32 s) noexcept {
        // TODO
        return _mm256_setzero_ps();
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
}