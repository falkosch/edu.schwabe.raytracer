#include "raytracing/geometry/partitioning/KDTreeWideRay.h"
#include "../../../stdafx.h"

namespace raytracer
{
    KDTreeWideRay::KDTreeWideRay(const Ray& ray) noexcept
        : originX(xxxx(ray.origin)), originY(yyyy(ray.origin)), originZ(zzzz(ray.origin)),
          reciprocalX(xxxx(ray.reciprocalDirection)), reciprocalY(yyyy(ray.reciprocalDirection)),
          reciprocalZ(zzzz(ray.reciprocalDirection))
    {
    }
}
