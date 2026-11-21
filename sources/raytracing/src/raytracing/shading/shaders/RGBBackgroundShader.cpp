#include "raytracing/shading/shaders/RGBBackgroundShader.h"
#include "../../../stdafx.h"

#include "raytracing/shading/SpectralContext.h"

namespace raytracer
{
    RGBBackgroundShader::RGBBackgroundShader(const RGBS& c) : color(c)
    {
    }

    RGBBackgroundShader::~RGBBackgroundShader() = default;

    spectral::Spectrum RGBBackgroundShader::sample(const SceneShader&, const BackgroundQuery& query) const
    {
        return query.spectralContext.fromRGB(color.value);
    }
}
