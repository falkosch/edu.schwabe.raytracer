#include <utility>

#include "../../stdafx.h"
#include "raytracing/scene/SceneObject.h"

namespace raytracer {
  SceneObject::SceneObject(std::string idIn) : ObjectShader(), bounding(), form(), id(std::move(idIn)) {
    resetModelMatrix();
  }

  SceneObject::~SceneObject() = default;

  const std::string &SceneObject::getId() const {
    return id;
  }

  const Form *const SceneObject::getForm() const {
    return form;
  }

  void SceneObject::setForm(const Form *const value) {
    this->form = value;
    updateBounding();
  }

  void SceneObject::updateBounding() {
    if (form) {
      bounding = transform(form->getBounding(), modelMatrix);
    }
  }

  const AxisAlignedBoundingBox SceneObject::includeInBounding(const AxisAlignedBoundingBox &aabb) const {
    return extendBy(aabb, bounding);
  }

  const bool SceneObject::overlaps(const AxisAlignedBoundingBox &aabb) const {
    return primitives::overlaps(aabb, bounding);
  }

  const Float SceneObject::transformIntersection(
      const RayCast &rayCast, const FacetIntersection &facetIntersection, FacetIntersection &intersectionOut
  ) const {
    // transform intersection point in msVertex to world-space
    const Float4 msVertex = facetIntersection.msVertex;
    const Float4 wsVertex = modelMatrix * msVertex;

    // the returned distance is in model-space, but we need it in world-space
    const Float4 wsDistance = distance3v(rayCast.ray.origin, wsVertex);
    if (allTrue(outOfReach(rayCast, wsDistance))) {
      return rayCast.maxDistance;
    }

    intersectionOut.msVertex = msVertex;

    const Float4 msSurfaceNormal = facetIntersection.msSurfaceNormal;
    intersectionOut.msSurfaceNormal = msSurfaceNormal;
    intersectionOut.vertex = wsVertex;
    intersectionOut.surfaceNormal = normalize3(transposeInverseModelMatrix * msSurfaceNormal);
    intersectionOut.smoothedNormal = normalize3(transposeInverseModelMatrix * facetIntersection.smoothedNormal);
    intersectionOut.texCoords = textureMatrix * facetIntersection.texCoords;
    intersectionOut.node = facetIntersection.node;
    // std::cout << "hit on " << intersectionOut.msVertex << std::endl;
    return x(wsDistance);
  }

  const RayCast transformRayCastToObjectSpace(
      const ASizeT objectOriginId, const RayCast &rayCast, const FacetIntersection *const originIntersection,
      const Float44 &transposeInverseModelMatrix
  ) {
    if (originIntersection && x(rayCast.originIds) == objectOriginId) {
      RayCast originatedRayCast = toObjectSpace(rayCast, transposeInverseModelMatrix, originIntersection->msVertex);

      // avoid self occlusion when intersection is to be tested originated at former intersection
      // when direction has not significantly moved away from surface
      const Float4 surfaceNormal = originIntersection->msSurfaceNormal;
      const Float4 orientation = dotv(
          originatedRayCast.ray.direction,
          blendMasked(-surfaceNormal, surfaceNormal, backfaceCulledv(originatedRayCast))
      );
      // we must move the originated ray if it faces towards the intersection
      if (isNegative(orientation)) {
        // The more near the ray direction is parallel to the surface, the more add to the movement fraction.
        const Float4 far = farPoint(originatedRayCast);
        const Float4 movedOrigin =
            mix(far, originatedRayCast.ray.origin,
                One<Float4>() + SelfOcclusionEpsilon<Float4>() * (orientation + Two<Float4>()));
        originatedRayCast.ray.origin = movedOrigin;
        // update of max distance required as we moved origin away from far point
        originatedRayCast.maxDistance = distance3(movedOrigin, far);
      }

      return originatedRayCast;
    }
    // Ray origin is not derived from intersection with this scene object,
    // so no other alternative is available as transforming it into object's space
    return toObjectSpace(rayCast, transposeInverseModelMatrix);
  }

  // Finds the nearest intersection.
  // Note, the ray.direction is transformed from world-space to object-space.
  // Here it's important to preserve the "P + t*d" (more precisely the t)
  // relation, so we won't normalize the object-space direction.
  const Float SceneObject::findNearestIntersection(
      const RayCast &r, const FacetIntersection *const originIntersection, FacetIntersection &intersectionOut
  ) const {
    // check for intersections with the Form
    const RayCast osr = transformRayCastToObjectSpace(
        reinterpret_cast<ASizeT>(this), r, originIntersection, transposeInverseModelMatrix
    );
    FacetIntersection osIntersectionOut = intersectionOut;

    if (outOfReach(osr, form->findNearestIntersection(osr, originIntersection, osIntersectionOut))) {
      return r.maxDistance;
    }

    return transformIntersection(r, osIntersectionOut, intersectionOut);
  }

  const Float SceneObject::findAnyIntersection(
      const RayCast &r, const FacetIntersection *const originIntersection, FacetIntersection &intersectionOut
  ) const {
    // check for intersections with the Form
    const RayCast osr = transformRayCastToObjectSpace(
        reinterpret_cast<ASizeT>(this), r, originIntersection, transposeInverseModelMatrix
    );
    FacetIntersection osIntersectionOut = intersectionOut;

    if (outOfReach(osr, form->findAnyIntersection(osr, originIntersection, osIntersectionOut))) {
      return r.maxDistance;
    }

    return transformIntersection(r, osIntersectionOut, intersectionOut);
  }
}