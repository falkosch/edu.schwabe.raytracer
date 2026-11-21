#pragma once

#include "KDTreeLink.h"

namespace raytracer
{
    // The links to both children of an inner node.
    struct KDTreeChildLinks
    {
        KDTreeLink childA, childB;
    };
}
