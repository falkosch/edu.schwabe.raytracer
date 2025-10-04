#include "raytracing/BRDFParameters.h"
#include "../stdafx.h"

namespace raytracer {
  BRDFParameters::BRDFParameters()
      : intersection(), surface(), lighting(), transmissionDirection(), reflectanceCoefficient(One<Float4>()),
        absorptionCoefficient(), viewDistance() {
  }
}
