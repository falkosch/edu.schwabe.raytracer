#pragma once

#include <vectorization.h>

namespace raytracer {
  using namespace vectorization;

  struct StatisticsCookie {
    // Thread-local pointer for contexts that cannot pass StatisticsCookie& through the call chain
    // (e.g., KD-tree traversers reached through the virtual Intersectable interface).
    static thread_local StatisticsCookie *current;

    ASizeT primaryRays, missedPrimaryRays;

    ASizeT secondaryRays, missedSecondaryRays;

    ASizeT shadowRays, missedShadowRays;
    ASizeT objectShadowRays, objectMissedShadowRays;

    ASizeT kdTreeNodesVisited;

    UInt_64 intersectionTicks;
    UInt_64 shadingTicks;

    StatisticsCookie();

    void merge(const StatisticsCookie &other);
  };
}
