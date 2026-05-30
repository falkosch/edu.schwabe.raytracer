#include "raytracing/shading/spectral/metal_ior.h"

#include <vectorization.h>

namespace raytracer::spectral
{
    struct MetalConstants
    {
        Float n;
        Float k;
    };

    static MetalConstants getPlaceholderConstants(const Metal metal) noexcept
    {
        switch (metal)
        {
        case Metal::Cu: return {0.27f, 3.60f};
        case Metal::Au: return {0.18f, 3.00f};
        case Metal::Al: return {1.50f, 7.60f};
        case Metal::Fe: return {2.87f, 3.35f};
        case Metal::Ag: return {0.05f, 4.00f};
        }
        return {One<Float>(), 3.0f};
    }

    ComplexIOR metalIOR(const Metal metal, const Float8& wavelengths) noexcept
    {
        const auto constants = getPlaceholderConstants(metal);
        return ComplexIOR{Float8(constants.n), Float8(constants.k)};
    }
}
