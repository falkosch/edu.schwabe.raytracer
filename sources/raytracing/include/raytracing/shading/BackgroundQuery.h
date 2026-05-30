#pragma once

#include <vectorization.h>

namespace raytracer
{
    using namespace vectorization;

    struct BackgroundQuery
    {
        Float4 direction;
        Float heroLambda;
    };
}
