#include "tests/TestScene2.h"
#include "../stdafx.h"

#include "tests/TestLightScene.h"
#include "tests/TestResources.h"

namespace raytracerui
{
    void TestScene2::setup(Scene& scene, Resources& resources)
    {
        TestResources::setup(resources);
        TestLightScene::setup(scene, resources);

        const auto vacuum2vacuum = RefractionIndices::refractionEta(RefractionIndices::Vacuum,
                                                                    RefractionIndices::Vacuum);

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
        // a specular lobe of F0 0.5 and weight 1
        sceneObject->setReflectanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(1.f, 1.f, 1.f, 0.f)));
        sceneObject->setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.5f, 0.5f, 0.5f, 0.f)));
        sceneObject->
            setRoughnessShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.3f, 0.3f, 0.3f, 0.f)));
        sceneObject->setTransmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.f, 0.f, 0.f, 0.f)));
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
        sceneObject->
            setEmittanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(.09f, .095f, .1f, 0.f)));
        sceneObject->setDiffusionShader(std::make_unique<Resources::ConstMaterialShader>(Float4(.2f, .2f, .2f, 0.f)));
        sceneObject->setReflectanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(.9f, .9f, .9f, 0.f)));
        sceneObject->setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(Float4(.9f, .9f, .9f, 0.f)));
        sceneObject->setRoughnessShader(std::make_unique<Resources::ConstMaterialShader>(Float4(.2f, .2f, .2f, 0.f)));
        sceneObject->setTransmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(.9f, .9f, .9f, 0.f)));
        sceneObject->setRefractionEtaShader(std::make_unique<Resources::ConstMaterialShader>(
            RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Water)
        ));
        scene.getSceneObjects().push_back(std::move(sceneObject));

        // A directional light from the right at the plane's horizon (perpendicular to its normal, +z): it lights the
        // sphere from the side and grazes the plane, whose shadow rays run parallel to it
        auto grazing = std::make_unique<LightInfo>(LightInfo::d65(Pi<Float>()));
        grazing->direction = Float4(1.f, 0.f, 0.f, 0.f);
        scene.getLights().push_back(std::move(grazing));
    }
}
