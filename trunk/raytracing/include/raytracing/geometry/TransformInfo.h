#pragma once

#include <vectorization.h>

namespace raytracer
{
    using namespace vectorization;

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
