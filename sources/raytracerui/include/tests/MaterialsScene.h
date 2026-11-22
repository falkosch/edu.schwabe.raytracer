#pragma once

#include <raytracing.h>

namespace raytracerui {
  using namespace raytracer;

  // A chart of surface materials and noise generators, one sphere each, in front of a checker backdrop:
  //   row 1: metal with roughness from 0 to 1 (roughness widens the highlights; the mirrored image stays sharp)
  //   row 2: every refraction index constant: caesium, air, aluminium, tabaxir, water, glass, diamond
  //   row 3: matte, rough and glossy plastic, gold, copper, absorbing tinted glass, an emitter
  //   row 4: Perlin and Simplex noise with 1, 4 and 8 octaves, and Simplex noise as the roughness of a metal
  struct MaterialsScene {
    static void setup(Scene &scene, Resources &resources);
  };
}
