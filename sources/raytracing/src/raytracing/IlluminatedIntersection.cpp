#include "raytracing/IlluminatedIntersection.h"
#include "../stdafx.h"

namespace raytracer
{
    IlluminatedIntersection::IlluminatedIntersection(const Float depthIn, const spectral::Spectrum& color)
        : depth(depthIn), color(color)
    {
    }

    IlluminatedIntersection::IlluminatedIntersection(const Float4& depthIn, const spectral::Spectrum& color)
        : depth(depthIn), color(color)
    {
    }
}
