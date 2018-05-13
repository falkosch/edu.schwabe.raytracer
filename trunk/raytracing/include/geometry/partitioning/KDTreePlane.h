#pragma once

#include <primitives/AxisAlignedBoundingBox.h>

namespace raytracer
{

	struct KDTreePlane
	{
		// axial-distance from min to point on split-axis; range (0.0, 1.0)
		Float t;

		// split-axis
		ASizeT splitAxis;

		ALIGNED_ALLOCATORS(__alignof(KDTreePlane));

		KDTreePlane();

		KDTreePlane(const Float t, const ASizeT splitAxis);

		const Float4 splitMax(const Float4 & min, const Float4 & max) const;

		const Float4 splitMin(const Float4 & min, const Float4 & max) const;

		const Float4 splitMax(const AxisAlignedBoundingBox & aabb) const;

		const Float4 splitMin(const AxisAlignedBoundingBox & aabb) const;
	};

}
