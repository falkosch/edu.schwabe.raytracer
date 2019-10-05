#include "raytracing/common/StatisticsCookie.h"
#include "../../stdafx.h"

namespace raytracer
{
    StatisticsCookie::StatisticsCookie()
        :
        primaryRays(),
        missedPrimaryRays(),
        secondaryRays(),
        missedSecondaryRays(),
        shadowRays(),
        missedShadowRays() { }

    void StatisticsCookie::merge(const StatisticsCookie & other) {
        this->primaryRays += other.primaryRays;
        this->missedPrimaryRays += other.missedPrimaryRays;
        this->secondaryRays += other.secondaryRays;
        this->missedSecondaryRays += other.missedSecondaryRays;
        this->shadowRays += other.shadowRays;
        this->missedShadowRays += other.missedShadowRays;
    }
}