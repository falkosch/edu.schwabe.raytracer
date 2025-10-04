#pragma once

#include "../geometry/forms/Form.h"
#include "../shading/ObjectShader.h"

namespace raytracer {
  using namespace vectorization;
  using namespace primitives;

  class SceneObject : public ObjectShader {
  public:
    explicit SceneObject(std::string id);

    ~SceneObject() override;

    const std::string &getId() const;

    const Form *getForm() const;

    void setForm(const Form *value);

    Float findNearestIntersection(
        const RayCast &rayCast, const FacetIntersection *originIntersection, FacetIntersection &intersectionOut
    ) const override;

    Float findAnyIntersection(
        const RayCast &rayCast, const FacetIntersection *originIntersection, FacetIntersection &intersectionOut
    ) const override;

    Float getIndividualIntersectionCosts() const override {
      return form ? form->getIndividualIntersectionCosts() : One<Float>();
    }

  protected:
    AxisAlignedBoundingBox bounding;
    const Form *form;
    std::string id;

    void updateBounding() override;

    AxisAlignedBoundingBox includeInBounding(const AxisAlignedBoundingBox &aabb) const override;

    bool overlaps(const AxisAlignedBoundingBox &aabb) const override;

    Float transformIntersection(
        const RayCast &rayCast, const FacetIntersection &facetIntersection, FacetIntersection &intersectionOut
    ) const;
  };
}
