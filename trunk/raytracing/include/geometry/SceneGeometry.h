#pragma once

#include "geometry/partitioning/KDTreeNode.h"
#include "geometry/partitioning/KDTreeTraverser.h"
#include "geometry/SceneIntersection.h"

#include <vector>

namespace raytracer
{

    class SceneGeometry : public Intersectable<Raycast, SceneIntersection>, protected GeometryNodesTraverser < SceneIntersection >
    {
    public:

        SceneGeometry();

        explicit SceneGeometry(const KDTreeTraverser<SceneIntersection>*const treeTraverser);

        virtual ~SceneGeometry();

        void clearSceneGraph();

        const Float findNearestIntersection(const Raycast & raycast, const SceneIntersection * const originIntersection, SceneIntersection & intersectionOut) const;

        const Float findAnyIntersection(const Raycast & raycast, const SceneIntersection * const originIntersection, SceneIntersection & intersectionOut) const;

        virtual void buildSceneGraph() = 0;

    protected:

        const KDTreeTraverser<SceneIntersection> *treeTraverser;

        const KDTreeRoot *sceneGraph;

        PGeometryNodeList finiteSceneObjectsAsGeometryNodes;

        PGeometryNodeList infiniteSceneObjectsAsGeometryNodes;

        const Float findNearestIntersection(
            const PGeometryNodeList & geometryNodes,
            const Raycast & raycast,
            const SceneIntersection * const originIntersection,
            SceneIntersection & intersectionOut) const;

        const Float findAnyIntersection(
            const PGeometryNodeList & geometryNodes,
            const Raycast & raycast,
            const SceneIntersection * const originIntersection,
            SceneIntersection & intersectionOut) const;
    };

}
