#include "../stdafx.h"

#include "geometry/TransformInfo.h"

namespace raytracer
{

    TransformInfo::TransformInfo() :
        translation(),
        rotation(),
        scale(One<Float>())
    { }

}
