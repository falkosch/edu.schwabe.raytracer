#include "../stdafx.h"

#include "scene/Scene.h"

// disables the generation of a BVH for the scene objects (the scene tree)
//#define DISABLE_BVH

namespace raytracer
{

    Scene::Scene()
        :
        SceneShader(),
        sceneObjects(),
        treeBalancer()
    { }

    Scene::Scene(
        const KDTreeTraverser<SceneIntersection> * const treeTraverserIn,
        const KDTreeBalancer * const treeBalancerIn)
        :
        SceneShader(treeTraverserIn),
        sceneObjects(),
        treeBalancer(treeBalancerIn)
    { }

    Scene::~Scene()
    {
        if (treeBalancer)
        {
            delete treeBalancer;
        }
    }

    Scene::SceneList & Scene::getSceneObjects()
    {
        return sceneObjects;
    }

    const Scene::SceneList & Scene::getSceneObjects() const
    {
        return sceneObjects;
    }

    void Scene::buildSceneGraph()
    {
        clearSceneGraph();
        finiteSceneObjectsAsGeometryNodes.clear();
        infiniteSceneObjectsAsGeometryNodes.clear();
        for (SceneList::iterator it = sceneObjects.begin(); it != sceneObjects.end(); ++it)
        {
            SceneObject * const sceneObject = *it;
            if (sceneObject->getForm()->isInfinite())
            {
                infiniteSceneObjectsAsGeometryNodes.push_back(sceneObject);
            }
            else
            {
                finiteSceneObjectsAsGeometryNodes.push_back(sceneObject);
            }
        }

#ifndef DISABLE_BVH
        if (treeBalancer)
        {
            std::cout << "Building culling tree for scene ... ";
            sceneGraph = treeBalancer->build(finiteSceneObjectsAsGeometryNodes);
            std::cout << "done" << std::endl;
        }
#endif
    }

}
