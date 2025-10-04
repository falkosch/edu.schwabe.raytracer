#include "raytracing/geometry/SceneGeometry.h"
#include "../../stdafx.h"

namespace raytracer {
  SceneGeometry::SceneGeometry()
      : treeTraverser(), sceneGraph(), finiteSceneObjectsAsGeometryNodes(), infiniteSceneObjectsAsGeometryNodes() {
  }

  SceneGeometry::SceneGeometry(const KDTreeTraverser<SceneIntersection> *const treeTraverserIn)
      : treeTraverser(treeTraverserIn), sceneGraph(), finiteSceneObjectsAsGeometryNodes(),
        infiniteSceneObjectsAsGeometryNodes() {
  }

  SceneGeometry::~SceneGeometry() {
    clearSceneGraph();
    if (treeTraverser) {
      delete treeTraverser;
    }
    finiteSceneObjectsAsGeometryNodes.clear();
    infiniteSceneObjectsAsGeometryNodes.clear();
  }

  void SceneGeometry::clearSceneGraph() {
    if (sceneGraph) {
      delete sceneGraph;
      sceneGraph = nullptr;
    }
  }

  Float SceneGeometry::findNearestIntersection(
      const RayCast &r, const SceneIntersection *const originIntersection, SceneIntersection &intersectionOut
  ) const {
    if (outOfReach(r, Zero<Float>())) {
      return r.maxDistance;
    }

    RayCast tr = r;
    if (sceneGraph && treeTraverser) {
      tr.maxDistance =
          treeTraverser->findNearestIntersection(*this, *sceneGraph, tr, originIntersection, intersectionOut);
    } else {
      tr.maxDistance =
          findNearestIntersection(finiteSceneObjectsAsGeometryNodes, tr, originIntersection, intersectionOut);
    }

    return findNearestIntersection(infiniteSceneObjectsAsGeometryNodes, tr, originIntersection, intersectionOut);
  }

  Float SceneGeometry::findAnyIntersection(
      const RayCast &r, const SceneIntersection *const originIntersection, SceneIntersection &intersectionOut
  ) const {
    if (outOfReach(r, Zero<Float>())) {
      return r.maxDistance;
    }

    Float d;
    if (sceneGraph && treeTraverser) {
      d = treeTraverser->findAnyIntersection(*this, *sceneGraph, r, originIntersection, intersectionOut);
    } else {
      d = findAnyIntersection(finiteSceneObjectsAsGeometryNodes, r, originIntersection, intersectionOut);
    }

    if (outOfReach(r, d)) {
      return findAnyIntersection(infiniteSceneObjectsAsGeometryNodes, r, originIntersection, intersectionOut);
    }

    return d;
  }

  void transformIntersection(
      const RayCast &rayCast, const ObjectGeometry *const objectGeometry, SceneIntersection &intersectionOut
  ) {
    const Float4 NdotI = dotv(intersectionOut.smoothedNormal, rayCast.ray.direction);
    intersectionOut.smoothedNdotI = NdotI;
    intersectionOut.reflectedDirection = reflect(rayCast.ray.direction, intersectionOut.smoothedNormal, NdotI);
    intersectionOut.object = objectGeometry;
  }

  Float SceneGeometry::findNearestIntersection(
      const PGeometryNodeList &geometryNodes, const RayCast &r, const SceneIntersection *const originIntersection,
      SceneIntersection &intersectionOut
  ) const {
    // iterate through all SceneObjects
    RayCast tr = r;

    for (auto geometryNode : geometryNodes) {
      const auto objectGeometry = dynamic_cast<const ObjectGeometry *const>(geometryNode);

      // check for intersections
      const Float t = objectGeometry->findNearestIntersection(tr, originIntersection, intersectionOut);
      if (outOfReach(tr, t))
        continue;

      tr.maxDistance = t;
      transformIntersection(tr, objectGeometry, intersectionOut);
    }

    return tr.maxDistance;
  }

  Float SceneGeometry::findAnyIntersection(
      const PGeometryNodeList &geometryNodes, const RayCast &r, const SceneIntersection *const originIntersection,
      SceneIntersection &intersectionOut
  ) const {
    // iterate through all SceneObjects
    for (auto geometryNode : geometryNodes) {
      const auto objectGeometry = dynamic_cast<const ObjectGeometry *const>(geometryNode);

      // check for intersections
      const Float t = objectGeometry->findAnyIntersection(r, originIntersection, intersectionOut);
      if (outOfReach(r, t))
        continue;

      transformIntersection(r, objectGeometry, intersectionOut);
      return t;
    }
    return r.maxDistance;
  }
}
