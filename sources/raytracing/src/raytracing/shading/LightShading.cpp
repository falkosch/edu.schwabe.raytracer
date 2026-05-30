#include "raytracing/shading/LightShading.h"
#include "../../stdafx.h"

namespace raytracer
{
    LightShading::LightShading() : ambient(), diffuse(), specular(), reflected(), transmitted()
    {
    }

    LightShading::LightShading(const spectral::Spectrum& ambientIn)
        : ambient(ambientIn), diffuse(), specular(), reflected(), transmitted()
    {
    }
}
