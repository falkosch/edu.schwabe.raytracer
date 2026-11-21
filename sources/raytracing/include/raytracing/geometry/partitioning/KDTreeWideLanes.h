#pragma once

#include <vectorization.h>

namespace raytracer
{
    using namespace vectorization;

    // Lane vector of the wide view of the tree (KDTreeWideNode, KDTreeWideRay): one lane per child box. Its component
    // count sets the children per wide node.
    using KDTreeWideLanes = Float8;
}
