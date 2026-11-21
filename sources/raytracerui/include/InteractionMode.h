#pragma once

#include <vectorization.h>

namespace raytracerui
{
    using namespace vectorization;

    enum class InteractionMode : ASizeT { Camera, Object, Light };
}
