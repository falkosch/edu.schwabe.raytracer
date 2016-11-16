#include "../stdafx.h"

#include "geometry/SceneIntersection.h"

namespace raytracer
{

    SceneIntersection::SceneIntersection()
        :
        FacetIntersection(),
        smoothedNdotI(),
        reflectedDirection(),
        object()
    { }

}
