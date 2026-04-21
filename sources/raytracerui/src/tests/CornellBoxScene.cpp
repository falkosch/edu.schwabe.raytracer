#include "tests/CornellBoxScene.h"
#include "../stdafx.h"

#include "tests/TestResources.h"

namespace raytracerui {

  auto ceiling() {
    auto sceneObject = std::make_unique<SceneObject>("ceiling, black");
    sceneObject->setForm(std::make_unique<Box>());
    sceneObject->scale(Float3(1.0f, 0.001f, 1.0f));
    sceneObject->translate(Float3(0.0f, 1.0f, 0.0f));
    sceneObject->setEmittanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setDiffusionShader(std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 1.0f)));
    sceneObject->setReflectanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setShininessShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 32.0f)));
    sceneObject->setTransmittanceShader(
        std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f))
    );
    sceneObject->setRefractionEtaShader(
        std::make_unique<Resources::ConstMaterialShader>(
            RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum)
        )
    );
    return sceneObject;
  }

  auto floor() {
    auto sceneObject = std::make_unique<SceneObject>("floor, white");
    sceneObject->setForm(std::make_unique<Box>());
    sceneObject->scale(Float3(1.0f, 0.001f, 1.0f));
    sceneObject->translate(Float3(0.0f, -1.0f, 0.0f));
    sceneObject->setEmittanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setDiffusionShader(std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 1.0f)));
    sceneObject->setReflectanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setShininessShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 32.0f)));
    sceneObject->setTransmittanceShader(
        std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f))
    );
    sceneObject->setRefractionEtaShader(
        std::make_unique<Resources::ConstMaterialShader>(
            RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum)
        )
    );
    return sceneObject;
  }

  auto backWall() {
    auto sceneObject = std::make_unique<SceneObject>("back wall, white");
    sceneObject->setForm(std::make_unique<Box>());
    sceneObject->scale(Float3(1.0f, 1.0f, 0.001f));
    sceneObject->translate(Float3(0.0f, 0.0f, -1.0f));
    sceneObject->setEmittanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setDiffusionShader(std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 1.0f)));
    sceneObject->setReflectanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setShininessShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 32.0f)));
    sceneObject->setTransmittanceShader(
        std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f))
    );
    sceneObject->setRefractionEtaShader(
        std::make_unique<Resources::ConstMaterialShader>(
            RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum)
        )
    );
    return sceneObject;
  }

  auto rightWall() {
    auto sceneObject = std::make_unique<SceneObject>("right wall, green");
    sceneObject->setForm(std::make_unique<Box>());
    sceneObject->scale(Float3(0.001f, 1.0f, 1.0f));
    sceneObject->translate(Float3(1.0f, 0.0f, 0.0f));
    sceneObject->setEmittanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setDiffusionShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 1.0f, 0.0f, 1.0f)));
    sceneObject->setReflectanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setShininessShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 32.0f)));
    sceneObject->setTransmittanceShader(
        std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f))
    );
    sceneObject->setRefractionEtaShader(
        std::make_unique<Resources::ConstMaterialShader>(
            RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum)
        )
    );
    return sceneObject;
  }

  auto leftWall() {
    auto sceneObject = std::make_unique<SceneObject>("left wall, red");
    sceneObject->setForm(std::make_unique<Box>());
    sceneObject->scale(Float3(0.001f, 1.0f, 1.0f));
    sceneObject->translate(Float3(-1.0f, 0.0f, 0.0f));
    sceneObject->setEmittanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setDiffusionShader(std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setReflectanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setShininessShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 32.0f)));
    sceneObject->setTransmittanceShader(
        std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f))
    );
    sceneObject->setRefractionEtaShader(
        std::make_unique<Resources::ConstMaterialShader>(
            RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum)
        )
    );
    return sceneObject;
  }

  auto smallSphere() {
    auto sceneObject = std::make_unique<SceneObject>("small sphere");
    sceneObject->setForm(std::make_unique<Sphere>());
    sceneObject->scale(Float3(0.35f, 0.35f, 0.35f));
    sceneObject->translate(Float3(0.35f, -0.64f, 0.35f));
    sceneObject->setEmittanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setDiffusionShader(std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 0.87f)));
    sceneObject->setReflectanceShader(
        std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 0.13f))
    );
    sceneObject->setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 0.13f)));
    sceneObject->setShininessShader(std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 4.0f)));
    sceneObject->setTransmittanceShader(
        std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 0.87f))
    );
    sceneObject->setRefractionEtaShader(
        std::make_unique<Resources::ConstMaterialShader>(
            RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Water)
        )
    );
    return sceneObject;
  }

  auto largeBox() {
    auto sceneObject = std::make_unique<SceneObject>("large box");
    sceneObject->setForm(std::make_unique<Box>());
    sceneObject->scale(Float3(0.35f, 0.60f, 0.35f));
    sceneObject->rotate(Float3(0.0f, 20.0f, 0.0f));
    sceneObject->translate(Float3(-0.35f, -0.39f, -0.35f));
    sceneObject->setEmittanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setDiffusionShader(std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 1.0f)));
    sceneObject->setReflectanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setShininessShader(std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 32.0f)));
    sceneObject->setTransmittanceShader(
        std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f))
    );
    sceneObject->setRefractionEtaShader(
        std::make_unique<Resources::ConstMaterialShader>(
            RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum)
        )
    );
    return sceneObject;
  }

  auto meshAboveLargeBox(Resources &resources) {
    auto sceneObject = std::make_unique<SceneObject>("mesh object");
    sceneObject->setForm(resources.getMesh("teapot"));
    sceneObject->scale(Float3(0.3f, 0.3f, 0.3f));
    sceneObject->rotate(Float3(0.0f, 0.0f, 0.0f));
    sceneObject->translate(Float3(-0.35f, 0.375f, -0.35f));
    sceneObject->setEmittanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setDiffusionShader(std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 0.8f, 0.8f, 0.2f)));
    sceneObject->setReflectanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.5f, 1.0f, 1.0f, 0.8f)));
    sceneObject->setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.5f, 0.9f, 0.9f, 0.8f)));
    sceneObject->setShininessShader(std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 32.0f)));
    sceneObject->setTransmittanceShader(
        std::make_unique<Resources::ConstMaterialShader>(Float4(0.9f, 0.9f, 0.9f, 0.8f))
    );
    sceneObject->setRefractionEtaShader(
        std::make_unique<Resources::ConstMaterialShader>(
            RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Glass)
        )
    );
    return sceneObject;
  }

  void CornellBoxScene::setup(Scene &scene, Resources &resources) {
    TestResources::setup(resources);

    scene.setBackgroundShader(
        std::make_unique<ConstShader<SceneShader, Float4, Float4>>(Float4(1.0f, 1.0f, 1.0f, 0.0f))
    );
    scene.setAmbientLight(Float4(0.0f, 0.0f, 0.0f, 0.0f));

    auto light = std::make_unique<LightInfo>();
    light->position = Float4(0.0f, 0.98f, 0.0f, 1.0f);
    light->emittance = Float4(1.0f, 0.9f, 0.8f, 1.0f);
    light->glare = Float4(4.0f, 4.0f, 4.0f, 1.0f);
    light->attenuationFactors = Float4(0.0f, 0.13f, 1.0f, 1.0f);
    scene.getLights().push_back(std::move(light));

    scene.getSceneObjects().push_back(ceiling());
    scene.getSceneObjects().push_back(floor());
    scene.getSceneObjects().push_back(backWall());
    scene.getSceneObjects().push_back(rightWall());
    scene.getSceneObjects().push_back(leftWall());
    scene.getSceneObjects().push_back(smallSphere());
    scene.getSceneObjects().push_back(largeBox());
    scene.getSceneObjects().push_back(meshAboveLargeBox(resources));
  }
}
