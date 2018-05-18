#pragma once

#include "Ray.h"

namespace primitives
{
	using namespace vectorization;

    /*
    * Combines casting data about a ray
    */
    struct Raycast
    {
        Ray ray;

        // x: indicates backface culling wanted
        // y: indicates frontface culling wanted
        // z, w: reserved
        Float4::VectorBoolType cullingMask;

        Size2 originIds;

        Float maxDistance;

        ALIGNED_ALLOCATORS(__alignof(Raycast));

        Raycast();

        explicit Raycast(
            const Ray & ray,
            const Float4::VectorBoolType & cullingMask,
            const Size2 & originIds,
            const Float maxDistance);

    };

    // Note: changes the maxDistance to the max-value of its value-type,
    // instead of taking over the original maxDistance.
    const Raycast toObjectSpace(
        const Raycast & r,
        const Float44 & transInvModelMatrix,
        const Float4 & osOrigin);

    const Raycast toObjectSpace(const Raycast & r, const Float44 & inverseModelMatrix);

    const Int cullingOrientation(const Raycast & r);

    const Float4::VectorBoolType cullingOrientationToMask(const Int cullingOrientation);

    const Float4::VectorBoolType backfaceCulledv(const Raycast & r);

    const bool backfaceCulled(const Raycast & r);

    const Float4::VectorBoolType frontfaceCulledv(const Raycast & r);

    const bool frontfaceCulled(const Raycast & r);

    const Float4::VectorBoolType notCulledv(const Raycast & r);

    const bool notCulled(const Raycast & r);

    const bool selfOcclusion(const Raycast & r, const Size2::ValueType toCheckId);

    const bool selfOcclusion(
        const Raycast & r,
        const Size2::ValueType toCheckId,
        const Float t,
        const Float epsilon);

    const bool selfOcclusion(
        const Raycast & r,
        const Size2::ValueType toCheckId,
        const Float t);

    const bool outOfReach(const Raycast & r, const Float t);

    const Float4::VectorBoolType outOfReach(const Raycast & r, const Float4 & t);

    const Float4 farPoint(const Raycast & r);

}
