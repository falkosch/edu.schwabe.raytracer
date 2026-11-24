#pragma once

#include <raytracing.h>

namespace raytracerui {
  using namespace raytracer;

  // Light sources on white surfaces: a row of light pools on the back wall for every spectral emitter kind
  // (blackbody at 1900 K, 4000 K and 10000 K, CIE illuminant A, D65) with quadratic falloff, three RGB lights with
  // linear falloff casting colored shadows of a white sphere, and an emissive sphere that shows that
  // emittance doesn't light its surroundings.
  struct LightingScene {
    static void setup(Scene &scene, Resources &resources);
  };
}
