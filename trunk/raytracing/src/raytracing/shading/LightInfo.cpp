#include "raytracing/shading/LightInfo.h"
#include "../../stdafx.h"

namespace raytracer
{

    LightInfo::LightInfo() :
        position(OneW<Float4>()),
        emissivity(),
        glare(Two<Float4>()),
        attenuationFactors(One<Float>(), Zero<Float>(), Zero<Float>(), One<Float>())
    { }

}
