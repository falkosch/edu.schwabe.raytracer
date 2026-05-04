#include "tests/TestScene1.h"
#include "../stdafx.h"

#include "tests/TestLightScene.h"
#include "tests/TestResources.h"

namespace raytracerui
{
    void TestScene1::setup(Scene& scene, Resources& resources)
    {
        TestResources::setup(resources);
        TestLightScene::setup(scene, resources);

        const Float4 vacuum2vacuum = RefractionIndices::refractionEta(RefractionIndices::Vacuum,
                                                                      RefractionIndices::Vacuum);

        // floor
        auto sceneObject = std::make_unique<SceneObject>("floor");
        sceneObject->setForm(std::make_unique<Plane>());
        sceneObject->scale(Float3(1.0f, 1.0f, 1.0f));
        sceneObject->rotate(Float3(-90.f, 0.0f, 0.0f));
        sceneObject->translate(Float3(0.0f, -1.0f, 0.0f));
        sceneObject->scaleTexture(Float2(1.0f, 1.0f));
        sceneObject->rotateTexture(0.0f);
        sceneObject->translateTexture(Float2(0.0f, 0.0f));
        sceneObject->setEmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setDiffusionShader(std::make_unique<HDRImageShader>(*resources.getPPM("seamless_floor_concrete")));
        sceneObject->setReflectanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->
            setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setRoughnessShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 0.0f)));
        sceneObject->setTransmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setRefractionEtaShader(std::make_unique<Resources::ConstMaterialShader>(vacuum2vacuum));
        scene.getSceneObjects().push_back(std::move(sceneObject));

        // back object
        sceneObject = std::make_unique<SceneObject>("back object");
        sceneObject->setForm(std::make_unique<Box>());
        sceneObject->scale(Float3(4.0f, 3.0f, 0.1f));
        sceneObject->rotate(Float3(0.0f, 0.0f, 0.0f));
        sceneObject->translate(Float3(0.0f, 10.5f, -5.f));
        sceneObject->scaleTexture(Float2(4.0f, 4.0f));
        sceneObject->rotateTexture(0.0f);
        sceneObject->translateTexture(Float2(0.0f, 0.0f));
        sceneObject->setEmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setDiffusionShader(std::make_unique<HDRImageShader>(*resources.getPPM("checker")));
        sceneObject->setReflectanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->
            setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setRoughnessShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.5f, 0.5f, 0.5f, 0.0f)));
        sceneObject->setTransmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setRefractionEtaShader(std::make_unique<Resources::ConstMaterialShader>(vacuum2vacuum));
        scene.getSceneObjects().push_back(std::move(sceneObject));

        // middle object
        sceneObject = std::make_unique<SceneObject>("middle object");
        sceneObject->setForm(std::make_unique<Sphere>());
        sceneObject->scale(Float3(0.5f, 0.5f, 0.5f));
        sceneObject->rotate(Float3(0.0f, 0.0f, 0.0f));
        sceneObject->translate(Float3(0.0f, 1.0f, 0.0f));
        sceneObject->scaleTexture(Float2(1.0f, 1.0f));
        sceneObject->rotateTexture(0.0f);
        sceneObject->translateTexture(Float2(0.0f, 0.0f));
        sceneObject->setEmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setDiffusionShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setReflectanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->
            setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setRoughnessShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.25f, 0.25f, 0.25f, 0.0f)));
        sceneObject->setTransmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 0.75f, 0.25f, 0.0f)));
        sceneObject->setRefractionEtaShader(std::make_unique<Resources::ConstMaterialShader>(
            RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Glass)
        ));
        scene.getSceneObjects().push_back(std::move(sceneObject));

        // left object
        sceneObject = std::make_unique<SceneObject>("left object");
        sceneObject->setForm(std::make_unique<Sphere>());
        sceneObject->scale(Float3(1.0f, 1.0f, 1.0f) / 3.0f);
        sceneObject->rotate(Float3(0.0f, 0.0f, 0.0f));
        sceneObject->translate(Float3(-1.0f, 0.0f, 0.0f));
        sceneObject->scaleTexture(Float2(1.0f, 1.0f));
        sceneObject->rotateTexture(0.0f);
        sceneObject->translateTexture(Float2(0.0f, 0.0f));
        sceneObject->setEmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setDiffusionShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setReflectanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.95f, 0.9f, 0.95f, 0.0f)));
        sceneObject->setSpecularShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.95f, 0.9f, 0.95f, 0.0f)));
        sceneObject->setRoughnessShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.15f, 0.15f, 0.15f, 0.0f)));
        sceneObject->setTransmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.25f, 0.25f, 0.4f, 0.0f)));
        sceneObject->setRefractionEtaShader(std::make_unique<Resources::ConstMaterialShader>(
            RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Diamond)
        ));
        scene.getSceneObjects().push_back(std::move(sceneObject));

        // right object
        sceneObject = std::make_unique<SceneObject>("right object");
        sceneObject->setForm(std::make_unique<Box>());
        sceneObject->scale(Float3(1.0f, 1.0f, 1.0f) / 3.0f);
        sceneObject->rotate(Float3(1.0f, 0.0f, 0.0f));
        sceneObject->translate(Float3(1.0f, 0.1f, 0.1f));
        sceneObject->scaleTexture(Float2(1.0f, 1.0f));
        sceneObject->rotateTexture(0.0f);
        sceneObject->translateTexture(Float2(0.0f, 0.0f));
        sceneObject->setEmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setDiffusionShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setReflectanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.1f, 0.1f, 0.1f, 0.0f)));
        sceneObject->
            setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.1f, 0.1f, 0.1f, 0.0f)));
        sceneObject->setRoughnessShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.3f, 0.3f, 0.3f, 0.0f)));
        sceneObject->setTransmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(.25f, 0.5f, 1.0f, 0.0f)));
        sceneObject->setRefractionEtaShader(std::make_unique<Resources::ConstMaterialShader>(
            RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::AirSTP)
        ));
        scene.getSceneObjects().push_back(std::move(sceneObject));

        // front object
        sceneObject = std::make_unique<SceneObject>("front object");
        sceneObject->setForm(resources.getMesh("cup"));
        sceneObject->scale(Float3(1.0f, 1.0f, 1.0f));
        sceneObject->rotate(Float3(0.0f, 0.0f, 0.0f));
        sceneObject->translate(Float3(0.0f, 0.0f, 0.0f));
        sceneObject->scaleTexture(Float2(1.0f, 1.0f));
        sceneObject->rotateTexture(0.0f);
        sceneObject->translateTexture(Float2(0.0f, 0.0f));
        sceneObject->setEmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setDiffusionShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 0.0f)));
        sceneObject->setReflectanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->
            setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setRoughnessShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.8f, 0.8f, 0.8f, 0.0f)));
        sceneObject->setTransmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setRefractionEtaShader(std::make_unique<Resources::ConstMaterialShader>(vacuum2vacuum));
        scene.getSceneObjects().push_back(std::move(sceneObject));
    }
}
