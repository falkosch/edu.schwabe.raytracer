#pragma once

#include <vectorization.h>

namespace primitives
{
	using namespace vectorization;

    struct FacetEdges
    {
        Float4 v0, v1;

        ALIGNED_ALLOCATORS(__alignof(FacetEdges));

        FacetEdges() noexcept;

        explicit FacetEdges(const Float4 & v0, const Float4 & v1) noexcept;
    };

}
