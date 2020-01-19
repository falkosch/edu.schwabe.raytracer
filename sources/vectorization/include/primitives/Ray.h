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

        Ray() noexcept;

        explicit Ray(const Float4 & origin) noexcept;

        explicit Ray(const Float4 & origin, const Float4 & direction) noexcept;

        void setDirection(const Float4 & direction) noexcept;
    };

    const Ray replaceDirection(const Ray & r, const Float4 & value) noexcept;

    const Float4 point(const Ray & r, const Float t) noexcept;

    const Float4 point(const Ray & r, const Float4 & v) noexcept;
}
