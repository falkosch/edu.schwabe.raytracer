#include "raytracing/IlluminatedIntersection.h"
#include "../stdafx.h"

namespace raytracer
{
    IlluminatedIntersection::IlluminatedIntersection(const Float depthIn, const RGBS& colorIn)
        : depth(depthIn), color(colorIn)
    {
    }

    IlluminatedIntersection::IlluminatedIntersection(const Float4& depthIn, const RGBS& colorIn)
        : depth(depthIn), color(colorIn)
    {
    }
}