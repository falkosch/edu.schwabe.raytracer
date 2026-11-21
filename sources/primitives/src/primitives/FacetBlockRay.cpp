#include "primitives/FacetBlockRay.h"

using namespace vectorization;

namespace primitives {
  // a Float4 broadcast fills both halves of a Float8
  FacetBlockRay::FacetBlockRay(const Ray &ray) noexcept
      : origin{
            FacetLanes{xxxx(ray.origin)}, FacetLanes{yyyy(ray.origin)}, FacetLanes{zzzz(ray.origin)},
            FacetLanes{wwww(ray.origin)}
        },
        direction{
            FacetLanes{xxxx(ray.direction)}, FacetLanes{yyyy(ray.direction)}, FacetLanes{zzzz(ray.direction)},
            FacetLanes{wwww(ray.direction)}
        } {
  }
}
