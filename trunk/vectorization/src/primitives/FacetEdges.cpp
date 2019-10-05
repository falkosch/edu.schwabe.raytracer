#include "primitives/FacetEdges.h"

using namespace vectorization;

namespace primitives
{
    FacetEdges::FacetEdges() noexcept
        : v0(), v1() { }

    FacetEdges::FacetEdges(const Float4 & v0In, const Float4 & v1In) noexcept
        : v0(v0In), v1(v1In) { }
}