#pragma once

#include "geometry/partitioning/KDTreeBalancer.h"

#include <algorithm>

namespace raytracer
{

    class SamplingKDTreeBalancer : public KDTreeBalancer
    {
    public:

        virtual ~SamplingKDTreeBalancer();

    protected:

        static const AxisAlignedBoundingBox geometryNodeBox(const GeometryNode & node);

        static const KDTreePlane sampleSplittingPlane(
            const ASizeT axis,
            const AxisAlignedBoundingBox & bounding,
            const Float4 & sample);

        static const KDTreePlane sampleSplittingPlane(
            const ASizeT axis,
            const Float4 & offset,
            const Float4 & normalization,
            const Float4 & sample);

        static const KDTreePlane sampleSplittingPlane(
            const ASizeT axis,
            const Float offset,
            const Float normalization,
            const Float sample);

        static const Float4 geometryNodeMinimumPredicate(const GeometryNode & node);

        static const Float4 geometryNodeMaximumPredicate(const GeometryNode & node);

        static const PGeometryNodeList geometrySortByMinimum(const ASizeT axis, const PGeometryNodeList & geometry);

        static const PGeometryNodeList geometrySortByMaximum(const ASizeT axis, const PGeometryNodeList & geometry);

        template <typename GeometryComparerFunc>
        static const PGeometryNodeList geometrySort(const PGeometryNodeList & geometry)
        {
            // creates new std::vector / copies the contained array
            PGeometryNodeList toBeSortedGeometry = geometry;
            std::stable_sort(toBeSortedGeometry.begin(), toBeSortedGeometry.end(), GeometryComparerFunc());
            return toBeSortedGeometry;
        }

    private:

        template <ASizeT Axis>
        struct GeometryMinimumComparer
        {
            const bool operator()(const GeometryNode * const left, const GeometryNode * const right) const
            {
                return !!component<Axis>(geometryNodeMinimumPredicate(*left) < geometryNodeMinimumPredicate(*right));
            }
        };

        template <ASizeT Axis>
        struct GeometryMaximumComparer
        {
            const bool operator()(const GeometryNode * const left, const GeometryNode * const right) const
            {
                return !!component<Axis>(geometryNodeMaximumPredicate(*left) < geometryNodeMaximumPredicate(*right));
            }
        };
    };

}
