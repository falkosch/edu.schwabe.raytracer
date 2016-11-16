#pragma once

#include "common/noise/NoiseGenerator.h"

namespace raytracer
{

    class SimplexNoiseGenerator : public NoiseGenerator
    {
    public:

        ALIGNED_ALLOCATORS(__alignof(SimplexNoiseGenerator));

        SimplexNoiseGenerator(const unsigned int seed);

        virtual ~SimplexNoiseGenerator() { }

        const Float noise(const Float v) const;

        const Float noise(const Float4 & v) const;

        const Float noise2(const Float2 & v) const;

        const Float noise2(const Float4 & v) const;

        const Float noise3(const Float3 & v) const;

        const Float noise3(const Float4 & v) const;

        const Float noise4(const Float4 & v) const;

    protected:

        static const ASizeT N = 8, B = 256;

        Int simplexGrid[N];

        const Int simplex(const Int3 & ijk, const Int B) const;

        const Float K(const Int a, const Float3 & ijk, const Float3 & uvw, Float3 & A) const;

        const Int shuffle(const Int3 & ijk) const;
    };

}
