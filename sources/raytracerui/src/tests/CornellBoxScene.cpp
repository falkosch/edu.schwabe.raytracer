#include "tests/CornellBoxScene.h"
#include "../stdafx.h"

#include "tests/TestResources.h"

namespace raytracerui {

  auto ceiling() {
    auto sceneObject = new SceneObject("ceiling, black");
    sceneObject->setForm(new Box());
    sceneObject->scale(Float3(1.0f, 0.001f, 1.0f));
    sceneObject->translate(Float3(0.0f, 1.0f, 0.0f));
    sceneObject->setEmittanceShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setDiffusionShader(new Resources::ConstMaterialShader(Float4(1.0f, 1.0f, 1.0f, 1.0f)));
    sceneObject->setReflectanceShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setSpecularShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setShininessShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 32.0f)));
    sceneObject->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setRefractionEtaShader(new Resources::ConstMaterialShader(
        RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum)
    ));
    return sceneObject;
  }

  auto floor() {
    auto sceneObject = new SceneObject("floor, white");
    sceneObject->setForm(new Box());
    sceneObject->scale(Float3(1.0f, 0.001f, 1.0f));
    sceneObject->translate(Float3(0.0f, -1.0f, 0.0f));
    sceneObject->setEmittanceShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setDiffusionShader(new Resources::ConstMaterialShader(Float4(1.0f, 1.0f, 1.0f, 1.0f)));
    sceneObject->setReflectanceShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setSpecularShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setShininessShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 32.0f)));
    sceneObject->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setRefractionEtaShader(new Resources::ConstMaterialShader(
        RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum)
    ));
    return sceneObject;
  }

  auto backWall() {
    auto sceneObject = new SceneObject("back wall, white");
    sceneObject->setForm(new Box());
    sceneObject->scale(Float3(1.0f, 1.0f, 0.001f));
    sceneObject->translate(Float3(0.0f, 0.0f, -1.0f));
    sceneObject->setEmittanceShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setDiffusionShader(new Resources::ConstMaterialShader(Float4(1.0f, 1.0f, 1.0f, 1.0f)));
    sceneObject->setReflectanceShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setSpecularShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setShininessShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 32.0f)));
    sceneObject->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setRefractionEtaShader(new Resources::ConstMaterialShader(
        RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum)
    ));
    return sceneObject;
  }

  auto rightWall() {
    auto sceneObject = new SceneObject("right wall, green");
    sceneObject->setForm(new Box());
    sceneObject->scale(Float3(0.001f, 1.0f, 1.0f));
    sceneObject->translate(Float3(1.0f, 0.0f, 0.0f));
    sceneObject->setEmittanceShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setDiffusionShader(new Resources::ConstMaterialShader(Float4(0.0f, 1.0f, 0.0f, 1.0f)));
    sceneObject->setReflectanceShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setSpecularShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setShininessShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 32.0f)));
    sceneObject->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setRefractionEtaShader(new Resources::ConstMaterialShader(
        RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum)
    ));
    return sceneObject;
  }

  auto leftWall() {
    auto sceneObject = new SceneObject("left wall, red");
    sceneObject->setForm(new Box());
    sceneObject->scale(Float3(0.001f, 1.0f, 1.0f));
    sceneObject->translate(Float3(-1.0f, 0.0f, 0.0f));
    sceneObject->setEmittanceShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setDiffusionShader(new Resources::ConstMaterialShader(Float4(1.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setReflectanceShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setSpecularShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setShininessShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 32.0f)));
    sceneObject->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setRefractionEtaShader(new Resources::ConstMaterialShader(
        RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum)
    ));
    return sceneObject;
  }

  auto smallSphere() {
    auto sceneObject = new SceneObject("small sphere");
    sceneObject->setForm(new Sphere());
    sceneObject->scale(Float3(0.35f, 0.35f, 0.35f));
    sceneObject->translate(Float3(0.35f, -0.64f, 0.35f));
    sceneObject->setEmittanceShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setDiffusionShader(new Resources::ConstMaterialShader(Float4(1.0f, 1.0f, 1.0f, 0.87f)));
    sceneObject->setReflectanceShader(new Resources::ConstMaterialShader(Float4(1.0f, 1.0f, 1.0f, 0.13f)));
    sceneObject->setSpecularShader(new Resources::ConstMaterialShader(Float4(1.0f, 1.0f, 1.0f, 0.13f)));
    sceneObject->setShininessShader(new Resources::ConstMaterialShader(Float4(1.0f, 1.0f, 1.0f, 4.0f)));
    sceneObject->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(1.0f, 1.0f, 1.0f, 0.87f)));
    sceneObject->setRefractionEtaShader(new Resources::ConstMaterialShader(
        RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Water)
    ));
    return sceneObject;
  }

  auto largeBox() {
    auto sceneObject = new SceneObject("large box");
    sceneObject->setForm(new Box());
    sceneObject->scale(Float3(0.35f, 0.60f, 0.35f));
    sceneObject->rotate(Float3(0.0f, 20.0f, 0.0f));
    sceneObject->translate(Float3(-0.35f, -0.39f, -0.35f));
    sceneObject->setEmittanceShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setDiffusionShader(new Resources::ConstMaterialShader(Float4(1.0f, 1.0f, 1.0f, 1.0f)));
    sceneObject->setReflectanceShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setSpecularShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setShininessShader(new Resources::ConstMaterialShader(Float4(1.0f, 1.0f, 1.0f, 32.0f)));
    sceneObject->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setRefractionEtaShader(new Resources::ConstMaterialShader(
        RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum)
    ));
    return sceneObject;
  }

  auto meshAboveLargeBox(Resources &resources) {
    auto sceneObject = new SceneObject("mesh object");
    sceneObject->setForm(resources.getMesh("teapot"));
    sceneObject->scale(Float3(0.3f, 0.3f, 0.3f));
    sceneObject->rotate(Float3(0.0f, 0.0f, 0.0f));
    sceneObject->translate(Float3(-0.35f, 0.375f, -0.35f));
    sceneObject->setEmittanceShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 1.0f)));
    sceneObject->setDiffusionShader(new Resources::ConstMaterialShader(Float4(1.0f, 1.0f, 1.0f, 0.13f)));
    sceneObject->setReflectanceShader(new Resources::ConstMaterialShader(Float4(1.0f, 1.0f, 1.0f, 0.87f)));
    sceneObject->setSpecularShader(new Resources::ConstMaterialShader(Float4(1.0f, 1.0f, 1.0f, 0.87f)));
    sceneObject->setShininessShader(new Resources::ConstMaterialShader(Float4(1.0f, 1.0f, 1.0f, 32.0f)));
    sceneObject->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(1.0f, 1.0f, 1.0f, 0.87f)));
    sceneObject->setRefractionEtaShader(new Resources::ConstMaterialShader(
        RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Glass)
    ));
    return sceneObject;
  }

  void CornellBoxScene::setup(Scene &scene, Resources &resources) {
    TestResources::setup(resources);

    scene.setBackgroundShader(new ConstShader<SceneShader, Float4, Float4>(Float4(1.0f, 1.0f, 1.0f, 0.0f)));
    scene.setAmbientLight(Float4(0.0f, 0.0f, 0.0f, 0.0f));

    auto light = new LightInfo();
    light->position = Float4(0.0f, 0.98f, 0.0f, 1.0f);
    light->emittance = Float4(1.0f, 0.9f, 0.8f, 1.0f);
    light->glare = Float4(4.0f, 4.0f, 4.0f, 1.0f);
    light->attenuationFactors = Float4(0.0f, 0.13f, 1.0f, 1.0f);
    scene.getLights().push_back(light);

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
