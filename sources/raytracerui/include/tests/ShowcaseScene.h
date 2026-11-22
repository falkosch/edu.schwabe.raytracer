#pragma once

#include <raytracing.h>

namespace raytracerui {
  using namespace raytracer;

  // A studio demo of what the raytracer does: an amber glass dragon, a gold bunny, a chrome sphere on a marble
  // pedestal and a diamond on a black lacquer floor that reflects them, in front of a backdrop with a cool glow.
  // A warm key light and a cool rim light; spectral blackbody lights throughout.
  struct ShowcaseScene {
    static void setup(Scene &scene, Resources &resources);
  };
}
