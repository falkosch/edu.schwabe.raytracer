#pragma once

#include <vectorization.h>

namespace raytracer
{
    using namespace vectorization;

    struct LightShading
    {
        Float4 ambient;

        Float4 diffuse;

        Float4 specular;

        Float4 reflected;

        Float4 transmitted;

        ALIGNED_ALLOCATORS(__alignof(LightShading));

        LightShading();

        LightShading(const Float4 & ambient);
    };
}
