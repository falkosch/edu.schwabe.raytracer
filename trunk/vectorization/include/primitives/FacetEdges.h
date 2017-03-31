#pragma once

#include "../vectorization/vectorization.h"

namespace primitives
{

    struct FacetEdges
    {
        vectorization::Float4 v0, v1;

        ALIGNED_ALLOCATORS(__alignof(FacetEdges));

        FacetEdges();

        explicit FacetEdges(const vectorization::Float4 & v0, const vectorization::Float4 & v1);
    };

}
