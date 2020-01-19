#pragma once

#include "../Shader.h"

namespace raytracer
{
    template <typename ContainmentType, typename IntersectionType, typename OutputType, typename OtherShader>
    class MultiplyByConstShader : public Shader < ContainmentType, IntersectionType, OutputType >
    {
        OutputType constant;

        const OtherShader * otherShader;

    public:

        MultiplyByConstShader(
            const OutputType & constantIn,
            const OtherShader & otherShaderIn)
            :
            constant(constantIn),
            otherShader(&otherShaderIn) { }

        virtual ~MultiplyByConstShader() { }

        const OutputType sample(const ContainmentType & containment, const IntersectionType & intersection) const {
            return (*this)(containment, intersection);
        }

        const OutputType operator()(const ContainmentType & containment, const IntersectionType & intersection) const {
            return constant * (*otherShader)(containment, intersection);
        }

        const OutputType & getConstant() const {
            return constant;
        }

        const OtherShader & getOtherShader() const {
            return *otherShader;
        }
    };
}
