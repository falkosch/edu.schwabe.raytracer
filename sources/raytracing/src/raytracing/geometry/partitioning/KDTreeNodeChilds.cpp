#include "raytracing/geometry/partitioning/KDTreeNodeChilds.h"
#include "../../../stdafx.h"

namespace raytracer
{
    KDTreeNodeChilds::KDTreeNodeChilds()
        :
        boundingA(),
        boundingB(),
        childA(),
        childB() { }

    KDTreeNodeChilds::~KDTreeNodeChilds() { }
}