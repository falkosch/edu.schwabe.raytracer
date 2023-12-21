#include "raytracing/shading/Raytrace.h"
#include "../../stdafx.h"

namespace raytracer {
  Raytrace::Raytrace() : rayCast(), originIntersection(), traceDepth(), visibilityIndex() {
  }

  Raytrace::Raytrace(
      const RayCast &rayCastIn, const SceneIntersection *const originIntersectionIn, const ASizeT traceDepthIn,
      const Float visibilityIndexIn
  )
      : rayCast(rayCastIn), originIntersection(originIntersectionIn), traceDepth(traceDepthIn),
        visibilityIndex(visibilityIndexIn) {
  }
}