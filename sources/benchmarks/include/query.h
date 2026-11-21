#pragma once

#include <raytracing/geometry/FacetIntersection.h>
#include <raytracing/geometry/forms/meshes/Mesh.h>

namespace benchmarks
{
    using namespace vectorization;
    using namespace primitives;
    using namespace raytracer;

    enum class Query { Nearest, Any };

    const char* nameOf(Query query);

    // the mesh's nearest or any-hit query through its production entry point
    Float trace(Query query, const Mesh& mesh, const RayCast& rayCast, FacetIntersection& intersection);
}
