#pragma once

#include <primitives.h>
#include <raytracing/geometry/FacetIntersection.h>
#include <raytracing/geometry/forms/meshes/Mesh.h>

#include <array>

namespace benchmarks {
  using namespace vectorization;
  using namespace primitives;
  using namespace raytracer;

  void runIntersectionBenchmarks(const std::array<RayCast, 4> &rayCasts, ASizeT iterations);
}
