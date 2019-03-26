#pragma once

#include "../geometry/forms/Form.h"
#include "../shading/ObjectShader.h"

namespace raytracer
{
	using namespace vectorization;
	using namespace primitives;

	class SceneObject : public ObjectShader
	{
	public:

		ALIGNED_ALLOCATORS(__alignof(SceneObject));

		SceneObject(const std::string & id);

		virtual ~SceneObject();

		const std::string & getId() const;

		const Form * const getForm() const;

		void setForm(const Form * const value);

		const Float findNearestIntersection(
			const Raycast & raycast,
			const FacetIntersection * const originIntersection,
			FacetIntersection & intersectionOut
		) const;

		const Float findAnyIntersection(
			const Raycast & raycast,
			const FacetIntersection * const originIntersection,
			FacetIntersection & intersectionOut
		) const;

		const Float getIndividualIntersectionCosts() const
		{
			return form ? form->getIndividualIntersectionCosts() : One<Float>();
		}

	protected:

		AxisAlignedBoundingBox bounding;

		const Form * form;

		std::string id;

		void updateBounding();

		void includeInBounding(AxisAlignedBoundingBox & aabb) const;

		const bool overlaps(const AxisAlignedBoundingBox & aabb) const;

		const Float transformIntersection(const Raycast & raycast, const FacetIntersection & facetIntersection, FacetIntersection & intersectionOut) const;
	};

}
