#pragma once

#include <vectorization.h>

namespace raytracer
{
    using namespace vectorization;

    struct SpectralContext;

    struct BackgroundQuery
    {
        Float4 direction;
        const SpectralContext& spectralContext;
    };
}
