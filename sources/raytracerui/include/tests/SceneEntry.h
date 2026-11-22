#pragma once

#include <raytracing.h>

namespace raytracerui {
  using namespace raytracer;

  // A selectable scene: its command-line id (--scene=<id>), its label in the scene dropdown and its setup function.
  struct SceneEntry {
    const char *id;
    const char *label;
    void (*setup)(Scene &scene, Resources &resources);
  };
}
