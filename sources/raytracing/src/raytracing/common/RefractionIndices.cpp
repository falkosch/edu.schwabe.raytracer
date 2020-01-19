#include "raytracing/common/RefractionIndices.h"
#include "../../stdafx.h"

namespace raytracer
{
    const Float RefractionIndices::Caesium = .345f;
    const Float RefractionIndices::Vacuum = 1.f;
    const Float RefractionIndices::AirSTP = 1.000292f;
    const Float RefractionIndices::Aluminium = 1.0972f;
    const Float RefractionIndices::Tabaxir = 1.15f;
    const Float RefractionIndices::Water = 1.333f;
    const Float RefractionIndices::Glass = 1.5f;
    const Float RefractionIndices::Diamond = 2.42f;

    const Float4 RefractionIndices::refractionEta(const Float indexA, const Float indexB) {
        return Float4(indexA / indexB, indexB / indexA, indexA, indexB);
    }
}