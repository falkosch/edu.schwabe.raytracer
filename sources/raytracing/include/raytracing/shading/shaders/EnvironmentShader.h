#pragma once

#include "../../common/HDRImage.h"
#include "../SceneShader.h"

namespace raytracer {
  using namespace vectorization;

  class EnvironmentShader final : public Shader<SceneShader, Float4, RGBS> {
    Size2 resolution;
    Float4 alignedSize;
    const HDRImage *environmentImage;

  public:
    explicit EnvironmentShader(const HDRImage &image);

    ~EnvironmentShader() override;

    RGBS sample(const SceneShader &sceneShader, const Float4 &rayDirection) const override;

    RGBS operator()(const SceneShader &sceneShader, const Float4 &rayDirection) const override;
  };
}
