#include "raytracing/shading/shaders/EnvironmentShader.h"
#include "../../../stdafx.h"

namespace raytracer
{
    EnvironmentShader::EnvironmentShader(const HDRImage & imageIn)
        :
        environmentImage(&imageIn) { }

    EnvironmentShader::~EnvironmentShader() { }

    const Float4 EnvironmentShader::sample(const SceneShader & sceneShader, const Float4 & rayDirection) const {
        return (*this)(sceneShader, rayDirection);
    }

    const Float4 EnvironmentShader::operator()(const SceneShader & /*sceneShader*/, const Float4 & rayDirection) const {
        const Float4 texCoords = Half<Float4>() * xz_xz(rayDirection, rayDirection) + Half<Float4>();
        return environmentImage->sampleBilinear(texCoords);
    }
}