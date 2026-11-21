#pragma once

#include <vectorization.h>

namespace raytracer::spectral
{
    using namespace vectorization;

    struct ComplexIOR
    {
        Float8 n;
        Float8 k;
    };
}
