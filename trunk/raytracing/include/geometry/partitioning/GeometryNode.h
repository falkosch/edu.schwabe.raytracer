#pragma once

#include "geometry/FacetIntersection.h"

#include <primitives/AxisAlignedBoundingBox.h>

#include <vector>

namespace raytracer
{

	struct GeometryNode
		: public Intersectable < Raycast, FacetIntersection >
	{
		virtual ~GeometryNode();

		virtual void includeInBounding(AxisAlignedBoundingBox & aabb) const = 0;

		virtual const bool overlaps(const AxisAlignedBoundingBox & aabb) const = 0;

		virtual const Float getIndividualIntersectionCosts() const = 0;
	};

	typedef std::vector<GeometryNode*, AlignedAllocator<GeometryNode*>> PGeometryNodeList;

}
