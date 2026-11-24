#include "raytracing/PixelFeatures.h"
#include "../stdafx.h"

namespace raytracer
{
    PixelFeatures::PixelFeatures() : normal(Zero<Float4>()), albedo(Zero<Float4>()), depth(Zero<Float>())
    {
    }
}
