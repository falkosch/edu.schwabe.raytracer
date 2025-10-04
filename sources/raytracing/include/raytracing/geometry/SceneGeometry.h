#pragma once

#include "SceneIntersection.h"
#include "partitioning/KDTreeNode.h"
#include "partitioning/KDTreeTraverser.h"

#include <vector>

namespace raytracer {
  using namespace vectorization;
  using namespace primitives;

  class SceneGeometry : public Intersectable<RayCast, SceneIntersection>,
                        protected GeometryNodesTraverser<SceneIntersection> {
  public:
    SceneGeometry();

    explicit SceneGeometry(const KDTreeTraverser<SceneIntersection> *const treeTraverser);

    virtual ~SceneGeometry();

    void clearSceneGraph();

    const Float findNearestIntersection(
        const RayCast &rayCast, const SceneIntersection *const originIntersection, SceneIntersection &intersectionOut
    ) const;

    const Float findAnyIntersection(
        const RayCast &rayCast, const SceneIntersection *const originIntersection, SceneIntersection &intersectionOut
    ) const;

    virtual void buildSceneGraph() = 0;

  protected:
    const KDTreeTraverser<SceneIntersection> *treeTraverser;

    const KDTreeRoot *sceneGraph;

    PGeometryNodeList finiteSceneObjectsAsGeometryNodes;

    PGeometryNodeList infiniteSceneObjectsAsGeometryNodes;

    const Float findNearestIntersection(
        const PGeometryNodeList &geometryNodes, const RayCast &rayCast,
        const SceneIntersection *const originIntersection, SceneIntersection &intersectionOut
    ) const;

    const Float findAnyIntersection(
        const PGeometryNodeList &geometryNodes, const RayCast &rayCast,
        const SceneIntersection *const originIntersection, SceneIntersection &intersectionOut
    ) const;
  };
}
