#include "../stdafx.h"

#include "tests/TestScene1.h"

#include "tests/TestResources.h"
#include "tests/TestLightScene.h"

#include <shading/shaders/HDRImageShader.h>
#include <shading/shaders/IntersectionNormalShader.h>
#include <scene/SceneObject.h>
#include <geometry/forms/Plane.h>
#include <geometry/forms/Box.h>
#include <geometry/forms/Sphere.h>
#include <common/RefractionIndices.h>

namespace raytracer
{

    void TestScene1::setup(Scene & scene, Resources & resources)
    {
        TestResources::setup(resources);
        TestLightScene::setup(scene, resources);

        const Float4 vacuum2vacuum = RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum);

        SceneObject* sceneObject;

        // floor
        sceneObject = new SceneObject("floor");
        sceneObject->setForm(new Plane());
        sceneObject->scale(Float3(1.0f, 1.0f, 1.0f));
        sceneObject->rotate(Float3(-90.f, 0.0f, 0.0f));
        sceneObject->translate(Float3(0.0f, -1.0f, 0.0f));
        sceneObject->scaleTexture(Float2(1.0f, 1.0f));
        sceneObject->rotateTexture(0.0f);
        sceneObject->translateTexture(Float2(0.0f, 0.0f));
        sceneObject->setEmissivityShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setDiffusionShader(new HDRImageShader(*resources.getPPM("seamless_floor_concrete")));
        sceneObject->setReflectanceShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setSpecularityShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setShininessShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setRefractionEtaShader(new Resources::ConstMaterialShader(vacuum2vacuum));
        scene.getSceneObjects().push_back(sceneObject);

        // back object
        sceneObject = new SceneObject("back object");
        sceneObject->setForm(new Box());
        sceneObject->scale(Float3(4.0f, 3.0f, 0.1f));
        sceneObject->rotate(Float3(0.0f, 0.0f, 0.0f));
        sceneObject->translate(Float3(0.0f, 10.5f, -5.f));
        sceneObject->scaleTexture(Float2(4.0f, 4.0f));
        sceneObject->rotateTexture(0.0f);
        sceneObject->translateTexture(Float2(0.0f, 0.0f));
        sceneObject->setEmissivityShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setDiffusionShader(new HDRImageShader(*resources.getPPM("checker")));
        sceneObject->setReflectanceShader(new Resources::ConstMaterialShader(Float4(1.0f, 1.0f, 1.0f, 0.0f)));
        sceneObject->setSpecularityShader(new Resources::ConstMaterialShader(Float4(1.0f, 1.0f, 1.0f, 0.0f)));
        sceneObject->setShininessShader(new Resources::ConstMaterialShader(Float4(1.0f, 1.0f, 1.0f, 32.f)));
        sceneObject->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setRefractionEtaShader(new Resources::ConstMaterialShader(vacuum2vacuum));
        scene.getSceneObjects().push_back(sceneObject);

