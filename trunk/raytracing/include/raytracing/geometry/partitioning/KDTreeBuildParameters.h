#pragma once

#include <vectorization.h>

namespace raytracer
{
    using namespace vectorization;

    struct KDTreeBuildParameters
    {
        // x = global traversal costs for testing bounds
        // y = global intersection cost factorization
        // z, w reserved
        Float4 costParameters;

        // how many GeometryNodes are at most allowed per leaf
        ASizeT maxNodesSize;

        // How many levels deep is the recursion allowed to be.
        ASizeT maxTreeDepth;

        ALIGNED_ALLOCATORS(__alignof(KDTreeBuildParameters));

        KDTreeBuildParameters();
    };
}
