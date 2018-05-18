#pragma once

#include "AxisAlignedBoundingBox.h"

namespace primitives
{
	using namespace vectorization;

    struct Facet
    {
        Float4 v0, v1, v2;

        ALIGNED_ALLOCATORS(__alignof(Facet));

        Facet();

        explicit Facet(const Float4 & v);

        explicit Facet(const Float4 & v0, const Float4 & v1, const Float4 & v2);
    };

    typedef Facet FacetTexCoords4, FacetNormals;

	const AxisAlignedBoundingBox bounding(const Facet & facet);

    const Facet operator+(const Facet & a, const Float4 & b);

    const Facet operator-(const Facet & a, const Float4 & b);

}
