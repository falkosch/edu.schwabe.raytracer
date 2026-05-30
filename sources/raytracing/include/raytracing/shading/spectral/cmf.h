#pragma once

#include <vectorization.h>

namespace raytracer::spectral
{
    using namespace vectorization;

    struct CmfSample
    {
        Float8 xBar;
        Float8 yBar;
        Float8 zBar;
    };

    CmfSample cmfAt(const Float8& wavelengths) noexcept;
}
