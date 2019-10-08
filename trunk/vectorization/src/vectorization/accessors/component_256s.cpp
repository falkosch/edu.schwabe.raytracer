#include "vectorization/accessors/component_256s.h"

namespace vectorization
{
    template <>
    const Float_32 component<VectorIndices::X1>(const PackedFloat8_256 & v) noexcept {
        return _mm256_cvtss_f32(v);
    }

    template <>
    const Float_32 component<VectorIndices::X2>(const PackedFloat8_256 & v) noexcept {
        // TODO
        return _mm256_cvtss_f32(v);
    }

    template <>
    const Float_32 component<VectorIndices::X3>(const PackedFloat8_256 & v) noexcept {
        // TODO
        return _mm256_cvtss_f32(v);
    }

    template <>
    const Float_32 component<VectorIndices::X4>(const PackedFloat8_256 & v) noexcept {
        // TODO
        return _mm256_cvtss_f32(v);
    }

    template <>
    const Float_32 component<VectorIndices::X5>(const PackedFloat8_256 & v) noexcept {
        // TODO
        return _mm256_cvtss_f32(v);
    }

    template <>
    const Float_32 component<VectorIndices::X6>(const PackedFloat8_256 & v) noexcept {
        // TODO
        return _mm256_cvtss_f32(v);
    }

    template <>
    const Float_32 component<VectorIndices::X7>(const PackedFloat8_256 & v) noexcept {
        // TODO
        return _mm256_cvtss_f32(v);
    }

    template <>
    const Float_32 component<VectorIndices::X8>(const PackedFloat8_256 & v) noexcept {
        // TODO
        return _mm256_cvtss_f32(v);
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