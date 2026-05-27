#include "raytracing/scene/Scene.h"
#include "../../stdafx.h"

#include "raytracing/geometry/forms/meshes/Mesh.h"

#include <logging.h>
#include <sstream>

// disables the generation of a BVH for the scene objects (the scene tree)
// #define DISABLE_BVH

static const auto Log = logging::scope("Scene");

namespace raytracer
{
    Scene::Scene() : SceneShader(), sceneObjects(), treeBalancer()
    {
    }

    Scene::Scene(
        std::unique_ptr<const KDTreeTraverser<SceneIntersection>> treeTraverserIn,
        std::unique_ptr<const KDTreeBalancer> treeBalancerIn
    )
        : SceneShader(std::move(treeTraverserIn)), sceneObjects(), treeBalancer(std::move(treeBalancerIn))
    {
    }

    Scene::~Scene() = default;

    Scene::SceneList& Scene::getSceneObjects()
    {
        return sceneObjects;
    }

    const Scene::SceneList& Scene::getSceneObjects() const
    {
        return sceneObjects;
    }

    void Scene::setTreeBalancer(std::unique_ptr<const KDTreeBalancer> balancer)
    {
        treeBalancer = std::move(balancer);
        buildSceneGraph();
    }

    void Scene::setMeshTreeBalancer(const std::function<std::unique_ptr<const KDTreeBalancer>()>& balancerFactory) const
    {
        for (const auto& sceneObject : sceneObjects)
        {
            if (auto* mesh = dynamic_cast<Mesh*>(sceneObject->getForm()))
            {
                mesh->setTreeBalancer(balancerFactory());
            }
        }
    }

    void Scene::buildSceneGraph()
    {
        clearSceneGraph();
        finiteSceneObjectsAsGeometryNodes.clear();
        infiniteSceneObjectsAsGeometryNodes.clear();
        for (const auto& sceneObject : sceneObjects)
        {
            if (sceneObject->getForm()->isInfinite())
            {
                infiniteSceneObjectsAsGeometryNodes.push_back(sceneObject.get());
            }
            else
            {
                finiteSceneObjectsAsGeometryNodes.push_back(sceneObject.get());
            }
        }

#ifndef DISABLE_BVH
        if (treeBalancer)
        {
            Log.info([n = finiteSceneObjectsAsGeometryNodes.size()]
            {
                return "Building culling tree for scene with " + std::to_string(n) + " finite scene objects";
            });
            sceneGraph = treeBalancer->build(finiteSceneObjectsAsGeometryNodes);
            if (sceneGraph)
            {
                Log.info([d = sceneGraph->depth(), n = sceneGraph->nodeCount()]
                {
                    return "Tree depth=" + std::to_string(d) + " nodes=" + std::to_string(n);
                });
            }
            Log.info([] { return "done"; });
        }
#endif
    }
}
