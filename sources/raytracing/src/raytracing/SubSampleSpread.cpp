#include "raytracing/SubSampleSpread.h"
#include "../stdafx.h"

namespace raytracer
{
    SubSampleSpread::SubSampleSpread() : count(), sum(), sumOfSquares()
    {
    }

    void SubSampleSpread::add(const Float4& rgb) noexcept
    {
        const auto value = displayValue(rgb);
        count += One<Float>();
        sum += value;
        sumOfSquares += value * value;
    }

    Float SubSampleSpread::error() const noexcept
    {
        if (!(count > One<Float>()))
        {
            return Zero<Float>();
        }
        const auto mean = sum / count;
        const auto variance = max(Zero<Float>(), (sumOfSquares - sum * mean) / (count - One<Float>()));
        return vectorization::sqrt(variance / count);
    }

    Float SubSampleSpread::displayValue(const Float4& rgb) noexcept
    {
        const auto luminance = max(Zero<Float>(), 0.2126f * x(rgb) + 0.7152f * y(rgb) + 0.0722f * z(rgb));
        return vectorization::pow(luminance / (One<Float>() + luminance), 1.0f / 2.2f);
    }
}
