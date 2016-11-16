#pragma once

#include <vectorization/vectorization.h>

namespace raytracer
{

    struct StatisticsCookie
    {
        ASizeT primaryRays, missedPrimaryRays;

        ASizeT secondaryRays, missedSecondaryRays;

        ASizeT shadowRays, missedShadowRays;

        ALIGNED_ALLOCATORS(__alignof(StatisticsCookie));

        StatisticsCookie();

        void merge(const StatisticsCookie &other);
    };

}
