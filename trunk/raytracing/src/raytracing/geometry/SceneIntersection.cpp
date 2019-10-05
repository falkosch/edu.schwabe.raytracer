#include "raytracing/geometry/SceneIntersection.h"
#include "../../stdafx.h"

namespace raytracer
{
    SceneIntersection::SceneIntersection()
        :
        FacetIntersection(),
        smoothedNdotI(),
        reflectedDirection(),
        object() { }
}