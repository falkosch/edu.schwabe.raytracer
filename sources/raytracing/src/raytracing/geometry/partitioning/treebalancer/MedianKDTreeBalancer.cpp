#include "raytracing/geometry/partitioning/treebalancer/MedianKDTreeBalancer.h"
#include "../../../../stdafx.h"

#include <cassert>

namespace raytracer {
  MedianKDTreeBalancer::MedianKDTreeBalancer() : SamplingKDTreeBalancer() {
  }

  MedianKDTreeBalancer::~MedianKDTreeBalancer() = default;

  const KDTreePlane MedianKDTreeBalancer::
      findSplitter(const KDTreeBuildParameters &, const ASizeT, const AxisAlignedBoundingBox &bounding, const PGeometryNodeList &geometry, const KDTreePlane *)
          const {
    const Float4 boundingExtents = extents(bounding);
    const ASizeT maxAxis = argmax3(boundingExtents);
    const PGeometryNodeList sortedGeometry = geometrySortByMinimum(maxAxis, geometry);
    const ASizeT geometrySize = sortedGeometry.size();
    assert(geometrySize > 1);

    if (geometrySize & One<ASizeT>()) {
      // list-size isn't even, median is at "geometrySize / 2 + 1"
      return sampleSplittingPlane(
          maxAxis, bounding.minimum, boundingExtents,
          geometryNodeMinimumPredicate(*sortedGeometry[(geometrySize >> 1) + One<ASizeT>()])
      );
    }

    // list-size is even, median is average of nodes at "geometrySize / 2" and "geometrySize / 2 + 1"
    const ASizeT geometrySizeHalf = geometrySize >> 1;
    const Float4 medianCoords =
        Half<Float>()
        * (geometryNodeMinimumPredicate(*sortedGeometry[geometrySizeHalf])
           + geometryNodeMinimumPredicate(*sortedGeometry[geometrySizeHalf + One<ASizeT>()]));

    return sampleSplittingPlane(maxAxis, bounding.minimum, boundingExtents, medianCoords);
  }
}