#pragma once

namespace raytracer {
  // How a Box spreads the texture coordinates over its faces
  enum class BoxTextureMapping {
    // Each face spans the whole texture, upright as seen from outside: the side faces with +y up, the top face with -z
    // up and the bottom face with +z up (a cube map of six equal images)
    PerFace,

    // The direction from the center as on a sphere: seamless around the box, but distorted on the faces
    Spherical,
  };
}
