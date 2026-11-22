#include "raytracing/shading/shaders/HDRImageShader.h"
#include "../../../stdafx.h"

namespace raytracer
{
    HDRImageShader::HDRImageShader(const HDRImage& imageIn, const TextureWrap wrapIn) : image(&imageIn), wrap(wrapIn)
    {
    }

    HDRImageShader::~HDRImageShader() = default;

    Float4 HDRImageShader::sample(const ObjectShader&, const FacetIntersection& intersection) const
    {
        return image->sampleBilinear(intersection.texCoords, wrap);
    }
}
