#pragma once

#include "../Shader.h"

#include <vectorization.h>

namespace raytracer
{
    template <
        typename ContainmentType, typename IntersectionType, typename OutputType, typename ConcatenationFunctor,
        typename LeftShader, typename RightShader>
    class ConcatenatingShader : public Shader<ContainmentType, IntersectionType, OutputType>
    {
    public:
        ConcatenatingShader(ConcatenationFunctor concat, const LeftShader& left, const RightShader& right)
            : concat(concat), left(&left), right(&right)
        {
        }

        ~ConcatenatingShader() override = default;

        const OutputType sample(const ContainmentType& containment, const IntersectionType& intersection) const override
        {
            return concat(left->sample(containment, intersection), right->sample(containment, intersection));
        }

        const ConcatenationFunctor& getConcat() const
        {
            return concat;
        }

        const LeftShader& getLeft() const
        {
            return *left;
        }

        const RightShader& getRight() const
        {
            return *right;
        }

    protected:
        ConcatenationFunctor concat;
        const LeftShader* left;
        const RightShader* right;
    };
}
