#pragma once

namespace raytracer
{
    template <typename ContainmentType, typename IntersectionType, typename OutputType>
    class Shader
    {
    public:

        virtual ~Shader() { }

        virtual const OutputType sample(const ContainmentType & containment, const IntersectionType & intersection) const = 0;

        const OutputType operator()(const ContainmentType & containment, const IntersectionType & intersection) const {
            return sample(containment, intersection);
        }
    };
}
