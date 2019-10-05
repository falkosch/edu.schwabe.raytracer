#include "ManualTests.h"
#include "stdafx.h"

#include <iostream>

namespace raytracerui
{
    ARCH_NOINLINE void collisionTest() {
        auto v0 = Float4(-0.156548679f, 0.286460012f, -0.441803336f, 1.00000000f);
        auto v1 = Float4(-0.250403672f, 0.286460012f, -0.443586677f, 1.00000000f);
        auto v2 = Float4(-0.240178674f, 0.286460012f, -0.417760015f, 1.00000000f);
        auto facet = Facet(v0, v1, v2);
        auto n = new MeshGeometryNode(0, facet);

        auto min = Float4(-0.199075446f, 0.254602253f, -0.460181773f, 1.00000000f);
        auto max = Float4(-0.159260362f, 0.286460012f, -0.437499464f, 1.00000000f);
        auto b = AxisAlignedBoundingBox(min, max);

        n->overlaps(b);

        delete n;
    }

    void ManualTests::operator()() const {
        collisionTest();
    }
}