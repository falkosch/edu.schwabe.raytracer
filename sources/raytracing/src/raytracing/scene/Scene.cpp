#include "raytracing/scene/Scene.h"
#include "../../stdafx.h"

#include <logging.h>

// disables the generation of a BVH for the scene objects (the scene tree)
// #define DISABLE_BVH

static const auto Log = logging::scope("Scene");

namespace raytracer {
  Scene::Scene() : SceneShader(), sceneObjects(), treeBalancer() {
  }

  Scene::Scene(
      std::unique_ptr<const KDTreeTraverser<SceneIntersection>> treeTraverserIn,
      std::unique_ptr<const KDTreeBalancer> treeBalancerIn
  )
      : SceneShader(std::move(treeTraverserIn)), sceneObjects(), treeBalancer(std::move(treeBalancerIn)) {
  }

  Scene::~Scene() = default;

  Scene::SceneList &Scene::getSceneObjects() {
    return sceneObjects;
  }

  const Scene::SceneList &Scene::getSceneObjects() const {
    return sceneObjects;
  }

  void Scene::buildSceneGraph() {
    clearSceneGraph();
    finiteSceneObjectsAsGeometryNodes.clear();
    infiniteSceneObjectsAsGeometryNodes.clear();
    for (const auto &sceneObject : sceneObjects) {
      if (sceneObject->getForm()->isInfinite()) {
        infiniteSceneObjectsAsGeometryNodes.push_back(sceneObject.get());
      } else {
        finiteSceneObjectsAsGeometryNodes.push_back(sceneObject.get());
      }
    }

#ifndef DISABLE_BVH
    if (treeBalancer) {
      Log.info([] { return "Building culling tree for scene ..."; });
      sceneGraph = treeBalancer->build(finiteSceneObjectsAsGeometryNodes);
      Log.info([] { return "done"; });
    }
#endif
  }
}
