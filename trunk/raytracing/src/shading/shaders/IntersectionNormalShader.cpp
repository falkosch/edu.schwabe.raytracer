#include "../../stdafx.h"

#include "shading/shaders/IntersectionNormalShader.h"

namespace raytracer
{

    IntersectionNormalShader::IntersectionNormalShader() { }

    IntersectionNormalShader::~IntersectionNormalShader() { }

    const Float4 IntersectionNormalShader::sample(const ObjectShader & objectShader, const FacetIntersection & intersection) const
    {
        return (*this)(objectShader, intersection);
    }

    const Float4 IntersectionNormalShader::operator()(const ObjectShader & /*objectShader*/, const FacetIntersection & intersection) const
    {
        return oneW(intersection.surfaceNormal * Half<Float4>() + Half<Float4>());
    }

}
