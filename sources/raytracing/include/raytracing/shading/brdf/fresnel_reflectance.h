#pragma once

#include <vectorization.h>

namespace raytracer::brdf
{
    using namespace vectorization;

    // The Fresnel reflectance at a dielectric interface, in all lanes: 1 for a total internal reflection into a less
    // dense medium, the exact Fresnel equations for unpolarized light otherwise (the mean of the s- and p-polarized
    // reflectances, the cosine of the transmitted direction from Snell's law: 0 at all angles for equal indices).
    // negNdotI is the cosine between the normal on the incident side and the reversed incident direction (clamped to
    // [0, 1]). eta is laid out as RefractionIndices::refractionEta(etaI, etaT) gives it, (etaI / etaT, etaT / etaI,
    // etaI, etaT), with I the medium of the incident ray and T the other one.
    Float4 fresnelReflectance(bool totalInternalReflection, const Float4& negNdotI, const Float4& eta);

    // The same for the incident direction at a surface (or microfacet) of the normal on the incident side, which
    // decides on the total internal reflection itself: 1 where refraction is impossible
    Float4 fresnelReflectance(const Float4& incident, const Float4& normal, const Float4& eta);

    // The same from the cosine negNdotI alone (in all lanes, from 0 to 1), which decides on the total internal
    // reflection by Snell's law
    Float4 fresnelReflectance(const Float4& negNdotI, const Float4& eta);
}
