#include "../../../stdafx.h"

#include "geometry/partitioning/treebalancer/SamplingKDTreeBalancer.h"

namespace raytracer
{

    SamplingKDTreeBalancer::~SamplingKDTreeBalancer() { }

    const AxisAlignedBoundingBox SamplingKDTreeBalancer::geometryNodeBox(const GeometryNode & node)
    {
        AxisAlignedBoundingBox b = AxisAlignedBoundingBox();
        node.includeInBounding(b);
        return b;
    }

    const Float4 SamplingKDTreeBalancer::geometryNodeMinimumPredicate(const GeometryNode & node)
    {
        return geometryNodeBox(node).minimum;
    }

    const Float4 SamplingKDTreeBalancer::geometryNodeMaximumPredicate(const GeometryNode & node)
    {
        return geometryNodeBox(node).maximum;
    }

    const PGeometryNodeList SamplingKDTreeBalancer::geometrySortByMinimum(const ASizeT axis, const PGeometryNodeList & geometry)
    {
        switch (axis)
        {
        case VectorIndices::X:
            return geometrySort<GeometryMinimumComparer<VectorIndices::X>>(geometry);
        case VectorIndices::Y:
            return geometrySort<GeometryMinimumComparer<VectorIndices::Y>>(geometry);
        default:
            return geometrySort<GeometryMinimumComparer<VectorIndices::Z>>(geometry);
        }
    }

    const PGeometryNodeList SamplingKDTreeBalancer::geometrySortByMaximum(const ASizeT axis, const PGeometryNodeList & geometry)
    {
        switch (axis)
        {
        case VectorIndices::X:
            return geometrySort<GeometryMaximumComparer<VectorIndices::X>>(geometry);
        case VectorIndices::Y:
            return geometrySort<GeometryMaximumComparer<VectorIndices::Y>>(geometry);
        default:
            return geometrySort<GeometryMaximumComparer<VectorIndices::Z>>(geometry);
        }
    }

    const KDTreePlane SamplingKDTreeBalancer::sampleSplittingPlane(
        const ASizeT axis,
        const AxisAlignedBoundingBox & bounding,
        const Float4 & sample)
    {
        return sampleSplittingPlane(axis, bounding.minimum, extents(bounding), sample);
    }

    const KDTreePlane SamplingKDTreeBalancer::sampleSplittingPlane(
        const ASizeT axis,
        const Float4 & offset,
        const Float4 & normalization,
        const Float4 & sample)
    {
        return sampleSplittingPlane(axis, offset[axis], normalization[axis], sample[axis]);
    }

    const KDTreePlane SamplingKDTreeBalancer::sampleSplittingPlane(
        const ASizeT axis,
        const Float offset,
        const Float normalization,
        const Float sample)
    {
        return KDTreePlane((sample - offset) / normalization, axis);
    }

}
