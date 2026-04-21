#include "tests/TestScene2.h"
#include "../stdafx.h"

#include "tests/TestLightScene.h"
#include "tests/TestResources.h"

namespace raytracerui {
  void TestScene2::setup(Scene &scene, Resources &resources) {
    TestResources::setup(resources);
    TestLightScene::setup(scene, resources);

    const auto vacuum2vacuum = RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum);

    // plane
    auto sceneObject = std::make_unique<SceneObject>("plane");
    sceneObject->setForm(std::make_unique<Plane>());
    sceneObject->scale(Float3(1.f, 1.f, 1.f));
    sceneObject->rotate(Float3(0.f, 0.f, 0.f));
    sceneObject->translate(Float3(0.f, 0.f, 0.f));
    sceneObject->scaleTexture(Float2(1.f, 1.f));
    sceneObject->rotateTexture(0.f);
    sceneObject->translateTexture(Float2(0.f, 0.f));
    sceneObject->setEmittanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.f, 0.f, 0.f, 0.f)));
    sceneObject->setDiffusionShader(std::make_unique<HDRImageShader>(*resources.getPPM("checker")));
    sceneObject->setReflectanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(1.f, 1.f, 1.f, 0.5f)));
    sceneObject->setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(Float4(1.f, 1.f, 1.f, 0.5f)));
    sceneObject->setShininessShader(std::make_unique<Resources::ConstMaterialShader>(Float4(1.f, 1.f, 1.f, 32.f)));
    sceneObject->setTransmittanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.f, 0.f, 0.f, 0.f)));
    sceneObject->setRefractionEtaShader(std::make_unique<Resources::ConstMaterialShader>(vacuum2vacuum));
    scene.getSceneObjects().push_back(std::move(sceneObject));

    // sphere
    sceneObject = std::make_unique<SceneObject>("sphere");
    sceneObject->setForm(std::make_unique<Sphere>());
    sceneObject->scale(Float3(1.f, 1.f, 1.f));
    sceneObject->rotate(Float3(0.f, 0.f, 0.f));
    sceneObject->translate(Float3(0.f, 0.f, 0.f));
    sceneObject->scaleTexture(Float2(1.f, 1.f));
    sceneObject->rotateTexture(0.f);
    sceneObject->translateTexture(Float2(0.f, 0.f));
    sceneObject->setEmittanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(.9f, .95f, 1.f, .1f)));
    sceneObject->setDiffusionShader(std::make_unique<Resources::ConstMaterialShader>(Float4(1.f, 1.f, 1.f, .2f)));
    sceneObject->setReflectanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(1.f, 1.f, 1.f, .9f)));
    sceneObject->setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(Float4(1.f, 1.f, 1.f, .9f)));
    sceneObject->setShininessShader(std::make_unique<Resources::ConstMaterialShader>(Float4(1.f, 1.f, 1.f, 32.f)));
    sceneObject->setTransmittanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(1.f, 1.f, 1.f, .90f)));
    sceneObject->setRefractionEtaShader(std::make_unique<Resources::ConstMaterialShader>(
        RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Water)
    ));
    scene.getSceneObjects().push_back(std::move(sceneObject));
  }
}
