#include "raytracing/shading/shaders/EnvironmentShader.h"
#include "../../../stdafx.h"

#include "raytracing/shading/spectral/spectrum.h"

namespace raytracer
{
    EnvironmentShader::EnvironmentShader(const HDRImage& imageIn) : environmentImage(&imageIn)
    {
    }

    EnvironmentShader::~EnvironmentShader() = default;

    spectral::Spectrum EnvironmentShader::sample(const SceneShader&, const BackgroundQuery& query) const
    {
        const Float4 texCoords = multiplyAdd(Half<Float4>(), xz_xz(query.direction, query.direction), Half<Float4>());
        const Float4 rgbPixel = environmentImage->sampleBilinear(texCoords);
        return spectral::fromRGB(query.heroLambda, rgbPixel);
    }
}
