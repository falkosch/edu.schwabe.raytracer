#pragma once

#include <vectorization.h>

namespace raytracer::spectral
{
    using namespace vectorization;

    enum class Illuminant { D65, A };

    Float8 illuminantAt(Illuminant illuminant, const Float8& wavelengths) noexcept;
}
