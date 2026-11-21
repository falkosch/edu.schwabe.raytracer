#pragma once

#include <primitives.h>

#include <string>
#include <vector>

namespace benchmarks
{
    using namespace vectorization;
    using namespace primitives;

    inline constexpr ASizeT CoherentResolution{512};
    inline constexpr ASizeT IncoherentRayCount{CoherentResolution * CoherentResolution};

    struct RaySet
    {
        std::string name;
        std::vector<RayCast> rayCasts;
    };

    // Pinhole camera on the -z side of the mesh, aimed at a grid on the front face of the bounding box, so every ray
    // enters the root bounding box. Backface culling as for primary rays (CULLING_ORIENTATION = -1).
    RaySet createCoherentRays(const AxisAlignedBoundingBox& bounding);

    // Random origins inside the bounding box and uniformly distributed directions, without culling: a stand-in for
    // secondary and shadow rays. Fixed seed, so every run traces the same rays.
    RaySet createIncoherentRays(const AxisAlignedBoundingBox& bounding);
}
