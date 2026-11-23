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
        Float horizonRadius;
        TextureWrap wrap;

    public:
        // horizonRadius: where the horizon lies in the image (AllSkyFisheye only), half its width by default
        explicit EnvironmentShader(
            const HDRImage& image, EnvironmentProjection projection = EnvironmentProjection::FisheyeDome,
            Float horizonRadius = Half<Float>()
        );

        ~EnvironmentShader() override;

        spectral::Spectrum sample(const SceneShader& sceneShader, const BackgroundQuery& query) const override;
    };
}
