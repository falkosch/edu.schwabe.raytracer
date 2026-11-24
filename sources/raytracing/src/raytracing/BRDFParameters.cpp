#include "raytracing/BRDFParameters.h"
#include "../stdafx.h"

namespace raytracer {
  BRDFParameters::BRDFParameters()
      : intersection(), surface(), lighting(), transmissionDirection(), fresnel(), reflectanceCoefficient(),
        fractionTransmitted(), viewDistance() {
  }
}
