#pragma once

#include "../../common/HDRImage.h"
#include "../SceneShader.h"

namespace raytracer {
  using namespace vectorization;

  class EnvironmentShader : public Shader<SceneShader, Float4, Float4> {
    Size2 resolution;

    Float4 alignedSize;

    const HDRImage *environmentImage;

  public:
    EnvironmentShader(const HDRImage &image);

    virtual ~EnvironmentShader();

    const Float4 sample(const SceneShader &sceneShader, const Float4 &rayDirection) const;

    const Float4 operator()(const SceneShader &sceneShader, const Float4 &rayDirection) const;
  };
}
