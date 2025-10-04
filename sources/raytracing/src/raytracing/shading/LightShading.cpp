#include "raytracing/shading/LightShading.h"
#include "../../stdafx.h"

namespace raytracer {
  LightShading::LightShading() : ambient(), diffuse(), specular(), reflected(), transmitted() {
  }

  LightShading::LightShading(const Float4 &ambientIn)
      : ambient(ambientIn), diffuse(), specular(), reflected(), transmitted() {
  }
}