#include "raytracing/shading/BoxLightFaces.h"
#include "../../stdafx.h"

#include "raytracing/shading/LightInfo.h"

namespace raytracer
{
    namespace
    {
        // Sutherland-Hodgman: the part of the quadrilateral above the plane through the origin with the normal. A
        // convex polygon crosses the plane twice at most; the guard only keeps rounding from writing past the end.
        ASizeT clipAtHorizon(
            const Float4 (&corners)[4], const Float4& normal, Float4 (&clipped)[BoxLightFaces::MaxVertices]
        ) noexcept
        {
            auto count = Zero<ASizeT>();
            auto previous = corners[3];
            auto previousHeight = dot3(normal, previous);
            for (const auto& corner : corners)
            {
                const auto height = dot3(normal, corner);
                if ((height >= Zero<Float>()) != (previousHeight >= Zero<Float>())
                    && count < BoxLightFaces::MaxVertices)
                {
                    clipped[count++] = previous + (corner - previous) * (previousHeight / (previousHeight - height));
                }
                if (height >= Zero<Float>() && count < BoxLightFaces::MaxVertices)
                {
                    clipped[count++] = corner;
                }
                previous = corner;
                previousHeight = height;
            }
            return count;
        }

        // The projected solid angle of a convex polygon above the horizon of the normal, seen from the origin
        // (Lambert's contour integral): half the sum over the edges of the angle they span times the cosine between the
        // normal and the normal of the plane through the edge and the origin
        Float polygonProjectedSolidAngle(const BoxLightFaces::Face& face, const Float4& normal) noexcept
        {
            auto sum = Zero<Float>();
            auto previous = normalize3(face.vertices[face.vertexCount - One<ASizeT>()]);
            for (auto i = Zero<ASizeT>(); i < face.vertexCount; ++i)
            {
                const auto current = normalize3(face.vertices[i]);
                const auto edgeNormal = cross3(previous, current);
                // atan2 of the sine and cosine stays accurate for the short edges of a distant light, acos doesn't
                const auto sine = length3(edgeNormal);
                if (sine > Zero<Float>())
                {
                    const auto angle = vectorization::atan2(dot3(previous, current), sine);
                    sum += angle / sine * dot3(normal, edgeNormal);
                }
                previous = current;
            }
            return Half<Float>() * vectorization::abs(sum);
        }

        // The point of the segment from a to b closest to the origin
        Float4 nearestOnSegment(const Float4& a, const Float4& b) noexcept
        {
            const auto edge = b - a;
            const auto edgeLengthSquared = dot3(edge, edge);
            const auto t = edgeLengthSquared > Zero<Float>()
                               ? clamp(-dot3(a, edge) / edgeLengthSquared, Zero<Float>(), One<Float>())
                               : Zero<Float>();
            return a + edge * t;
        }

        // The point of a face's convex polygon closest to the origin
        Float4 nearestOnFace(const BoxLightFaces::Face& face) noexcept
        {
            // the foot of the perpendicular onto the face's plane, if it lies inside the polygon: on the same side of
            // all edges
            const auto foot = face.normal * dot3(face.center, face.normal);
            auto inFront = false;
            auto behind = false;
            for (auto i = Zero<ASizeT>(); i < face.vertexCount; ++i)
            {
                const auto& a = face.vertices[i];
                const auto& b = face.vertices[(i + One<ASizeT>()) % face.vertexCount];
                const auto side = dot3(face.normal, cross3(b - a, foot - a));
                inFront = inFront || side > Zero<Float>();
                behind = behind || side < Zero<Float>();
            }
            if (!(inFront && behind))
            {
                return foot;
            }

            auto nearest = face.vertices[0];
            auto nearestDistanceSquared = dot3(nearest, nearest);
            for (auto i = Zero<ASizeT>(); i < face.vertexCount; ++i)
            {
                const auto candidate = nearestOnSegment(
                    face.vertices[i], face.vertices[(i + One<ASizeT>()) % face.vertexCount]
                );
                const auto distanceSquared = dot3(candidate, candidate);
                if (distanceSquared < nearestDistanceSquared)
                {
                    nearest = candidate;
                    nearestDistanceSquared = distanceSquared;
                }
            }
            return nearest;
        }
    }

    BoxLightFaces::Face::Face()
        : vertices(), vertexCount(), triangleAreas(), area(), projectedSolidAngle(), normal(), center(), tangentU(),
          tangentV()
    {
    }

