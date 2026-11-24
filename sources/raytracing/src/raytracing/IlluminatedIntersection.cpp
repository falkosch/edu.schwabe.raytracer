#include "raytracing/IlluminatedIntersection.h"
#include "../stdafx.h"

namespace raytracer
{
    IlluminatedIntersection::IlluminatedIntersection(const Float depthIn, const spectral::Spectrum& color)
        : depth(depthIn), color(color), emitterLight()
    {
    }

    IlluminatedIntersection::IlluminatedIntersection(const Float4& depthIn, const spectral::Spectrum& color)
        : depth(depthIn), color(color), emitterLight()
    {
    }

    IlluminatedIntersection::IlluminatedIntersection(
        const Float depthIn, const spectral::Spectrum& color, const LightInfo* const emitterLightIn
    )
        : depth(depthIn), color(color), emitterLight(emitterLightIn)
    {
    }
}
