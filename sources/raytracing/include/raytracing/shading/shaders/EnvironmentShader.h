#pragma once

#include "../../common/HDRImage.h"
#include "../SceneShader.h"

namespace raytracer {
  using namespace vectorization;

  class EnvironmentShader final : public Shader<SceneShader, Float4, Float4> {
    Size2 resolution;
    Float4 alignedSize;
    const HDRImage *environmentImage;

  public:
    explicit EnvironmentShader(const HDRImage &image);

    ~EnvironmentShader() override;

    Float4 sample(const SceneShader &sceneShader, const Float4 &rayDirection) const override;

    Float4 operator()(const SceneShader &sceneShader, const Float4 &rayDirection) const override;
  };
}
