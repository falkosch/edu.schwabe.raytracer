#include "tests/TestScene2.h"
#include "../stdafx.h"

#include "tests/TestResources.h"
#include "tests/TestLightScene.h"

namespace raytracerui
{
    void TestScene2::setup(Scene & scene, Resources & resources) {
        TestResources::setup(resources);
        TestLightScene::setup(scene, resources);

        const Float4 vacuum2vacuum = RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum);
        SceneObject * sceneObject;

        // plane
        sceneObject = new SceneObject("plane");
        sceneObject->setForm(new Plane());
        sceneObject->scale(Float3(1.f, 1.f, 1.f));
        sceneObject->rotate(Float3(0.f, 0.f, 0.f));
        sceneObject->translate(Float3(0.f, 0.f, 0.f));
        sceneObject->scaleTexture(Float2(1.f, 1.f));
        sceneObject->rotateTexture(0.f);
        sceneObject->translateTexture(Float2(0.f, 0.f));
        sceneObject->setEmissivityShader(new Resources::ConstMaterialShader(Float4(0.f, 0.f, 0.f, 0.f)));
        sceneObject->setDiffusionShader(new HDRImageShader(*resources.getPPM("checker")));
        sceneObject->setReflectanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.5f)));
        sceneObject->setSpecularityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.5f)));
        sceneObject->setShininessShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 32.f)));
        sceneObject->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(0.f, 0.f, 0.f, 0.f)));
        sceneObject->setRefractionEtaShader(new Resources::ConstMaterialShader(vacuum2vacuum));
        scene.getSceneObjects().push_back(sceneObject);

        // sphere
        sceneObject = new SceneObject("sphere");
        sceneObject->setForm(new Sphere());
        sceneObject->scale(Float3(1.f, 1.f, 1.f));
        sceneObject->rotate(Float3(0.f, 0.f, 0.f));
        sceneObject->translate(Float3(0.f, 0.f, 0.f));
        sceneObject->scaleTexture(Float2(1.f, 1.f));
        sceneObject->rotateTexture(0.f);
        sceneObject->translateTexture(Float2(0.f, 0.f));
        sceneObject->setEmissivityShader(new Resources::ConstMaterialShader(Float4(.9f, .95f, 1.f, .1f)));
        sceneObject->setDiffusionShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, .2f)));
        sceneObject->setReflectanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, .9f)));
        sceneObject->setSpecularityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, .9f)));
        sceneObject->setShininessShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 32.f)));
        sceneObject->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, .90f)));
        sceneObject->setRefractionEtaShader(new Resources::ConstMaterialShader(
            RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Water)));
        scene.getSceneObjects().push_back(sceneObject);
    }
}