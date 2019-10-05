#pragma once

#include "../geometry/partitioning/KDTreeTraverser.h"
#include "../geometry/partitioning/KDTreeBalancer.h"
#include "../shading/SceneShader.h"
#include "SceneObject.h"

namespace raytracer
{
    using namespace vectorization;
    using namespace primitives;

    class Scene : public SceneShader
    {
    public:

        typedef std::vector<SceneObject *, AlignedAllocator<SceneObject *>> SceneList;

        ALIGNED_ALLOCATORS(__alignof(Scene));

        Scene();

        Scene(const KDTreeTraverser<SceneIntersection> * const treeTraverser, const KDTreeBalancer * const treeBalancer);

        virtual ~Scene();

        const SceneList & getSceneObjects() const;

        SceneList & getSceneObjects();

        void buildSceneGraph();

    protected:

        SceneList sceneObjects;

        const KDTreeBalancer * treeBalancer;
    };
}
