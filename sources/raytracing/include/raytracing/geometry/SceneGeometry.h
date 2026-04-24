#pragma once

#include "SceneIntersection.h"
#include "partitioning/KDTreeNode.h"
#include "partitioning/KDTreeTraverser.h"

#include <memory>
#include <vector>

namespace raytracer {
  using namespace vectorization;
  using namespace primitives;

  class SceneGeometry : public Intersectable<RayCast, SceneIntersection>,
                        protected GeometryNodesTraverser<SceneIntersection> {
  public:
    SceneGeometry();

    explicit SceneGeometry(std::unique_ptr<const KDTreeTraverser<SceneIntersection>> treeTraverser);

    ~SceneGeometry() override;

    void clearSceneGraph();

    Float findNearestIntersection(
        const RayCast &rayCast, const SceneIntersection *originIntersection, SceneIntersection &intersectionOut
    ) const override;

    Float findAnyIntersection(
        const RayCast &rayCast, const SceneIntersection *originIntersection, SceneIntersection &intersectionOut
    ) const override;

    const KDTreeRoot *getSceneGraph() const;

    virtual void buildSceneGraph() = 0;

  protected:
    std::unique_ptr<const KDTreeTraverser<SceneIntersection>> treeTraverser;

    std::unique_ptr<const KDTreeRoot> sceneGraph;

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
