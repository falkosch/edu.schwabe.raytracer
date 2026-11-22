#pragma once

#include "EnvironmentProjection.h"

#include "../../common/HDRImage.h"
#include "../SceneShader.h"

namespace raytracer
{
    using namespace vectorization;

    // Background from an environment image in the given projection
    class EnvironmentShader final : public Shader<SceneShader, BackgroundQuery, spectral::Spectrum>
    {
        Size2 resolution;
        Float4 alignedSize;
        const HDRImage* environmentImage;
        EnvironmentProjection projection;
        TextureWrap wrap;

    public:
        explicit EnvironmentShader(
            const HDRImage& image, EnvironmentProjection projection = EnvironmentProjection::FisheyeDome
        );

        ~EnvironmentShader() override;

        spectral::Spectrum sample(const SceneShader& sceneShader, const BackgroundQuery& query) const override;
    };
}
