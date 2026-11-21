#include "raytracing/geometry/partitioning/KDTreeWideRay.h"
#include "../../../stdafx.h"

namespace raytracer
{
    KDTreeWideRay::KDTreeWideRay(const Ray& ray) noexcept
        : originX(KDTreeWideLanes{xxxx(ray.origin)}), originY(KDTreeWideLanes{yyyy(ray.origin)}),
          originZ(KDTreeWideLanes{zzzz(ray.origin)}), reciprocalX(KDTreeWideLanes{xxxx(ray.reciprocalDirection)}),
          reciprocalY(KDTreeWideLanes{yyyy(ray.reciprocalDirection)}),
          reciprocalZ(KDTreeWideLanes{zzzz(ray.reciprocalDirection)})
    {
    }
}
