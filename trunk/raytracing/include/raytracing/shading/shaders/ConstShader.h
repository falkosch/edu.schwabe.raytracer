#pragma once

#include "../Shader.h"

namespace raytracer
{
    template <typename ContainmentType, typename IntersectionType, typename OutputType>
    class ConstShader : public Shader < ContainmentType, IntersectionType, OutputType >
    {
        OutputType constant;

    public:

        ALIGNED_ALLOCATORS(__alignof(ConstShader));

        ConstShader(const OutputType & constantIn)
            :
            constant(constantIn) { }

        virtual ~ConstShader() { }

        const OutputType sample(const ContainmentType &, const IntersectionType &) const {
            return constant;
        }

        const OutputType operator()(const ContainmentType &, const IntersectionType &) const {
            return constant;
        }

        const OutputType & getConstant() const {
            return constant;
        }
    };
}
