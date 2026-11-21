#pragma once

#include "complex_ior.h"
#include "metal.h"

#include <vectorization.h>

namespace raytracer::spectral
{
    using namespace vectorization;

    ComplexIOR metalIOR(Metal id, const Float8& wavelengths) noexcept;
}
