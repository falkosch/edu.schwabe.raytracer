#include "../stdafx.h"

#include "shading/LightShading.h"

namespace raytracer
{

    LightShading::LightShading()
        :
        ambient(),
        diffuse(),
        specular(),
        reflected(),
        transmitted()
    { }

    LightShading::LightShading(const Float4 & ambientIn)
        :
        ambient(ambientIn),
        diffuse(),
        specular(),
        reflected(),
        transmitted()
    { }

}
