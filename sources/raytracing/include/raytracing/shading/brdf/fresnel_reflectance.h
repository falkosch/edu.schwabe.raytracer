#pragma once

#include <vectorization.h>

namespace raytracer::brdf
{
    using namespace vectorization;

    // The Fresnel reflectance at a dielectric interface, in all lanes: 1 for a total internal reflection into a less
    // dense medium, Schlick's approximation otherwise. negNdotI is the cosine between the normal on the incident side
    // and the reversed incident direction. eta is laid out as RefractionIndices::refractionEta(etaI, etaT) gives it,
    // (etaI / etaT, etaT / etaI, etaI, etaT), with I the medium of the incident ray and T the other one. Entering a
    // less dense medium (etaT < etaI) takes the cosine of the transmitted direction from Snell's law.
    Float4 fresnelReflectance(bool totalInternalReflection, const Float4& negNdotI, const Float4& eta);
}
