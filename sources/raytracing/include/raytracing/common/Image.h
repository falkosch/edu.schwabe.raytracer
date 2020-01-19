#pragma once

#include <vectorization.h>

namespace raytracer
{
    using namespace vectorization;

    template <typename ImageElementType>
    class Image
    {
    public:

        virtual ~Image() { }

        virtual ImageElementType & operator[](const ASizeT index) = 0;

        virtual const ImageElementType & operator[](const ASizeT index) const = 0;

        virtual const Size2 getResolution() const = 0;
    };
}
