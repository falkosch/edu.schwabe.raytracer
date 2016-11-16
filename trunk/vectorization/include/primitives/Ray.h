#pragma once

#include "../vectorization/vectorization.h"

namespace primitives
{

    /*
    * A simple structure to contain a ray
    */
    struct Ray
    {
        vectorization::Float4 origin, direction, reciprocalDirection;

        ALIGNED_ALLOCATORS(__alignof(Ray));

        Ray();

        explicit Ray(const vectorization::Float4 & origin);

        explicit Ray(const vectorization::Float4 & origin, const vectorization::Float4 & direction);

        void setDirection(const vectorization::Float4 & direction);

    };

    const Ray replaceDirection(const Ray & r, const vectorization::Float4 & value);

    const vectorization::Float4 point(const Ray & r, const vectorization::Float t);

    const vectorization::Float4 point(const Ray & r, const vectorization::Float4 & v);

}
