#include "raytracing/shading/SphereLightCap.h"
#include "../../stdafx.h"

#include "raytracing/shading/LightInfo.h"

namespace raytracer
{
    SphereLightCap::SphereLightCap(const LightInfo& light, const Float4& point) noexcept
        : origin(point), center(light.position), radius(light.radius)
    {
        const auto toCenter = light.position - point;
        centerDistance = length3(toCenter);
        w = toCenter / centerDistance;
        const auto up = vectorization::abs(y(w)) < 0.999f ? OneY<Float4>() : OneX<Float4>();
        u = normalize3(cross3(up, w));
        v = cross3(w, u);
        visibleCap = vectorization::sqrt(max(Zero<Float>(), One<Float>() - sqr(light.radius / centerDistance)));
    }

    Float4 SphereLightCap::samplePoint(const Float4& xi) const noexcept
    {
        const auto r = vectorization::sqrt(x(xi)) * visibleCap;
        const auto phi = Two<Float>() * Pi<Float>() * y(xi);
        const auto lift = vectorization::sqrt(max(Zero<Float>(), One<Float>() - sqr(r)));
        const auto onDisk = u * (r * vectorization::cos(phi)) + v * (r * vectorization::sin(phi));
        const auto onSphere = center + (onDisk - w * lift) * radius;
        return onSphere - origin;
    }

    Float SphereLightCap::solidAnglePdf(const Float4& toPoint) const noexcept
    {
        // the density on the disk, per area of the sphere: the disk is the cap's projection, scaled by the lift (the
        // cosine between the sphere's normal and the direction to the point's center)
        const auto normal = (origin + toPoint - center) / radius;
        const auto lift = -dot3(normal, w);
        if (!(visibleCap > Zero<Float>()) || !(lift > Zero<Float>()))
        {
            return Zero<Float>();
        }
        const auto areaPdf = lift / (Pi<Float>() * sqr(visibleCap * radius));

        // per solid angle: the distance squared over the cosine at the sphere, which goes to 0 at the cap's rim
        const auto distanceSquared = dot3(toPoint, toPoint);
        const auto distance = vectorization::sqrt(distanceSquared);
        const auto cosine = max(-dot3(normal, toPoint) / distance, Float{1e-4f});
        return areaPdf * distanceSquared / cosine;
    }
}
