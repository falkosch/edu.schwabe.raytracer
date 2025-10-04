#pragma once

#include "../geometry/forms/Form.h"
#include "../shading/ObjectShader.h"

namespace raytracer {
  using namespace vectorization;
  using namespace primitives;

  class SceneObject : public ObjectShader {
  public:
    SceneObject(std::string id);

    virtual ~SceneObject();

    const std::string &getId() const;

    const Form *const getForm() const;

    void setForm(const Form *const value);

    const Float findNearestIntersection(
        const RayCast &rayCast, const FacetIntersection *const originIntersection, FacetIntersection &intersectionOut
    ) const;

    const Float findAnyIntersection(
        const RayCast &rayCast, const FacetIntersection *const originIntersection, FacetIntersection &intersectionOut
    ) const;

    const Float getIndividualIntersectionCosts() const {
      return form ? form->getIndividualIntersectionCosts() : One<Float>();
    }

  protected:
    AxisAlignedBoundingBox bounding;

    const Form *form;

    std::string id;

    void updateBounding();

    const AxisAlignedBoundingBox includeInBounding(const AxisAlignedBoundingBox &aabb) const;

    const bool overlaps(const AxisAlignedBoundingBox &aabb) const;

    const Float transformIntersection(
        const RayCast &rayCast, const FacetIntersection &facetIntersection, FacetIntersection &intersectionOut
    ) const;
  };
}
