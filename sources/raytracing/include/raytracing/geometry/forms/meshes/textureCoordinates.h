#pragma once

#include <vectorization.h>
#include <primitives.h>

#include <vector>

namespace raytracer
{
    void computeTexCoordsOrtho(
        const std::vector<vectorization::UInt3> & facetIndices,
        const std::vector<primitives::Facet> & facets,
        const vectorization::Float4 & sPlane,
        const vectorization::Float4 & tPlane,
        std::vector<primitives::FacetTexCoords4> & texCoords
    ) noexcept;

    void computeTexCoordsSpherical(
        const std::vector<vectorization::UInt3> & facetIndices,
        const std::vector<primitives::Facet> & facets,
        std::vector<primitives::FacetTexCoords4> & texCoords
    ) noexcept;
}
