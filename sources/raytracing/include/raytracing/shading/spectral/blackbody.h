#pragma once

#include <vectorization.h>

namespace raytracer::spectral
{
    using namespace vectorization;

    Float8 blackbody(Float temperatureKelvin, const Float8& wavelengths) noexcept;
}
