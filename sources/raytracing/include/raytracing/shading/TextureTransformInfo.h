#pragma once

#include <vectorization.h>

namespace raytracer {
  using namespace vectorization;

  /*
   * Texture-coords transformations
   */
  struct TextureTransformInfo {
    Float2 translation;
    Float2 scale;
    Float rotation;

    TextureTransformInfo();
  };
}
