#pragma once

#include "../ObjectShader.h"
#include "../../common/HDRImage.h"

namespace raytracer
{
    using namespace vectorization;

    class HDRImageShader : public Shader < ObjectShader, FacetIntersection, Float4 >
    {
        const HDRImage * image;

    public:

        HDRImageShader(const HDRImage & image);

        virtual ~HDRImageShader();

        const Float4 sample(const ObjectShader & objectShader, const FacetIntersection & intersection) const;

        const Float4 operator()(const ObjectShader & objectShader, const FacetIntersection & intersection) const;
    };
}
