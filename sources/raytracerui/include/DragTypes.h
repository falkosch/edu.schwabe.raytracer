#pragma once

#include <vectorization.h>

namespace raytracerui
{
    using namespace vectorization;

    enum class DragTypes : ASizeT { None, Rotate, Scale, Light, Object };
}
