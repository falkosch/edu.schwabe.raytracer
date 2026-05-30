#pragma once

#include "../../common/HDRImage.h"
#include "../SceneShader.h"

namespace raytracer
{
    using namespace vectorization;

    class EnvironmentShader final : public Shader<SceneShader, BackgroundQuery, spectral::Spectrum>
    {
        Size2 resolution;
        Float4 alignedSize;
        const HDRImage* environmentImage;

    public:
        explicit EnvironmentShader(const HDRImage& image);

        ~EnvironmentShader() override;

        spectral::Spectrum sample(const SceneShader& sceneShader, const BackgroundQuery& query) const override;
    };
}
