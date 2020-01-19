#include "raytracing/geometry/TransformInfo.h"
#include "../../stdafx.h"

namespace raytracer
{
    TransformInfo::TransformInfo() :
        translation(),
        rotation(),
        scale(One<Float>()) { }
}