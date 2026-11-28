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
        // Contrast per the generator's deviation and the octaves' summed deviation
        Float scale;

    public:
        // Octave i at the frequency 2^i weighs Gain^i: above 1/2, so that the finer octaves add visible detail
        static constexpr Float Gain = static_cast<Float>(0.6);
        // The grey the summed octaves' deviation adds to the middle grey 1/2: two deviations reach 0 and 1
        static constexpr Float Contrast = static_cast<Float>(0.25);

        NoiseGeneratorMap(const NoiseGenerator& generatorIn) : generator(&generatorIn), scale()
        {
            // The octaves are nearly independent: their sum deviates by the root of the summed squared weights
            auto squaredWeights = Zero<Float>();
            auto weight = One<Float>();
            for (auto i = Zero<ASizeT>(); i < OctavesCount; ++i)
            {
                squaredWeights += weight * weight;
                weight *= Gain;
            }
            scale = Contrast * rsqrt(squaredWeights) / generator->deviation();
        }

        ~NoiseGeneratorMap() override = default;

        Float4 sample(const ObjectShader&, const FacetIntersection& intersection) const override
        {
            auto noise = Zero<Float>();
            auto frequency = One<Float4>();
            auto weight = One<Float>();

            // Fractal noise, each octave at twice the frequency of the one before.
            // 3D noise: the w of the texture coordinates is the homogeneous 1
            for (auto i = Zero<ASizeT>(); i < OctavesCount; ++i)
            {
                noise += weight * generator->noise3(intersection.texCoords * frequency);
                frequency += frequency;
                weight *= Gain;
            }

            // the same contrast for every generator and octave count
            const auto grey = clamp(noise * scale + Half<Float>(), Zero<Float>(), One<Float>());
            return replaceW(Float4(grey), One<Float>());
        }
    };
}
