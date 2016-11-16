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
        sceneObject->scale(Float3(1.f, 1.f, 1.f));
        sceneObject->rotate(Float3(-90.f, 0.f, 0.f));
        sceneObject->translate(Float3(0.f, -1.f, 0.f));
        sceneObject->scaleTexture(Float2(1.f, 1.f));
        sceneObject->rotateTexture(0.f);
        sceneObject->translateTexture(Float2(0.f, 0.f));
        sceneObject->setEmissivityShader(new Resources::ConstMaterialShader(Float4(0.f, 0.f, 0.f, 0.f)));
        sceneObject->setDiffusionShader(new HDRImageShader(*resources.getPPM("seamless_floor_concrete")));
        sceneObject->setReflectanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        sceneObject->setSpecularityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        sceneObject->setShininessShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 1.f)));
        sceneObject->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(0.f, 0.f, 0.f, 0.f)));
        sceneObject->setRefractionEtaShader(new Resources::ConstMaterialShader(vacuum2vacuum));
        scene.getSceneObjects().push_back(sceneObject);

        // back object
        sceneObject = new SceneObject("back object");
        sceneObject->setForm(new Box());
        sceneObject->scale(Float3(4.f, 3.f, .1f));
        sceneObject->rotate(Float3(0.f, 0.f, 0.f));
        sceneObject->translate(Float3(0.f, 1.5f, -5.f));
        sceneObject->scaleTexture(Float2(4.f, 4.f));
        sceneObject->rotateTexture(0.f);
        sceneObject->translateTexture(Float2(0.f, 0.f));
        sceneObject->setEmissivityShader(new Resources::ConstMaterialShader(Float4(0.f, 0.f, 0.f, 0.f)));
        sceneObject->setDiffusionShader(new HDRImageShader(*resources.getPPM("checker")));
        sceneObject->setReflectanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        sceneObject->setSpecularityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        sceneObject->setShininessShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 32.f)));
        sceneObject->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(0.f, 0.f, 0.f, 0.f)));
        sceneObject->setRefractionEtaShader(new Resources::ConstMaterialShader(vacuum2vacuum));
        scene.getSceneObjects().push_back(sceneObject);

        // middle object
        sceneObject = new SceneObject("middle object");
        sceneObject->setForm(new Sphere());
        sceneObject->scale(Float3(.5f, .5f, .5f));
        sceneObject->rotate(Float3(0.f, 0.f, 0.f));
        sceneObject->translate(Float3(0.f, 1.f, 0.f));
        sceneObject->scaleTexture(Float2(1.f, 1.f));
        sceneObject->rotateTexture(0.f);
        sceneObject->translateTexture(Float2(0.f, 0.f));
        sceneObject->setEmissivityShader(new Resources::ConstMaterialShader(Float4(0.f, 0.f, 0.f, 0.f)));
        sceneObject->setDiffusionShader(new Resources::ConstMaterialShader(Float4(0.f, 0.f, 0.f, 0.f)));
        sceneObject->setReflectanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        sceneObject->setSpecularityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        sceneObject->setShininessShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 32.f)));
        sceneObject->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(1.f, .75f, .25f, 1.f)));
        sceneObject->setRefractionEtaShader(new Resources::ConstMaterialShader(RefractionIndices::refractionEta(
            RefractionIndices::Vacuum, RefractionIndices::Glass)));
        scene.getSceneObjects().push_back(sceneObject);

        // left object
        sceneObject = new SceneObject("left object");
        sceneObject->setForm(new Sphere());
        sceneObject->scale(Float3(1.f, 1.f, 1.f) / 3.f);
        sceneObject->rotate(Float3(0.f, 0.f, 0.f));
        sceneObject->translate(Float3(-1.f, 0.f, 0.f));
        sceneObject->scaleTexture(Float2(1.f, 1.f));
        sceneObject->rotateTexture(0.f);
        sceneObject->translateTexture(Float2(0.f, 0.f));
        sceneObject->setEmissivityShader(new Resources::ConstMaterialShader(Float4(0.f, 0.f, 0.f, 0.f)));
        sceneObject->setDiffusionShader(new Resources::ConstMaterialShader(Float4(0.f, 0.f, 0.f, 0.f)));
        sceneObject->setReflectanceShader(new Resources::ConstMaterialShader(Float4(1.f, .95f, 1.f, .95f)));
        sceneObject->setSpecularityShader(new Resources::ConstMaterialShader(Float4(1.f, .95f, 1.f, .95f)));
        sceneObject->setShininessShader(new Resources::ConstMaterialShader(Float4(1.f, .99f, .98f, 32.f)));
        sceneObject->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(.25f, .25f, .4f, 1.f)));
        sceneObject->setRefractionEtaShader(new Resources::ConstMaterialShader(RefractionIndices::refractionEta(
            RefractionIndices::Vacuum, RefractionIndices::Diamond)));
        scene.getSceneObjects().push_back(sceneObject);

        // right object
        sceneObject = new SceneObject("right object");
        sceneObject->setForm(new Box());
        sceneObject->scale(Float3(1.f, 1.f, 1.f) / 3.f);
        sceneObject->rotate(Float3(1.f, 0.f, 0.f));
        sceneObject->translate(Float3(1.f, .1f, .1f));
        sceneObject->scaleTexture(Float2(1.f, 1.f));
        sceneObject->rotateTexture(0.f);
        sceneObject->translateTexture(Float2(0.f, 0.f));
        sceneObject->setEmissivityShader(new Resources::ConstMaterialShader(Float4(0.f, 0.f, 0.f, 0.f)));
        sceneObject->setDiffusionShader(new Resources::ConstMaterialShader(Float4(0.f, 0.f, 0.f, 0.f)));
        sceneObject->setReflectanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, .1f)));
        sceneObject->setSpecularityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, .1f)));
        sceneObject->setShininessShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 32)));
        sceneObject->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(.25f, .5f, 1.f, 1.f)));
        sceneObject->setRefractionEtaShader(new Resources::ConstMaterialShader(RefractionIndices::refractionEta(
            RefractionIndices::Vacuum, RefractionIndices::AirSTP)));
        scene.getSceneObjects().push_back(sceneObject);

        // front object
        sceneObject = new SceneObject("front object");
        sceneObject->setForm(resources.getMesh("teapot"));
        sceneObject->scale(Float3(.4f, .4f, .4f));
        sceneObject->rotate(Float3(0.f, 0.f, 0.f));
        sceneObject->translate(Float3(0.f, -.45f, 1.f));
        sceneObject->scaleTexture(Float2(1.f, 1.f));
        sceneObject->rotateTexture(0.f);
        sceneObject->translateTexture(Float2(0.f, 0.f));
        sceneObject->setEmissivityShader(new Resources::ConstMaterialShader(Float4(0.f, 0.f, 0.f, 0.f)));
        sceneObject->setDiffusionShader(new Resources::ConstMaterialShader(Float4(1.f, .9f, .8f, 1.f)));
        sceneObject->setReflectanceShader(new Resources::ConstMaterialShader(Float4(0.f, 0.f, 0.f, 0.f)));
        sceneObject->setSpecularityShader(new Resources::ConstMaterialShader(Float4(0.f, 0.f, 0.f, 0.f)));
        sceneObject->setShininessShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 1.f)));
        sceneObject->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(0.f, 0.f, 0.f, 0.f)));
        sceneObject->setRefractionEtaShader(new Resources::ConstMaterialShader(vacuum2vacuum));
        scene.getSceneObjects().push_back(sceneObject);
    }

}
