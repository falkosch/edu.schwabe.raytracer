#include "raytracing/shading/spectral/conversion.h"

#include "raytracing/shading/spectral/cmf.h"
#include "raytracing/shading/spectral/wavelengths.h"
#include "raytracing/shading/color/primaries.h"

#include <vectorization.h>

namespace raytracer::spectral
{
    XYZContext makeXYZContext(const Float heroLambda) noexcept
    {
        const auto cmf = cmfAt(wavelengthsAt(heroLambda));
        return XYZContext{cmf, reciprocal(horizontalSum(cmf.yBar))};
    }

    Float4 spectrumToXYZ(const Spectrum& s, const XYZContext& ctx) noexcept
    {
        const auto X = horizontalSum(s.data * ctx.cmf.xBar) * ctx.rcpYBarSum;
        const auto Y = horizontalSum(s.data * ctx.cmf.yBar) * ctx.rcpYBarSum;
        const auto Z = horizontalSum(s.data * ctx.cmf.zBar) * ctx.rcpYBarSum;
        return Float4{X, Y, Z, Zero<Float>()};
    }

    RGBS xyzToDisplayRGB(const Float4& xyz) noexcept
    {
        return RGBS{color::XYZToSRGBLinear() * xyz};
    }
}
