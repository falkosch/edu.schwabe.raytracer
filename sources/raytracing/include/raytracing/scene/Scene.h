#pragma once

#include "../geometry/partitioning/KDTreeBalancer.h"
#include "../geometry/partitioning/KDTreeTraverser.h"
#include "../shading/SceneShader.h"
#include "SceneObject.h"

#include <functional>

namespace raytracer
{
    using namespace vectorization;
    using namespace primitives;

    class Scene : public SceneShader
    {
    public:
        typedef std::vector<std::unique_ptr<SceneObject>> SceneList;

        Scene();

        Scene(
            std::unique_ptr<const KDTreeTraverser<SceneIntersection>> treeTraverser,
            std::unique_ptr<const KDTreeBalancer> treeBalancer
        );

        ~Scene() override;

        const SceneList& getSceneObjects() const;

        SceneList& getSceneObjects();

        void buildSceneGraph() override;

        void setTreeBalancer(std::unique_ptr<const KDTreeBalancer> balancer);

        void setMeshTreeBalancer(const std::function<std::unique_ptr<const KDTreeBalancer>()>& balancerFactory) const;

    protected:
        SceneList sceneObjects;

        std::unique_ptr<const KDTreeBalancer> treeBalancer;
    };
}
