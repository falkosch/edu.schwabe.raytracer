#pragma once

#include <vectorization.h>

namespace primitives
{
	using namespace vectorization;

    /*
    * A simple structure to contain a ray
    */
    struct Ray
    {
        Float4 origin, direction, reciprocalDirection;

        ALIGNED_ALLOCATORS(__alignof(Ray));

        Ray();

        explicit Ray(const Float4 & origin);

        explicit Ray(const Float4 & origin, const Float4 & direction);

        void setDirection(const Float4 & direction);

    };

    const Ray replaceDirection(const Ray & r, const Float4 & value);

    const Float4 point(const Ray & r, const Float t);

    const Float4 point(const Ray & r, const Float4 & v);

}
