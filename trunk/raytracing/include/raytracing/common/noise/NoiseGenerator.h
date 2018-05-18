#pragma once

#include <vectorization.h>

namespace raytracer
{
	using namespace vectorization;

    class NoiseGenerator
    {
    public:

        virtual ~NoiseGenerator() { }

        virtual const Float noise(const Float v) const = 0;

        virtual const Float noise(const Float4 & v) const = 0;

        virtual const Float noise2(const Float2 & v) const = 0;

        virtual const Float noise2(const Float4 & v) const = 0;

        virtual const Float noise3(const Float3 & v) const = 0;

        virtual const Float noise3(const Float4 & v) const = 0;

        virtual const Float noise4(const Float4 & v) const = 0;
    };

}
