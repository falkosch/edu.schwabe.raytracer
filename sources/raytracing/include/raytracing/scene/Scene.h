#pragma once

#include "../geometry/partitioning/KDTreeBalancer.h"
#include "../geometry/partitioning/KDTreeTraverser.h"
#include "../shading/SceneShader.h"
#include "SceneObject.h"

namespace raytracer {
  using namespace vectorization;
  using namespace primitives;

  class Scene : public SceneShader {
  public:
    typedef std::vector<std::unique_ptr<SceneObject>> SceneList;

    Scene();

    Scene(
        std::unique_ptr<const KDTreeTraverser<SceneIntersection>> treeTraverser,
        std::unique_ptr<const KDTreeBalancer> treeBalancer
    );

    virtual ~Scene();

    const SceneList &getSceneObjects() const;

    SceneList &getSceneObjects();

    void buildSceneGraph();

  protected:
    SceneList sceneObjects;

    std::unique_ptr<const KDTreeBalancer> treeBalancer;
  };
}
