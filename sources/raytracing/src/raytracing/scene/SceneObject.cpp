#include "raytracing/scene/SceneObject.h"
#include "../../stdafx.h"

namespace raytracer
{
    SceneObject::SceneObject(const std::string & idIn)
        :
        ObjectShader(),
        bounding(),
        form(),
        id(idIn) {
        resetModelMatrix();
    }

    SceneObject::~SceneObject() { }

    const std::string & SceneObject::getId() const {
        return id;
    }

    const Form * const SceneObject::getForm() const {
        return form;
    }

    void SceneObject::setForm(const Form * const value) {
        this->form = value;
        updateBounding();
    }

    void SceneObject::updateBounding() {
        if (form) {
            bounding = transform(form->getBounding(), modelMatrix);
        }
    }

    const AxisAlignedBoundingBox SceneObject::includeInBounding(const AxisAlignedBoundingBox & aabb) const {
        return extendBy(aabb, bounding);
    }

    const bool SceneObject::overlaps(const AxisAlignedBoundingBox & aabb) const {
        return primitives::overlaps(aabb, bounding);
    }

    const Float SceneObject::transformIntersection(const Raycast & raycast, const FacetIntersection & facetIntersection, FacetIntersection & intersectionOut) const {
        // transform intersection point in msVertex to world-space
        const Float4 msVertex = facetIntersection.msVertex;
        const Float4 wsVertex = modelMatrix * msVertex;

        // the returned distance is in model-space, but we need it in world-space
        const Float4 wsDistance = distance3v(raycast.ray.origin, wsVertex);
        if (allTrue(outOfReach(raycast, wsDistance))) {
            return raycast.maxDistance;
        }

        intersectionOut.msVertex = msVertex;

        const Float4 msSurfaceNormal = facetIntersection.msSurfaceNormal;
        intersectionOut.msSurfaceNormal = msSurfaceNormal;
        intersectionOut.vertex = wsVertex;
        intersectionOut.surfaceNormal = normalize3(transposeInverseModelMatrix * msSurfaceNormal);
        intersectionOut.smoothedNormal = normalize3(transposeInverseModelMatrix * facetIntersection.smoothedNormal);
        intersectionOut.texCoords = textureMatrix * facetIntersection.texCoords;
        intersectionOut.node = facetIntersection.node;
        //std::cout << "hit on " << intersectionOut.msVertex << std::endl;
        return x(wsDistance);
    }

    const Raycast transformRaycastToObjectSpace(const ASizeT objectOriginId, const Raycast & r, const FacetIntersection * const originIntersection, const Float44 & transposeInverseModelMatrix) {
        if (originIntersection && x(r.originIds) == objectOriginId) {
            Raycast originatedRaycast = toObjectSpace(r, transposeInverseModelMatrix, originIntersection->msVertex);

            // avoid self occlusion when intersection is to be tested originated at former intersection
            // when direction has not significantly moved away from surface
            const Float4 surfaceNormal = originIntersection->msSurfaceNormal;
            const Float4 orientation = dotv(
                originatedRaycast.ray.direction,
                blendMasked(-surfaceNormal, surfaceNormal, backfaceCulledv(originatedRaycast)));
            // we must move the originated ray if it faces towards the intersection
            if (isNegative(orientation)) {
                // The more near the ray direction is parallel to the surface, the more add to the movement fraction.
                const Float4 far = farPoint(originatedRaycast);
                const Float4 movedOrigin = mix(
                    far,
                    originatedRaycast.ray.origin,
                    One<Float4>() + SelfOcclusionEpsilon<Float4>() * (orientation + Two<Float4>()));
                originatedRaycast.ray.origin = movedOrigin;
                // update of max distance required as we moved origin away from far point
                originatedRaycast.maxDistance = distance3(movedOrigin, far);
            }

            return originatedRaycast;
        }
        // Ray origin wasn't derived from intersection with this scene object,
        // so no other alternative is available as transforming it into object's space
        return toObjectSpace(r, transposeInverseModelMatrix);
    }

    // Finds the nearest intersection.
    // Note, the ray.direction is transformed from world-space to object-space.
    // Here it's important to preserve the "P + t*d" (more precisely the t)
    // relation, so we won't normalize the object-space direction.
    const Float SceneObject::findNearestIntersection(const Raycast & r, const FacetIntersection * const originIntersection, FacetIntersection & intersectionOut) const {
        // check for intersections with the Form
        const Raycast osr = transformRaycastToObjectSpace(reinterpret_cast<ASizeT>(this), r, originIntersection, transposeInverseModelMatrix);
        FacetIntersection osIntersectionOut = intersectionOut;

        if (outOfReach(osr, form->findNearestIntersection(osr, originIntersection, osIntersectionOut))) {
            return r.maxDistance;
        }

        return transformIntersection(r, osIntersectionOut, intersectionOut);
    }

    const Float SceneObject::findAnyIntersection(const Raycast & r, const FacetIntersection * const originIntersection, FacetIntersection & intersectionOut) const {
        // check for intersections with the Form
        const Raycast osr = transformRaycastToObjectSpace(reinterpret_cast<ASizeT>(this), r, originIntersection, transposeInverseModelMatrix);
        FacetIntersection osIntersectionOut = intersectionOut;

        if (outOfReach(osr, form->findAnyIntersection(osr, originIntersection, osIntersectionOut))) {
            return r.maxDistance;
        }

        return transformIntersection(r, osIntersectionOut, intersectionOut);
    }
}