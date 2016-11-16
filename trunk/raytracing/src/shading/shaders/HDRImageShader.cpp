#include "../../stdafx.h"

#include "shading/shaders/HDRImageShader.h"

namespace raytracer
{

    HDRImageShader::HDRImageShader(const HDRImage & imageIn)
        :
        image(&imageIn)
    { }

    HDRImageShader::~HDRImageShader() { }

    const Float4 HDRImageShader::sample(const ObjectShader & objectShader, const FacetIntersection & intersection) const
    {
        return (*this)(objectShader, intersection);
    }

    const Float4 HDRImageShader::operator()(const ObjectShader &, const FacetIntersection & intersection) const
    {
        return image->sampleBilinear(intersection.texCoords);
    }

}
