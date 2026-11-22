#pragma once

#include <raytracing.h>

namespace raytracerui {
  using namespace raytracer;

  // Every mesh in data/meshes in a grid, each scaled to the same size by its largest extent and turned upright, in
  // a light clay material in front of a dark backdrop. Loading all of them takes a few seconds.
  struct MeshesScene {
    static void setup(Scene &scene, Resources &resources);
  };
}
