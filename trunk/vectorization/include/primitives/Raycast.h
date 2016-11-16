#pragma once

#include "Ray.h"

namespace primitives
{

    /*
    * Combines casting data about a ray
    */
    struct Raycast
    {
        Ray ray;

        // x: indicates backface culling wanted
        // y: indicates frontface culling wanted
        // z, w: reserved
        vectorization::Float4::VectorBoolType cullingMask;

        vectorization::Size2 originIds;

        vectorization::Float maxDistance;

        ALIGNED_ALLOCATORS(__alignof(Raycast));

        Raycast();

        explicit Raycast(
            const Ray & ray,
            const vectorization::Float4::VectorBoolType & cullingMask,
            const vectorization::Size2 & originIds,
            const vectorization::Float maxDistance);

    };

    // Note: changes the maxDistance to the max-value of its value-type,
    // instead of taking over the original maxDistance.
    const Raycast toObjectSpace(
        const Raycast & r,
        const vectorization::Float44 & transInvModelMatrix,
        const vectorization::Float4 & osOrigin);

    const Raycast toObjectSpace(const Raycast & r, const vectorization::Float44 & inverseModelMatrix);

    const vectorization::Int cullingOrientation(const Raycast & r);

    const vectorization::Float4::VectorBoolType cullingOrientationToMask(const vectorization::Int cullingOrientation);

    const vectorization::Float4::VectorBoolType backfaceCulledv(const Raycast & r);

    const bool backfaceCulled(const Raycast & r);

    const vectorization::Float4::VectorBoolType frontfaceCulledv(const Raycast & r);

    const bool frontfaceCulled(const Raycast & r);

    const vectorization::Float4::VectorBoolType notCulledv(const Raycast & r);

    const bool notCulled(const Raycast & r);

    const bool selfOcclusion(const Raycast & r, const vectorization::Size2::ValueType toCheckId);

    const bool selfOcclusion(
        const Raycast & r,
        const vectorization::Size2::ValueType toCheckId,
        const vectorization::Float t,
        const vectorization::Float epsilon);

    const bool selfOcclusion(
        const Raycast & r,
        const vectorization::Size2::ValueType toCheckId,
        const vectorization::Float t);

    const bool outOfReach(const Raycast & r, const vectorization::Float t);

    const vectorization::Float4::VectorBoolType outOfReach(const Raycast & r, const vectorization::Float4 & t);

    const vectorization::Float4 farPoint(const Raycast & r);

}
