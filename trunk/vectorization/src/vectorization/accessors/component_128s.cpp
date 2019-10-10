#include "vectorization/accessors/component_128s.h"

#include "vectorization/swizzles.h"

namespace vectorization
{
    template <>
    const Float_32 component<VectorIndices::X>(const PackedFloat4_128 & v) noexcept {
        return _mm_cvtss_f32(v);
    }

    template <>
    const Float_32 component<VectorIndices::Y>(const PackedFloat4_128 & v) noexcept {
        return _mm_cvtss_f32(yyww(v));
    }

    template <>
    const Float_32 component<VectorIndices::Z>(const PackedFloat4_128 & v) noexcept {
        return _mm_cvtss_f32(zwzw(v));
    }

    template <>
    const Float_32 component<VectorIndices::W>(const PackedFloat4_128 & v) noexcept {
        return _mm_cvtss_f32(wwww(v));
    }

    const Float_32 x(const PackedFloat4_128 & v) noexcept {
        return component<VectorIndices::X>(v);
    }

    const Float_32 y(const PackedFloat4_128 & v) noexcept {
        return component<VectorIndices::Y>(v);
    }

    const Float_32 z(const PackedFloat4_128 & v) noexcept {
        return component<VectorIndices::Z>(v);
    }

    const Float_32 w(const PackedFloat4_128 & v) noexcept {
        return component<VectorIndices::W>(v);
    }

    const Float_32 x1(const PackedFloat4_128 & v) noexcept {
        return component<VectorIndices::X1>(v);
    }

    const Float_32 x2(const PackedFloat4_128 & v) noexcept {
        return component<VectorIndices::X2>(v);
    }

    const Float_32 x3(const PackedFloat4_128 & v) noexcept {
        return component<VectorIndices::X3>(v);
    }

    const Float_32 x4(const PackedFloat4_128 & v) noexcept {
        return component<VectorIndices::X4>(v);
    }
}