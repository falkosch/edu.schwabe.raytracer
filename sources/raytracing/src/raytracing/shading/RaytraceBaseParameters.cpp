#include "raytracing/shading/RaytraceBaseParameters.h"
#include "../../stdafx.h"

namespace raytracer {
  RaytraceBaseParameters::RaytraceBaseParameters()
      : visibilityCutoff(), maxDistance(), maxTraceDepth(), sceneShader(), cullingOrientation() {
  }
}