        // middle object
        sceneObject = new SceneObject("middle object");
        sceneObject->setForm(new Sphere());
        sceneObject->scale(Float3(0.5f, 0.5f, 0.5f));
        sceneObject->rotate(Float3(0.0f, 0.0f, 0.0f));
        sceneObject->translate(Float3(0.0f, 1.0f, 0.0f));
        sceneObject->scaleTexture(Float2(1.0f, 1.0f));
        sceneObject->rotateTexture(0.0f);
        sceneObject->translateTexture(Float2(0.0f, 0.0f));
        sceneObject->setEmissivityShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setDiffusionShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setReflectanceShader(new Resources::ConstMaterialShader(Float4(1.0f, 1.0f, 1.0f, 0.0f)));
        sceneObject->setSpecularityShader(new Resources::ConstMaterialShader(Float4(1.0f, 1.0f, 1.0f, 0.0f)));
        sceneObject->setShininessShader(new Resources::ConstMaterialShader(Float4(1.0f, 1.0f, 1.0f, 32.f)));
        sceneObject->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(1.0f, 0.75f, 0.25f, 1.0f)));
        sceneObject->setRefractionEtaShader(new Resources::ConstMaterialShader(RefractionIndices::refractionEta(
            RefractionIndices::Vacuum, RefractionIndices::Glass)));
        scene.getSceneObjects().push_back(sceneObject);

        // left object
        sceneObject = new SceneObject("left object");
        sceneObject->setForm(new Sphere());
        sceneObject->scale(Float3(1.0f, 1.0f, 1.0f) / 3.0f);
        sceneObject->rotate(Float3(0.0f, 0.0f, 0.0f));
        sceneObject->translate(Float3(-1.0f, 0.0f, 0.0f));
        sceneObject->scaleTexture(Float2(1.0f, 1.0f));
        sceneObject->rotateTexture(0.0f);
        sceneObject->translateTexture(Float2(0.0f, 0.0f));
        sceneObject->setEmissivityShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setDiffusionShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setReflectanceShader(new Resources::ConstMaterialShader(Float4(1.0f, 0.95f, 1.0f, 0.95f)));
        sceneObject->setSpecularityShader(new Resources::ConstMaterialShader(Float4(1.0f, 0.95f, 1.0f, 0.95f)));
        sceneObject->setShininessShader(new Resources::ConstMaterialShader(Float4(1.0f, 0.99f, 0.98f, 32.f)));
        sceneObject->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(0.25f, 0.25f, 0.4f, 1.0f)));
        sceneObject->setRefractionEtaShader(new Resources::ConstMaterialShader(RefractionIndices::refractionEta(
            RefractionIndices::Vacuum, RefractionIndices::Diamond)));
        scene.getSceneObjects().push_back(sceneObject);

        // right object
        sceneObject = new SceneObject("right object");
        sceneObject->setForm(new Box());
        sceneObject->scale(Float3(1.0f, 1.0f, 1.0f) / 3.0f);
        sceneObject->rotate(Float3(1.0f, 0.0f, 0.0f));
        sceneObject->translate(Float3(1.0f, 0.1f, 0.1f));
        sceneObject->scaleTexture(Float2(1.0f, 1.0f));
        sceneObject->rotateTexture(0.0f);
        sceneObject->translateTexture(Float2(0.0f, 0.0f));
        sceneObject->setEmissivityShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setDiffusionShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setReflectanceShader(new Resources::ConstMaterialShader(Float4(1.0f, 1.0f, 1.0f, 0.1f)));
        sceneObject->setSpecularityShader(new Resources::ConstMaterialShader(Float4(1.0f, 1.0f, 1.0f, 0.1f)));
        sceneObject->setShininessShader(new Resources::ConstMaterialShader(Float4(1.0f, 1.0f, 1.0f, 32.0f)));
        sceneObject->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(.25f, 0.5f, 1.0f, 1.0f)));
        sceneObject->setRefractionEtaShader(new Resources::ConstMaterialShader(RefractionIndices::refractionEta(
            RefractionIndices::Vacuum, RefractionIndices::AirSTP)));
        scene.getSceneObjects().push_back(sceneObject);

        // front object
        sceneObject = new SceneObject("front object");
        sceneObject->setForm(resources.getMesh("sphere_low"));
        sceneObject->scale(Float3(1.0f, 1.0f, 1.0f));
        sceneObject->rotate(Float3(0.0f, 0.0f, 0.0f));
        sceneObject->translate(Float3(0.0f, 0.0f, 0.0f));
        sceneObject->scaleTexture(Float2(1.0f, 1.0f));
        sceneObject->rotateTexture(0.0f);
        sceneObject->translateTexture(Float2(0.0f, 0.0f));
        sceneObject->setEmissivityShader(new Resources::ConstMaterialShader(Float4(1.0f, 0.0f, 0.0f, 1.0f)));
        sceneObject->setDiffusionShader(new Resources::ConstMaterialShader(Float4(0.0f, 1.0f, 1.0f, 1.0f)));
        sceneObject->setReflectanceShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setSpecularityShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setShininessShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setRefractionEtaShader(new Resources::ConstMaterialShader(vacuum2vacuum));
        scene.getSceneObjects().push_back(sceneObject);
    }

}
