#include "ManualTests.h"
#include "stdafx.h"

namespace raytracerui
{

	ARCH_NOINLINE void collisionTest() {
		auto v0 = Float4(-0.156548679, 0.286460012, -0.441803336, 1.00000000);
		auto v1 = Float4(-0.250403672, 0.286460012, -0.443586677, 1.00000000);
		auto v2 = Float4(-0.240178674, 0.286460012, -0.417760015, 1.00000000);
		auto facet = Facet(v0, v1, v2);
		auto n = new MeshGeometryNode(0, facet);

		auto min = Float4(-0.199075446, 0.254602253, -0.460181773, 1.00000000);
		auto max = Float4(-0.159260362, 0.286460012, -0.437499464, 1.00000000);
		auto b = AxisAlignedBoundingBox(min, max);

		n->overlaps(b);

		delete n;
	}

	ARCH_NOINLINE void staticForTest() {
		const UTVector<100, Float> a = UTVector<100, Float>();
		const UTVector<100, Float> b = UTVector<100, Float>();

		auto c = a + b;
		std::cout << c << std::endl;
	}

	void ManualTests::operator()() const
	{
		collisionTest();
		staticForTest();
	}

}
