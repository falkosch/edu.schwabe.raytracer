#include "raytracing/shading/shaders/IntersectionNormalShader.h"
#include "../../../stdafx.h"

namespace raytracer
{
    IntersectionNormalShader::IntersectionNormalShader()
    {
    }

    IntersectionNormalShader::~IntersectionNormalShader() = default;

    Float4
    IntersectionNormalShader::sample(const ObjectShader&, const FacetIntersection& intersection) const
    {
        return oneW(multiplyAdd(intersection.surfaceNormal, Half<Float4>(), Half<Float4>()));
    }
}
