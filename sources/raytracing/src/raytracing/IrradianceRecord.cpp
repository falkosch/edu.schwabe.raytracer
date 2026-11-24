#include "raytracing/IrradianceRecord.h"
#include "../stdafx.h"

namespace raytracer
{
    IrradianceRecord::IrradianceRecord()
        : position(Zero<Float4>()), normal(Zero<Float4>()), irradiance(spectral::Spectrum::zero()),
          radius(Zero<Float>()), maxRadius(std::numeric_limits<Float>::max()), context(Zero<ASizeT>())
    {
    }

    Float IrradianceRecord::validRadius() const noexcept
    {
        return min(radius, maxRadius);
    }
}
