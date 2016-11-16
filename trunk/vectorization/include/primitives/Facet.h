#pragma once

#include "../vectorization/vectorization.h"

namespace primitives
{

    struct Facet
    {
        vectorization::Float4 v0, v1, v2;

        ALIGNED_ALLOCATORS(__alignof(Facet));

        Facet();

        explicit Facet(const vectorization::Float4 & v0, const vectorization::Float4 & v1, const vectorization::Float4 & v2);
    };

    typedef Facet FacetTexCoords4, FacetNormals;

    const Facet operator+(const Facet & a, const vectorization::Float4 & b);

    const Facet operator-(const Facet & a, const vectorization::Float4 & b);

}
