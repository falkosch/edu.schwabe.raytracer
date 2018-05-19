#include "primitives/Facet.h"

using namespace vectorization;

namespace primitives
{

    Facet::Facet()
        :
        v0(),
        v1(),
        v2()
    { }

    Facet::Facet(const Float4 & v)
        :
        v0(v), v1(v), v2(v)
    { }

    Facet::Facet(
        const Float4 & v0In,
        const Float4 & v1In,
        const Float4 & v2In)
        :
        v0(v0In),
        v1(v1In),
        v2(v2In)
    { }

	const AxisAlignedBoundingBox bounding(const Facet & facet)
	{
		return extendBy(AxisAlignedBoundingBox(facet.v0, facet.v1), facet.v2);
	}

    const Facet operator+(const Facet & a, const Float4 & b)
    {
        return Facet(a.v0 + b, a.v1 + b, a.v2 + b);
    }

    const Facet operator-(const Facet & a, const Float4 & b)
    {
        return Facet(a.v0 - b, a.v1 - b, a.v2 - b);
    }

}
