#include "raytracing/shading/shaders/HDRImageShader.h"
#include "../../../stdafx.h"

namespace raytracer {
  HDRImageShader::HDRImageShader(const HDRImage &imageIn) : image(&imageIn) {
  }

  HDRImageShader::~HDRImageShader() = default;

  Float4 HDRImageShader::sample(const ObjectShader &objectShader, const FacetIntersection &intersection) const {
    return (*this)(objectShader, intersection);
  }

  Float4 HDRImageShader::operator()(const ObjectShader &, const FacetIntersection &intersection) const {
    return image->sampleBilinear(intersection.texCoords);
  }
}
