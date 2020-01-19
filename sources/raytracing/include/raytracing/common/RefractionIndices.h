#pragma once

#include <vectorization.h>

namespace raytracer
{
    using namespace vectorization;

    struct RefractionIndices
    {
        static const Float Caesium;
        static const Float Vacuum;
        static const Float AirSTP;
        static const Float Aluminium;
        static const Float Tabaxir;
        static const Float Water;
        static const Float Glass;
        static const Float Diamond;

        static const Float4 refractionEta(const Float indexA, const Float indexB);
    };
}
