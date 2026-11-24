#include "raytracing/shading/power_heuristic.h"
#include "../../stdafx.h"

namespace raytracer
{
    Float powerHeuristic(const Float samples, const Float pdf, const Float otherSamples, const Float otherPdf) noexcept
    {
        const auto weighted = samples * pdf;
        if (!(weighted > Zero<Float>()))
        {
            return Zero<Float>();
        }
        const auto ratio = otherSamples * otherPdf / weighted;
        return One<Float>() / (One<Float>() + sqr(ratio));
    }
}
