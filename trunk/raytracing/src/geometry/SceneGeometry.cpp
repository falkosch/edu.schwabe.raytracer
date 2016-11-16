#include "../stdafx.h"

#include "geometry/SceneGeometry.h"

namespace raytracer
{

    SceneGeometry::SceneGeometry()
        :
        treeTraverser(),
        sceneGraph(),
        finiteSceneObjectsAsGeometryNodes(),
        infiniteSceneObjectsAsGeometryNodes()
    { }

    SceneGeometry::SceneGeometry(const KDTreeTraverser<SceneIntersection> * const treeTraverserIn)
        :
        treeTraverser(treeTraverserIn),
        sceneGraph(),
        finiteSceneObjectsAsGeometryNodes(),
        infiniteSceneObjectsAsGeometryNodes()
    { }

    SceneGeometry::~SceneGeometry()
    {
        clearSceneGraph();
        if (treeTraverser)
        {
            delete(treeTraverser);
        }
        finiteSceneObjectsAsGeometryNodes.clear();
        infiniteSceneObjectsAsGeometryNodes.clear();
    }

    void SceneGeometry::clearSceneGraph()
    {
        if (sceneGraph)
        {
            delete sceneGraph;
            sceneGraph = nullptr;
        }
    }

    const Float SceneGeometry::findNearestIntersection(const Raycast & r, const SceneIntersection * const originIntersection, SceneIntersection & intersectionOut) const
    {
        if (outOfReach(r, Zero<Float>()))
        {
            return r.maxDistance;
        }

        Raycast tr = r;
        if (sceneGraph && treeTraverser)
        {
            tr.maxDistance = treeTraverser->findNearestIntersection(*this, *sceneGraph, tr, originIntersection, intersectionOut);
        }
        else
        {
            tr.maxDistance = findNearestIntersection(finiteSceneObjectsAsGeometryNodes, tr, originIntersection, intersectionOut);
        }

        return findNearestIntersection(infiniteSceneObjectsAsGeometryNodes, tr, originIntersection, intersectionOut);
    }

    const Float SceneGeometry::findAnyIntersection(const Raycast & r, const SceneIntersection * const originIntersection, SceneIntersection & intersectionOut) const
    {
        if (outOfReach(r, Zero<Float>()))
        {
            return r.maxDistance;
        }

        Float d;
        if (sceneGraph && treeTraverser)
        {
            d = treeTraverser->findAnyIntersection(*this, *sceneGraph, r, originIntersection, intersectionOut);
        }
        else
        {
            d = findAnyIntersection(finiteSceneObjectsAsGeometryNodes, r, originIntersection, intersectionOut);
        }

        if (outOfReach(r, d))
        {
            return findAnyIntersection(infiniteSceneObjectsAsGeometryNodes, r, originIntersection, intersectionOut);
        }

        return d;
    }

    inline void transformIntersection(const Raycast & raycast, const ObjectGeometry * const objectGeometry, SceneIntersection & intersectionOut)
    {
        const Float4 NdotI = dotv(intersectionOut.smoothedNormal, raycast.ray.direction);
        intersectionOut.smoothedNdotI = NdotI;
        intersectionOut.reflectedDirection = reflect(raycast.ray.direction, intersectionOut.smoothedNormal, NdotI);
        intersectionOut.object = objectGeometry;
    }

    const Float SceneGeometry::findNearestIntersection(
        const PGeometryNodeList & geometryNodes,
        const Raycast & r,
        const SceneIntersection * const originIntersection, 
        SceneIntersection & intersectionOut) const
    {
        // iterate through all SceneObjects
        Raycast tr = r;

        for (PGeometryNodeList::const_iterator it = geometryNodes.cbegin(); it != geometryNodes.cend(); ++it)
        {
            const ObjectGeometry * const objectGeometry = static_cast<const ObjectGeometry * const>(*it);

            // check for intersections
            const Float t = objectGeometry->findNearestIntersection(tr, originIntersection, intersectionOut);
            if (outOfReach(tr, t)) continue;

            tr.maxDistance = t;
            transformIntersection(tr, objectGeometry, intersectionOut);
        }

        return tr.maxDistance;
    }

    const Float SceneGeometry::findAnyIntersection(
        const PGeometryNodeList & geometryNodes,
        const Raycast & r,
        const SceneIntersection * const originIntersection,
        SceneIntersection & intersectionOut) const
    {
        // iterate through all SceneObjects
        for (PGeometryNodeList::const_iterator it = geometryNodes.cbegin(); it != geometryNodes.cend(); ++it)
        {
            const ObjectGeometry * const objectGeometry = static_cast<const ObjectGeometry * const>(*it);

            // check for intersections
            const Float t = objectGeometry->findAnyIntersection(r, originIntersection, intersectionOut);
            if (outOfReach(r, t)) continue;

            transformIntersection(r, objectGeometry, intersectionOut);
            return t;
        }
        return r.maxDistance;
    }

}
