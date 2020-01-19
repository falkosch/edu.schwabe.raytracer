#pragma once

#include <vectorization.h>

namespace raytracer
{
    using namespace vectorization;

    struct StatisticsCookie
    {
        ASizeT primaryRays, missedPrimaryRays;

        ASizeT secondaryRays, missedSecondaryRays;

        ASizeT shadowRays, missedShadowRays;
        ASizeT objectShadowRays, objectMissedShadowRays;

        StatisticsCookie();

        void merge(const StatisticsCookie & other);
    };
}
