#pragma once

#include "../../common/HDRImage.h"
#include "../ObjectShader.h"

namespace raytracer {
  using namespace vectorization;

  class HDRImageShader : public Shader<ObjectShader, FacetIntersection, Float4> {
    const HDRImage *image;
    TextureWrap wrap;

  public:
    HDRImageShader(const HDRImage &image, TextureWrap wrap = TextureWrap::Repeat);

    ~HDRImageShader() override;

    Float4 sample(const ObjectShader &objectShader, const FacetIntersection &intersection) const override;
  };
}
