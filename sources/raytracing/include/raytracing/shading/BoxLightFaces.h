#pragma once

#include <vectorization.h>

namespace raytracer
{
    using namespace vectorization;

    struct LightInfo;

    // The emitting faces of a box light (LightInfo::halfExtents) that a point sees, clipped at the point's horizon:
    // convex polygons relative to the point, at most one face per axis of the box. They give the light's exact
    // irradiance without shadows (the polygons' projected solid angle, Lambert's form factor) and the area its shadow
    // rays spread over.
    struct BoxLightFaces
    {
        static constexpr ASizeT MaxFaces = 3;

        // a rectangle clipped by a plane
        static constexpr ASizeT MaxVertices = 5;

        struct Face
        {
            // the clipped polygon relative to the point, its corners in order around it
            Float4 vertices[MaxVertices];
            ASizeT vertexCount;

            // the triangles (vertices[0], vertices[i + 1], vertices[i + 2]) that make up the polygon, by area
            Float triangleAreas[MaxVertices - 2];
            Float area;

            // irradiance per radiance: the integral of the cosine to the point's normal over the polygon's solid angle
            Float projectedSolidAngle;

            // the whole face before clipping: its outward normal, its center relative to the point, and its corners
            // center +- tangentU +- tangentV
            Float4 normal;
            Float4 center;
            Float4 tangentU;
            Float4 tangentV;

            Face();
        };

        Face faces[MaxFaces];
        ASizeT faceCount;

        // the sums over the faces
        Float area;
        Float projectedSolidAngle;

        // The faces of the light that emit towards the point and lie (partly) above the horizon of its normal
        BoxLightFaces(const LightInfo& light, const Float4& point, const Float4& normal) noexcept;

        // The point of the faces' polygons for xi in [0, 1)^2 (x, y), relative to the point: equal areas of the
        // polygons get equal areas of xi, the faces and their triangles side by side along x. face is set to its face.
        Float4 samplePoint(const Float4& xi, ASizeT& face) const noexcept;

        // The point of the faces' polygons closest to the point, relative to it. face is set to its face.
        Float4 nearestPoint(ASizeT& face) const noexcept;

        // The representative point of a face for a highlight (Karis 2013), relative to the point: the point of the
        // whole face closest to the ray from the point in the reflected direction
        Float4 representativePoint(ASizeT face, const Float4& reflectedDirection) const noexcept;
    };
}
