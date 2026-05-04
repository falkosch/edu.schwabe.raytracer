#include "tests/DragonScene.h"
#include "../stdafx.h"

#include "tests/TestLightScene.h"
#include "tests/TestResources.h"

namespace raytracerui
{
    void DragonScene::setup(Scene& scene, Resources& resources)
    {
        TestResources::setup(resources);
        TestLightScene::setup(scene, resources);

        // background plane
        auto sceneObject = std::make_unique<SceneObject>("background plane");
        sceneObject->setForm(std::make_unique<Plane>());
        sceneObject->scale(Float3(1.f, 1.f, 1.f));
        sceneObject->rotate(Float3(0.f, 0.f, 0.f));
        sceneObject->translate(Float3(0.f, 0.f, -2.f));
        sceneObject->scaleTexture(Float2(.25f, .25f));
        sceneObject->rotateTexture(0.f);
        sceneObject->translateTexture(Float2(0.f, 0.f));
        sceneObject->setEmittanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.f, 0.f, 0.f, 0.f)));
        sceneObject->setDiffusionShader(std::make_unique<HDRImageShader>(*resources.getPPM("checker")));
        sceneObject->setReflectanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.f, 0.f, 0.f, 0.f)));
        sceneObject->setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.f, 0.f, 0.f, 0.f)));
        sceneObject->setRoughnessShader(std::make_unique<Resources::ConstMaterialShader>(Float4(1.f, 1.f, 1.f, 0.f)));
        sceneObject->setTransmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.f, 0.f, 0.f, 0.f)));
        sceneObject->setRefractionEtaShader(std::make_unique<Resources::ConstMaterialShader>(
            RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum)
        ));
        scene.getSceneObjects().push_back(std::move(sceneObject));

        // mesh figure
        sceneObject = std::make_unique<SceneObject>("mesh figure");
        sceneObject->setForm(resources.getMesh("dragon2"));
        sceneObject->scale(Float3(1.f, 1.f, 1.f));
        sceneObject->rotate(Float3(90.f, 0.f, 0.f));
        sceneObject->translate(Float3(0.f, 0.f, 0.f));
        sceneObject->scaleTexture(Float2(1.f, 1.f));
        sceneObject->rotateTexture(0.f);
        sceneObject->translateTexture(Float2(0.f, 0.f));
        sceneObject->setEmittanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.f, 0.f, 0.f, 0.f)));
        sceneObject->setDiffusionShader(std::make_unique<HDRImageShader>(*resources.getPPM("marble")));
        sceneObject->setReflectanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.5f, 0.5f, 0.5f, 0.f)));
        sceneObject->setSpecularShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.f)));
        sceneObject->setRoughnessShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 0.f)));
        sceneObject->setTransmittanceShader(std::make_unique<HDRImageShader>(*resources.getLinearPPM("earth_iced")));
        sceneObject->setRefractionEtaShader(std::make_unique<Resources::ConstMaterialShader>(
            RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Diamond)
        ));
        scene.getSceneObjects().push_back(std::move(sceneObject));
    }
}
