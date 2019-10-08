#include "vectorization/accessors/component_128d.h"

#include "vectorization/swizzles/swizzle_128d.h"

namespace vectorization
{
    template <>
    const Float_64 component<VectorIndices::X>(const PackedFloat2_128 & v) noexcept {
        return _mm_cvtsd_f64(v);
    }

    template <>
    const Float_64 component<VectorIndices::Y>(const PackedFloat2_128 & v) noexcept {
        return _mm_cvtsd_f64(yy(v));
    }

    const Float_64 x(const PackedFloat2_128 & v) noexcept {
        return component<VectorIndices::X>(v);
    }

    const Float_64 y(const PackedFloat2_128 & v) noexcept {
        return component<VectorIndices::Y>(v);
    }

    const Float_64 x1(const PackedFloat2_128 & v) noexcept {
        return component<VectorIndices::X1>(v);
    }

    const Float_64 x2(const PackedFloat2_128 & v) noexcept {
        return component<VectorIndices::X2>(v);
    }
}