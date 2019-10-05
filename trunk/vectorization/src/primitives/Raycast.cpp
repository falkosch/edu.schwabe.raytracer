#include "primitives/Raycast.h"

#include <limits>

using namespace vectorization;

namespace primitives
{
    Raycast::Raycast() noexcept
        :
        ray(),
        cullingMask(),
        originIds(),
        maxDistance(std::numeric_limits<Float>::max()) { }

    Raycast::Raycast(
        const Ray & rayIn,
        const Float4::VectorBoolType & cullingMaskIn,
        const Size2 & originIdsIn,
        const Float maxDistanceIn
    ) noexcept
        : ray(rayIn), cullingMask(cullingMaskIn), originIds(originIdsIn), maxDistance(maxDistanceIn) { }

    const Raycast toObjectSpace(const Raycast & r, const Float44 & transInvModelMatrix, const Float4 & osOrigin) noexcept {
        return Raycast(
            Ray(osOrigin, normalize3(r.ray.direction * transInvModelMatrix)),
            r.cullingMask,
            r.originIds,
            distance3(osOrigin, farPoint(r) * transInvModelMatrix)
        );
    }

    const Raycast toObjectSpace(const Raycast & r, const Float44 & transInvModelMatrix) noexcept {
        return toObjectSpace(r, transInvModelMatrix, r.ray.origin * transInvModelMatrix);
    }

    const Float4::VectorBoolType backfaceCulledv(const Raycast & r) noexcept {
        return xxxx(r.cullingMask);
    }

    const bool backfaceCulled(const Raycast & r) noexcept {
        return !!x(backfaceCulledv(r));
    }

    const Float4::VectorBoolType frontfaceCulledv(const Raycast & r) noexcept {
        return yyyy(r.cullingMask);
    }

    const bool frontfaceCulled(const Raycast & r) noexcept {
        return !!x(frontfaceCulledv(r));
    }

    const Float4::VectorBoolType notCulledv(const Raycast & r) noexcept {
        return ~(backfaceCulledv(r) ^ frontfaceCulledv(r));
    }

    const bool notCulled(const Raycast & r) noexcept {
        return !!x(notCulledv(r));
    }

    const Int cullingOrientation(const Raycast & r) noexcept {
        if (notCulled(r)) return Zero<Int>();
        return backfaceCulled(r) ? NegativeOne<Int>() : One<Int>();
    }

    const Float4::VectorBoolType cullingOrientationToMask(const Int cullingOrientation) noexcept {
        return convert<Float4::VectorBoolType>(cullingOrientation < Zero<Float4::BoolType>(), cullingOrientation > Zero<Float4::BoolType>());
    }

    const bool selfOcclusion(const Raycast & r, const Size2::ValueType toCheckId) noexcept {
        return y(r.originIds) == toCheckId;
    }

    const bool selfOcclusion(const Raycast & r, const Size2::ValueType toCheckId, const Float t, const Float epsilon) noexcept {
        return selfOcclusion(r, toCheckId) & (t <= epsilon);
    }

    const bool selfOcclusion(const Raycast & r, const Size2::ValueType toCheckId, const Float t) noexcept {
        return selfOcclusion(r, toCheckId, t, Epsilon<Float>());
    }

    const bool outOfReach(const Raycast & r, const Float t) noexcept {
        return allTrue(outOfReach(r, Float4(t)));
    }

    const Float4::VectorBoolType outOfReach(const Raycast & r, const Float4 & t) noexcept {
        const Float4 rMaxDistanceV = Float4(r.maxDistance);
        // t == NaN or (r.maxDistance != NaN and t >= r.maxDistance)
        return isNaN(t) | (andnot(isNaN(rMaxDistanceV), t >= rMaxDistanceV));
    }

    const Float4 farPoint(const Raycast & r) noexcept {
        return point(r.ray, r.maxDistance);
    }
}