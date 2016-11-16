#pragma once

#include <vectorization/vectorization.h>

namespace raytracer
{

    /*
    * Model or view transformations
    */
    struct TransformInfo
    {
        Float3 translation;
        Float3 rotation;
        Float3 scale;

        ALIGNED_ALLOCATORS(__alignof(TransformInfo));

        TransformInfo();
    };

}
