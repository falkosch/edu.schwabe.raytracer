#pragma once

#include <raytracing.h>

namespace raytracerui {
  using namespace raytracer;

  // The shader implementations and texture transforms: an environment map as background and in a mirror sphere
  // (EnvironmentShader), the earth texture loaded as sRGB and as linear (HDRImageShader), normals as colors on a
  // teapot (IntersectionNormalShader), a constant color (ConstShader), marble times blue next to plain marble
  // (MultiplyByConstShader), and scaled, rotated and translated texture coordinates on the floor and a box.
  struct ShadingScene {
    static void setup(Scene &scene, Resources &resources);
  };
}
