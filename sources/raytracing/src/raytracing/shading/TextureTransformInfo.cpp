#include "raytracing/shading/TextureTransformInfo.h"
#include "../../stdafx.h"

namespace raytracer
{
    TextureTransformInfo::TextureTransformInfo()
        :
        translation(),
        scale(One<Float>()),
        rotation() { }
}