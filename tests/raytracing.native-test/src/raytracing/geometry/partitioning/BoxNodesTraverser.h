#pragma once

#include "raytracing_tests.h"

namespace raytracer::test {
  // A minimal GeometryNodesTraverser over Box leaves: at each leaf, iterate the boxes and pick the
  // nearest hit. This lets us drive a NaiveKDTreeTraverser end-to-end without going through Mesh.
  class BoxNodesTraverser : public GeometryNodesTraverser<FacetIntersection> {
  public:
    Float findNearestIntersection(
        GeometryNodeSpan geometryNodes, const RayCast &rayCast, const FacetIntersection *const origin,
        FacetIntersection &intersectionOut
    ) const override {
      Float nearest = rayCast.maxDistance;
      for (auto *node : geometryNodes) {
        const auto *box = static_cast<const Box *>(node);
        if (origin && origin->node == box) {
          continue;
        }
        FacetIntersection candidate{};
        RayCast scoped{rayCast};
        scoped.maxDistance = nearest;
        const Float d = box->findNearestIntersection(scoped, origin, candidate);
        if (d < nearest) {
          nearest = d;
          intersectionOut = candidate;
          intersectionOut.node = box;
        }
      }
      return nearest;
    }

    Float findAnyIntersection(
        GeometryNodeSpan geometryNodes, const RayCast &rayCast, const FacetIntersection *const origin,
        FacetIntersection &intersectionOut
    ) const override {
      for (auto *node : geometryNodes) {
        const auto *box = static_cast<const Box *>(node);
        if (origin && origin->node == box) {
          continue;
        }
        FacetIntersection candidate{};
        const Float d = box->findAnyIntersection(rayCast, origin, candidate);
        if (d < rayCast.maxDistance) {
          intersectionOut = candidate;
          intersectionOut.node = box;
          return d;
        }
      }
      return rayCast.maxDistance;
    }
  };
}
