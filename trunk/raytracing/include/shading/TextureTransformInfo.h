#pragma once

#include <vectorization/vectorization.h>

namespace raytracer
{

    /*
    * Texture-coords transformations
    */
    struct TextureTransformInfo
    {
        Float2 translation;
        Float2 scale;
        Float rotation;

        ALIGNED_ALLOCATORS(__alignof(TextureTransformInfo));

        TextureTransformInfo();
    };

}
