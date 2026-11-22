#pragma once

#include "../../common/noise/NoiseGenerator.h"
#include "../ObjectShader.h"

namespace raytracer
{
    using namespace vectorization;

    template <ASizeT OctavesCount>
    class NoiseGeneratorMap : public Shader<ObjectShader, FacetIntersection, Float4>
    {
        const NoiseGenerator* generator;

    public:
        NoiseGeneratorMap(const NoiseGenerator& generatorIn) : generator(&generatorIn)
        {
        }

        ~NoiseGeneratorMap() override = default;

        Float4 sample(const ObjectShader&, const FacetIntersection& intersection) const override
        {
            auto noise = Zero<Float4>();
            auto frequency = One<Float4>();
            auto weight = One<Float>();

            // Fractal noise: octave i at the frequency 2^i weighs 2^-i, so the coarsest octave dominates.
            // 3D noise: the w of the texture coordinates is the homogeneous 1
            for (auto i = Zero<ASizeT>(); i < OctavesCount; ++i)
            {
                noise += Float4(weight * vectorization::sin(generator->noise3(intersection.texCoords * frequency)));
                frequency += frequency;
                weight *= Half<Float>();
            }

            return replaceW(clamp(noise * Half<Float4>() + Half<Float4>(), Zero<Float4>(), One<Float4>()),
                            One<Float>());
        }
    };
}
