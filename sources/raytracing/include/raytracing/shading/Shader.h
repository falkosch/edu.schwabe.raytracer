#pragma once

namespace raytracer
{
    template <typename ContainmentType, typename IntersectionType, typename OutputType>
    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual OutputType sample(const ContainmentType& containment, const IntersectionType& intersection) const = 0;
    };
}
