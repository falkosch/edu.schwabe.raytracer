#pragma once

#include "../SceneShader.h"

namespace raytracer
{
    class RGBBackgroundShader final : public SceneShader::BackgroundShader
    {
        RGBS color;

    public:
        explicit RGBBackgroundShader(const RGBS& c);

        ~RGBBackgroundShader() override;

        spectral::Spectrum sample(const SceneShader& sceneShader, const BackgroundQuery& query) const override;
    };
}
