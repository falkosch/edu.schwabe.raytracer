#pragma once

#include "../Shader.h"

namespace raytracer
{
    template <typename ContainmentType, typename IntersectionType, typename OutputType, typename OtherShader>
    class MultiplyByConstShader : public Shader<ContainmentType, IntersectionType, OutputType>
    {
        OutputType constant;

        const OtherShader* otherShader;

    public:
        MultiplyByConstShader(const OutputType& constantIn, const OtherShader& otherShaderIn)
            : constant(constantIn), otherShader(&otherShaderIn)
        {
        }

        ~MultiplyByConstShader() override = default;

        OutputType sample(const ContainmentType& containment, const IntersectionType& intersection) const override
        {
            return constant * otherShader->sample(containment, intersection);
        }

        OutputType& getConstant() const
        {
            return constant;
        }

        OtherShader& getOtherShader() const
        {
            return *otherShader;
        }
    };
}
