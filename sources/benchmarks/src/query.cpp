#include "query.h"

namespace benchmarks
{
    const char* nameOf(const Query query)
    {
        return query == Query::Nearest ? "nearest" : "any";
    }

    Float trace(const Query query, const Mesh& mesh, const RayCast& rayCast, FacetIntersection& intersection)
    {
        return query == Query::Nearest
                   ? mesh.findNearestIntersection(rayCast, nullptr, intersection)
                   : mesh.findAnyIntersection(rayCast, nullptr, intersection);
    }
}