    BoxLightFaces::BoxLightFaces(const LightInfo& light, const Float4& point, const Float4& normal) noexcept
        : faces(), faceCount(), area(), projectedSolidAngle()
    {
        const Float4 axes[3] = {
            light.orientation * OneX<Float4>(), light.orientation * OneY<Float4>(), light.orientation * OneZ<Float4>()
        };
        const Float halfExtents[3] = {x(light.halfExtents), y(light.halfExtents), z(light.halfExtents)};
        const auto toCenter = zeroW(light.position - point);

        for (auto axis = Zero<ASizeT>(); axis < 3; ++axis)
        {
            // the face on the point's side along the axis emits towards it if the point lies beyond its plane
            const auto height = -dot3(toCenter, axes[axis]);
            const auto positive = height > Zero<Float>();
            const auto u = (axis + One<ASizeT>()) % 3;
            const auto v = (axis + Two<ASizeT>()) % 3;
            if (vectorization::abs(height) <= halfExtents[axis]
                || !(light.emittingFaces & LightInfo::faceBit(axis, positive))
                || !(halfExtents[u] > Zero<Float>() && halfExtents[v] > Zero<Float>()))
            {
                continue;
            }

            auto& face = faces[faceCount];
            face.normal = positive ? axes[axis] : -axes[axis];
            face.center = toCenter + face.normal * halfExtents[axis];
            face.tangentU = axes[u] * halfExtents[u];
            face.tangentV = axes[v] * halfExtents[v];
            const auto& c = face.center;
            const auto& U = face.tangentU;
            const auto& V = face.tangentV;
            const Float4 corners[4] = {c - U - V, c + U - V, c + U + V, c - U + V};
            face.vertexCount = clipAtHorizon(corners, normal, face.vertices);
            if (face.vertexCount < 3)
            {
                continue;
            }

            face.area = Zero<Float>();
            for (auto i = Zero<ASizeT>(); i + Two<ASizeT>() < face.vertexCount; ++i)
            {
                const auto& origin = face.vertices[0];
                face.triangleAreas[i] = Half<Float>() * length3(cross3(
                    face.vertices[i + One<ASizeT>()] - origin, face.vertices[i + Two<ASizeT>()] - origin
                ));
                face.area += face.triangleAreas[i];
            }
            face.projectedSolidAngle = polygonProjectedSolidAngle(face, normal);
            if (!(face.area > Zero<Float>()))
            {
                continue;
            }

            area += face.area;
            projectedSolidAngle += face.projectedSolidAngle;
            ++faceCount;
        }
    }

    Float4 BoxLightFaces::samplePoint(const Float4& xi, ASizeT& face) const noexcept
    {
        // walk the triangles to the one that covers x(xi) of the total area, the last one if rounding runs past it
        auto remaining = x(xi) * area;
        face = faceCount - One<ASizeT>();
        auto triangle = faces[face].vertexCount - Two<ASizeT>() - One<ASizeT>();
        auto triangleX = One<Float>();
        auto found = false;
        for (auto f = Zero<ASizeT>(); f < faceCount && !found; ++f)
        {
            for (auto i = Zero<ASizeT>(); i + Two<ASizeT>() < faces[f].vertexCount; ++i)
            {
                const auto triangleArea = faces[f].triangleAreas[i];
                if (remaining < triangleArea)
                {
                    face = f;
                    triangle = i;
                    triangleX = remaining / triangleArea;
                    found = true;
                    break;
                }
                remaining -= triangleArea;
            }
        }

        // equal areas of the triangle by the square root of x
        const auto& vertices = faces[face].vertices;
        const auto& a = vertices[0];
        const auto s = vectorization::sqrt(triangleX);
        return a + (vertices[triangle + One<ASizeT>()] - a) * (s * (One<Float>() - y(xi)))
            + (vertices[triangle + Two<ASizeT>()] - a) * (s * y(xi));
    }

    Float4 BoxLightFaces::nearestPoint(ASizeT& face) const noexcept
    {
        face = Zero<ASizeT>();
        auto nearest = nearestOnFace(faces[0]);
        auto nearestDistanceSquared = dot3(nearest, nearest);
        for (auto f = One<ASizeT>(); f < faceCount; ++f)
        {
            const auto candidate = nearestOnFace(faces[f]);
            const auto distanceSquared = dot3(candidate, candidate);
            if (distanceSquared < nearestDistanceSquared)
            {
                face = f;
                nearest = candidate;
                nearestDistanceSquared = distanceSquared;
            }
        }
        return nearest;
    }

    Float4 BoxLightFaces::representativePoint(const ASizeT face, const Float4& reflectedDirection) const noexcept
    {
        const auto& f = faces[face];
        // the point lies in front of the face: dot3(center, normal) < 0
        const auto centerHeight = dot3(f.center, f.normal);
        const auto directionHeight = dot3(reflectedDirection, f.normal);

        // where the ray meets the face's plane; a ray that runs away from it gets the plane's point far along it
        Float4 onPlane;
        if (directionHeight < -1e-6f)
        {
            onPlane = reflectedDirection * (centerHeight / directionHeight);
        }
        else
        {
            const auto distant = 1e3f * (length3(f.center) + length3(f.tangentU) + length3(f.tangentV));
            onPlane = f.normal * centerHeight + (reflectedDirection - f.normal * directionHeight) * distant;
        }

        // clamped to the face
        const auto offset = onPlane - f.center;
        const auto u = clamp(
            dot3(offset, f.tangentU) / dot3(f.tangentU, f.tangentU), NegativeOne<Float>(), One<Float>()
        );
        const auto v = clamp(
            dot3(offset, f.tangentV) / dot3(f.tangentV, f.tangentV), NegativeOne<Float>(), One<Float>()
        );
        return f.center + f.tangentU * u + f.tangentV * v;
    }
}
