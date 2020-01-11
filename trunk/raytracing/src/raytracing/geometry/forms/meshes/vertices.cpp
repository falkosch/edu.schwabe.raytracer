#include "raytracing/geometry/forms/meshes/vertices.h"
#include "../../../../stdafx.h"

namespace raytracer
{
    const AxisAlignedBoundingBox computeBounding(
        const std::vector<Float4> & vertices
    ) noexcept {
        auto allBounding = AxisAlignedBoundingBox();

#pragma omp parallel
        {
            auto bounding = AxisAlignedBoundingBox();

#pragma omp for
            for (int i{ 0 }; i < static_cast<int>(vertices.size()); i++) {
                bounding = extendBy(
                    bounding,
                    vertices[static_cast<ASizeT>(i)]
                );
            }

#pragma omp critical
            {
                allBounding = extendBy(allBounding, bounding);
            }
        }

        return allBounding;
    }

    const AxisAlignedBoundingBox computeStandardMesh(
        std::vector<Float4> & vertices
    ) noexcept {
        auto prebounds = computeBounding(vertices);
        auto translation = center(prebounds);
        auto verticesExtents = extents(prebounds);
        auto scale = reciprocal(
            min3v(
                blendMasked(
                    verticesExtents,
                    max3v(verticesExtents),
                    verticesExtents == Zero<Float4>()
                )
            )
        );

#pragma omp parallel for
        for (int i{ 0 }; i < static_cast<int>(vertices.size()); i++) {
            auto index = static_cast<ASizeT>(i);
            vertices[index] = replaceW(
                (vertices[index] - translation) * scale,
                One<Float>()
            );
        }

        return computeBounding(vertices);
    }
}