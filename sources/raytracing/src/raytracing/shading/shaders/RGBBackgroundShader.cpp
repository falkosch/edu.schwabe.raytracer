#include "raytracing/shading/shaders/RGBBackgroundShader.h"
#include "../../../stdafx.h"

#include "raytracing/shading/spectral/spectrum.h"

namespace raytracer
{
    RGBBackgroundShader::RGBBackgroundShader(const RGBS& c) : color(c)
    {
    }

    RGBBackgroundShader::~RGBBackgroundShader() = default;

    spectral::Spectrum RGBBackgroundShader::sample(const SceneShader&, const BackgroundQuery& query) const
    {
        return spectral::fromRGBS(query.heroLambda, color);
    }
}
