#include "primitives/FacetEdges.h"

using namespace vectorization;

namespace primitives
{

    FacetEdges::FacetEdges()
        :
        v0(), v1()
    { }

    FacetEdges::FacetEdges(const Float4 & v0In, const Float4 & v1In)
        :
        v0(v0In), v1(v1In)
    { }

}
