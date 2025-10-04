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

    ~SceneGeometry() override;

    void clearSceneGraph();

    Float findNearestIntersection(
        const RayCast &rayCast, const SceneIntersection *originIntersection, SceneIntersection &intersectionOut
    ) const override;

    Float findAnyIntersection(
        const RayCast &rayCast, const SceneIntersection *originIntersection, SceneIntersection &intersectionOut
    ) const override;

    virtual void buildSceneGraph() = 0;

  protected:
    const KDTreeTraverser<SceneIntersection> *treeTraverser;

    const KDTreeRoot *sceneGraph;

    PGeometryNodeList finiteSceneObjectsAsGeometryNodes;

    PGeometryNodeList infiniteSceneObjectsAsGeometryNodes;

    Float findNearestIntersection(
        const PGeometryNodeList &geometryNodes, const RayCast &rayCast, const SceneIntersection *originIntersection,
        SceneIntersection &intersectionOut
    ) const override;

    Float findAnyIntersection(
        const PGeometryNodeList &geometryNodes, const RayCast &rayCast, const SceneIntersection *originIntersection,
        SceneIntersection &intersectionOut
    ) const override;
  };
}
