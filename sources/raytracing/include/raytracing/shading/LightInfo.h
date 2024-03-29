#pragma once

#include "../geometry/ObjectGeometry.h"

namespace raytracer {
  using namespace vectorization;

  // Lighting info for emission.
  struct LightInfo {
    Float4 position;

    Float4 emittance;

    Float4 glare;

    // describes the fall-off of the lighting intensity over distance: x
    // is the constant, y is the linear and z is the quadratic factor in
    // [(L/|L|) dot N] / (x + y * |L| + z * |L| * |L|), where L is the
    // vector between the light-position and the facet's
    // intersection-point.
    Float4 attenuationFactors;

    LightInfo();
  };
}
