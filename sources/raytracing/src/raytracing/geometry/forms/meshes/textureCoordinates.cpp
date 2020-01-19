#include "raytracing/geometry/forms/meshes/facets.h"
#include "../../../../stdafx.h"

namespace raytracer
{
    void computeTexCoordsOrtho(
        const std::vector<UInt3> & facetIndices,
        const std::vector<Facet> & facets,
        const Float4 & sPlane,
        const Float4 & tPlane,
        std::vector<FacetTexCoords4> & texCoords
    ) noexcept {
        texCoords.resize(facetIndices.size());

#pragma omp parallel for
        for (auto i = int{ 0 }; i < static_cast<int>(facetIndices.size()); i++) {
            auto index = static_cast<ASizeT>(i);
            auto facet = facets[index];
            texCoords[index] = FacetTexCoords4{
                Float4{
                    dot(facet.v0, sPlane),
                    dot(facet.v0, tPlane)
                },
                Float4{
                    dot(facet.v1, sPlane),
                    dot(facet.v1, tPlane)
                },
                Float4{
                    dot(facet.v2, sPlane),
                    dot(facet.v2, tPlane)
                }
            };
        }
    }

    void computeTexCoordsSpherical(
        const std::vector<UInt3> & facetIndices,
        const std::vector<Facet> & facets,
        std::vector<FacetTexCoords4> & texCoords
    ) noexcept {
        texCoords.resize(facetIndices.size());

        // calculate tex-coords for each vertex-normal
#pragma omp parallel for
        for (auto i = int{ 0 }; i < static_cast<int>(facetIndices.size()); i++) {
            auto index = static_cast<ASizeT>(i);

            // normalize vertices, they are used as normals for determining the texcoords
            // "- Common::ONE_W" elimates w-coord, which is always 1.0f for vertices
            auto facet = facets[index];
            const Facet normalized{
                normalize3(facet.v0),
                normalize3(facet.v1),
                normalize3(facet.v2)
            };

            // calculate latitude coords
            FacetTexCoords4 facetTexCoords{
                Float4{
                    (vectorization::atan2(x(normalized.v0), z(normalized.v0)) + Pi<Float>()) * RadianToUniform<Float>(),
                    vectorization::acos(y(-normalized.v0)) * ReciprocalPi<Float>()
                },
                Float4{
                    (vectorization::atan2(x(normalized.v1), z(normalized.v1)) + Pi<Float>()) * RadianToUniform<Float>(),
                    vectorization::acos(y(-normalized.v1)) * ReciprocalPi<Float>()
                },
                Float4{
                    (vectorization::atan2(x(normalized.v2), z(normalized.v2)) + Pi<Float>()) * RadianToUniform<Float>(),
                    vectorization::acos(y(-normalized.v2)) * ReciprocalPi<Float>()
                }
            };
            texCoords[index] = facetTexCoords;

            // general idea: http://www.biederman.net/leslie/terranim8or/sphere_mapping.htm
            // the distortion happens on the negative z-side at the transition of positive to negative x-coords
            // so we're determine min, mid and max to check where a triangle is positioned
            if (z(facet.v0) > 0.0f || z(facet.v1) > 0.0f || z(facet.v2) > 0.0f)
                continue;

            // search min, max and mid points of facet
            auto min = facet.v0;
            auto mid = facet.v0;
            auto max = facet.v0;
            auto & minUV = facetTexCoords.v0;
            auto & midUV = facetTexCoords.v0;
            auto & maxUV = facetTexCoords.v0;

            if (x(facet.v1) < x(facet.v0)) {
                if (x(facet.v2) < x(facet.v1)) {
                    mid = facet.v1;
                    midUV = facetTexCoords.v1;
                    min = facet.v2;
                    minUV = facetTexCoords.v2;
                } else if (x(facet.v2) < x(facet.v0)) {
                    min = facet.v1;
                    minUV = facetTexCoords.v1;
                    mid = facet.v2;
                    midUV = facetTexCoords.v2;
                } else {
                    min = facet.v1;
                    minUV = facetTexCoords.v1;
                    max = facet.v2;
                    maxUV = facetTexCoords.v2;
                }
            } else if (x(facet.v1) >= x(facet.v0)) {
                if (x(facet.v2) >= x(facet.v1)) {
                    mid = facet.v1;
                    midUV = facetTexCoords.v1;
                    max = facet.v2;
                    maxUV = facetTexCoords.v2;
                } else if (x(facet.v2) >= x(facet.v0)) {
                    max = facet.v1;
                    maxUV = facetTexCoords.v1;
                    mid = facet.v2;
                    midUV = facetTexCoords.v2;
                } else {
                    max = facet.v1;
                    maxUV = facetTexCoords.v1;
                    min = facet.v2;
                    minUV = facetTexCoords.v2;
                }
            }

            // case 1: facet overlaps x-axis; min->x is negative, max->x is positive
            if (x(min) < 0.0f && x(max) > 0.0f) {
                // subcase for easier fixing: if mid->x is not negative, only minUV needs to be fixed
                if (x(mid) >= 0.0f)
                    minUV += wzyx(OneW<Float4>());
                else // subcase: mid is on the negative side, only maxUV needs to be fixed
                    maxUV -= wzyx(OneW<Float4>());
            }
            // case 2: facet touches the origin of the x-axis with max-vertex, but doesn't overlap
            else if (x(max) == 0.0f) {
                if (x(mid) == 0.0f)
                    minUV += wzyx(OneW<Float4>());
                else
                    maxUV += wzyx(OneW<Float4>());
            }

            texCoords[index] = facetTexCoords;
        }
    }
}