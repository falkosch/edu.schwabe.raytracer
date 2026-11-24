#pragma once

#include <vectorization.h>

namespace raytracer
{
    using namespace vectorization;

    // The denoiser's guides along a primary ray's path (Raytrace::features, atrousDenoise): the shading normal (towards
    // the viewer) and the diffuse albedo (sRGB-linear) of its first hit that isn't a smooth mirror or glass, whose
    // reflected or refracted image shows instead, and the length of the path up to that hit. A path that leaves the
    // scene at once keeps depth 0; one that leaves it behind a mirror or glass keeps that surface's guides.
    struct PixelFeatures
    {
        Float4 normal;
        Float4 albedo;
        Float depth;

        PixelFeatures();
    };
}
