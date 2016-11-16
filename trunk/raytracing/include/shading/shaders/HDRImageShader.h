#pragma once

#include "shading/ObjectShader.h"
#include "common/HDRImage.h"

namespace raytracer
{

    class HDRImageShader : public Shader < ObjectShader, FacetIntersection, Float4 >
    {
        const HDRImage* image;

    public:

        ALIGNED_ALLOCATORS(__alignof(HDRImageShader));

        HDRImageShader(const HDRImage & image);

        virtual ~HDRImageShader();

        const Float4 sample(const ObjectShader & objectShader, const FacetIntersection & intersection) const;

        const Float4 operator()(const ObjectShader & objectShader, const FacetIntersection & intersection) const;

    };

}
