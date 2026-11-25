#pragma once

#include "../Shader.h"

namespace raytracer
{
    // A box in two values by its faces: one on the faces of the mask, the other elsewhere. The face is the one of the
    // intersection's model-space normal (msSurfaceNormal, along one of the box's axes); the mask has the bit
    // 2 * axis + 1 for the face at the axis's positive end and 2 * axis for the negative one, as LightInfo::faceBit: a box
    // light's emitter emits from the light's emitting faces only.
    template <typename ContainmentType, typename IntersectionType, typename OutputType>
    class BoxFacesShader final : public Shader<ContainmentType, IntersectionType, OutputType>
    {
        UInt_32 faces;
        OutputType onFaces;
        OutputType elsewhere;

    public:
        BoxFacesShader(const UInt_32 facesIn, const OutputType& onFacesIn, const OutputType& elsewhereIn)
            : faces(facesIn), onFaces(onFacesIn), elsewhere(elsewhereIn)
        {
        }

        ~BoxFacesShader() override = default;

        OutputType sample(const ContainmentType&, const IntersectionType& intersection) const override
        {
            // the axis of the normal's largest component, and its side
            const auto& normal = intersection.msSurfaceNormal;
            const auto magnitude = abs(normal);
            const auto axis = x(magnitude) >= y(magnitude) && x(magnitude) >= z(magnitude)
                                  ? 0u
                                  : y(magnitude) >= z(magnitude) ? 1u : 2u;
            const auto component = axis == 0u ? x(normal) : axis == 1u ? y(normal) : z(normal);
            const auto bit = 1u << (2u * axis + (component > Zero<Float>() ? 1u : 0u));
            return (faces & bit) != 0u ? onFaces : elsewhere;
        }

        UInt_32 getFaces() const
        {
            return faces;
        }
    };
}
