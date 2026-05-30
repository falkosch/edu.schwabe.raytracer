#pragma once

#include <vectorization.h>

namespace raytracer::spectral
{
    using namespace vectorization;

    enum class Metal { Cu, Au, Al, Fe, Ag };

    struct ComplexIOR
    {
        Float8 n;
        Float8 k;
    };

    ComplexIOR metalIOR(Metal id, const Float8& wavelengths) noexcept;
}
