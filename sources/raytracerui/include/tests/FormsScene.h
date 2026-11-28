#pragma once

#include <raytracing.h>

namespace raytracerui {
  using namespace raytracer;

  // Every form type with the checker texture, so that intersections, normals and texture coordinates of each show:
  // analytic and mesh sphere side by side, an ellipsoid (non-uniformly scaled sphere), an upright and a tilted box,
  // a low-poly mesh and two planes (floor and back wall). Each object but the planes stands in a pair with a twin on
  // its right whose diffuse color is its surface normal (IntersectionNormalShader).
  struct FormsScene {
    static void setup(Scene &scene, Resources &resources);
  };
}
