#include "tests/DragonScene.h"
#include "../stdafx.h"

#include "tests/TestResources.h"
#include "tests/TestLightScene.h"

namespace raytracerui
{
    void DragonScene::setup(Scene & scene, Resources & resources) {
        TestResources::setup(resources);
        TestLightScene::setup(scene, resources);

        SceneObject * sceneObject;

        // background plane
        sceneObject = new SceneObject("background plane");
        sceneObject->setForm(new Plane());
        sceneObject->scale(Float3(1.f, 1.f, 1.f));
        sceneObject->rotate(Float3(0.f, 0.f, 0.f));
        sceneObject->translate(Float3(0.f, 0.f, -2.f));
        sceneObject->scaleTexture(Float2(.25f, .25f));
        sceneObject->rotateTexture(0.f);
        sceneObject->translateTexture(Float2(0.f, 0.f));
        sceneObject->setEmissivityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        sceneObject->setDiffusionShader(new HDRImageShader(*resources.getPPM("checker")));
        sceneObject->setReflectanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 1.f)));
        sceneObject->setSpecularityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 1.f)));
        sceneObject->setShininessShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 64.f)));
        sceneObject->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        sceneObject->setRefractionEtaShader(new Resources::ConstMaterialShader(
            RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum)));
        scene.getSceneObjects().push_back(sceneObject);

        // mesh figure
        sceneObject = new SceneObject("mesh figure");
        sceneObject->setForm(resources.getMesh("dragon2"));
        sceneObject->scale(Float3(1.f, 1.f, 1.f));
        sceneObject->rotate(Float3(90.f, 0.f, 0.f));
        sceneObject->translate(Float3(0.f, 0.f, 0.f));
        sceneObject->scaleTexture(Float2(1.f, 1.f));
        sceneObject->rotateTexture(0.f);
        sceneObject->translateTexture(Float2(0.f, 0.f));
        sceneObject->setEmissivityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        sceneObject->setDiffusionShader(new HDRImageShader(*resources.getPPM("marble")));
        sceneObject->setReflectanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, .25f)));
        sceneObject->setSpecularityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, .25f)));
        sceneObject->setShininessShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 32.f)));
        sceneObject->setTransmittanceShader(new HDRImageShader(*resources.getPPM("earth_iced")));
        sceneObject->setRefractionEtaShader(new Resources::ConstMaterialShader(RefractionIndices::refractionEta(
            RefractionIndices::Vacuum, RefractionIndices::Diamond)));
        scene.getSceneObjects().push_back(sceneObject);
    }
}