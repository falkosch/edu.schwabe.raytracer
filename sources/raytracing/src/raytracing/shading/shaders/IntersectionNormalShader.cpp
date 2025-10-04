#include "raytracing/shading/shaders/IntersectionNormalShader.h"
#include "../../../stdafx.h"

namespace raytracer {
  IntersectionNormalShader::IntersectionNormalShader() {
  }

  IntersectionNormalShader::~IntersectionNormalShader() = default;

  Float4
  IntersectionNormalShader::sample(const ObjectShader &objectShader, const FacetIntersection &intersection) const {
    return (*this)(objectShader, intersection);
  }

  Float4 IntersectionNormalShader::operator()(
      const ObjectShader & /*objectShader*/, const FacetIntersection &intersection
  ) const {
    return oneW(intersection.surfaceNormal * Half<Float4>() + Half<Float4>());
  }
}
