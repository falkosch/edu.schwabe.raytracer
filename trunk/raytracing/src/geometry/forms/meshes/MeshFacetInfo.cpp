#include "../../../stdafx.h"

#include "geometry/forms/meshes/MeshFacetInfo.h"

namespace raytracer
{

    MeshFacetInfo::MeshFacetInfo()
        :
        index(),
        trianglePlanes()
    { }

    MeshFacetInfo::MeshFacetInfo(const ASizeT index, const Facet & trianglePlanes)
        :
        index(index),
        trianglePlanes(trianglePlanes)
    { }

}