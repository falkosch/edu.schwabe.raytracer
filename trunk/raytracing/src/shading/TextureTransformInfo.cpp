#include "../stdafx.h"

#include "shading/TextureTransformInfo.h"

namespace raytracer
{

    TextureTransformInfo::TextureTransformInfo()
        :
        translation(),
        scale(One<Float>()),
        rotation()
    { }

}
