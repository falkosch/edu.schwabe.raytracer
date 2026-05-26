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

        Float4 sample(const ObjectShader& objectShader, const FacetIntersection& intersection) const override
        {
            return (*this)(objectShader, intersection);
        }

        Float4 operator()(const ObjectShader& /*objectShader*/, const FacetIntersection& intersection) const override
        {
            auto noise = Zero<Float4>();
            auto frequency = One<Float4>();

            for (auto i = Zero<ASizeT>(); i < OctavesCount; ++i)
            {
                noise =
                    noise * Half<Float4>() + Float4(
                        vectorization::sin(generator->noise4(intersection.texCoords * frequency)));
                frequency += frequency;
            }

            return replaceW(clamp(noise * Half<Float4>() + Half<Float4>(), Zero<Float4>(), One<Float4>()),
                            One<Float>());
        }
    };
}
