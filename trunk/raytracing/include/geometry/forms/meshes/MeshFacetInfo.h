#pragma once

#include <primitives/Facet.h>

namespace raytracer
{

    struct MeshFacetInfo
    {

        ASizeT index;

        Facet trianglePlanes;

        MeshFacetInfo();

        explicit MeshFacetInfo(const ASizeT index, const Facet & trianglePlanes);

    };

}
