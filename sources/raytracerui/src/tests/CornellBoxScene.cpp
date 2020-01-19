#include "tests/CornellBoxScene.h"
#include "../stdafx.h"

#include "tests/TestResources.h"
#include "tests/TestLightScene.h"

namespace raytracerui
{
    void CornellBoxScene::setup(Scene & scene, Resources & resources) {
        TestResources::setup(resources);

        scene.setBackgroundShader(new ConstShader<SceneShader, Float4, Float4>(Float4(1.f, 1.f, 1.f, 0.f)));
        scene.setAmbientLight(Float4(0.f, 0.f, 0.f, 0.f));

        LightInfo * light = new LightInfo();
        light->position = Float4(0.f, .98f, 0.f, 1.f);
        light->emissivity = Float4(1.f, .9f, .8f, 1.f);
        light->glare = Float4(4.f, 4.f, 4.f, 1.f);
        light->attenuationFactors = Float4(.5f, .4f, .3f, 1.f);
        scene.getLights().push_back(light);

        const Float4 vacuum2vacuum = RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum);
        SceneObject * so;

        // ceiling
        so = new SceneObject("ceiling");
        so->setForm(new Box());
        so->scale(Float3(1.f, .001f, 1.f));
        so->rotate(Float3(0.f, 0.f, 0.f));
        so->translate(Float3(0.f, 1.f, 0.f));
        so->scaleTexture(Float2(1.f, 1.f));
        so->rotateTexture(0.f);
        so->translateTexture(Float2(0.f, 0.f));
        so->setEmissivityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setDiffusionShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 1.f)));
        so->setReflectanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setSpecularityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setShininessShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 32.f)));
        so->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setRefractionEtaShader(new Resources::ConstMaterialShader(vacuum2vacuum));
        scene.getSceneObjects().push_back(so);

        // floor
        so = new SceneObject("floor");
        so->setForm(new Box());
        so->scale(Float3(1.f, .001f, 1.f));
        so->rotate(Float3(0.f, 0.f, 0.f));
        so->translate(Float3(0.f, -1.f, 0.f));
        so->scaleTexture(Float2(1.f, 1.f));
        so->rotateTexture(0.f);
        so->translateTexture(Float2(0.f, 0.f));
        so->setEmissivityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setDiffusionShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 1.f)));
        so->setReflectanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setSpecularityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setShininessShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 32.f)));
        so->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setRefractionEtaShader(new Resources::ConstMaterialShader(vacuum2vacuum));
        scene.getSceneObjects().push_back(so);

        // back wall
        so = new SceneObject("back wall");
        so->setForm(new Box());
        so->scale(Float3(1.f, 1.f, .001f));
        so->rotate(Float3(0.f, 0.f, 0.f));
        so->translate(Float3(0.f, 0.f, -1.f));
        so->scaleTexture(Float2(1.f, 1.f));
        so->rotateTexture(0.f);
        so->translateTexture(Float2(0.f, 0.f));
        so->setEmissivityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setDiffusionShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 1.f)));
        so->setReflectanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setSpecularityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setShininessShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 32.f)));
        so->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setRefractionEtaShader(new Resources::ConstMaterialShader(vacuum2vacuum));
        scene.getSceneObjects().push_back(so);

        // right wall
        so = new SceneObject("right wall");
        so->setForm(new Box());
        so->scale(Float3(.001f, 1.f, 1.f));
        so->rotate(Float3(0.f, 0.f, 0.f));
        so->translate(Float3(1.f, 0.f, 0.f));
        so->scaleTexture(Float2(1.f, 1.f));
        so->rotateTexture(0.f);
        so->translateTexture(Float2(0.f, 0.f));
        so->setEmissivityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setDiffusionShader(new Resources::ConstMaterialShader(Float4(0.f, 1.f, 0.f, 1.f)));
        so->setReflectanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setSpecularityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setShininessShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 32.f)));
        so->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setRefractionEtaShader(new Resources::ConstMaterialShader(vacuum2vacuum));
        scene.getSceneObjects().push_back(so);

        // left wall
        so = new SceneObject("left wall");
        so->setForm(new Box());
        so->scale(Float3(.001f, 1.f, 1.f));
        so->rotate(Float3(0.f, 0.f, 0.f));
        so->translate(Float3(-1.f, 0.f, 0.f));
        so->scaleTexture(Float2(1.f, 1.f));
        so->rotateTexture(0.f);
        so->translateTexture(Float2(0.f, 0.f));
        so->setEmissivityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setDiffusionShader(new Resources::ConstMaterialShader(Float4(1.f, 0.f, 0.f, 1.f)));
        so->setReflectanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setSpecularityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setShininessShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 32.f)));
        so->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setRefractionEtaShader(new Resources::ConstMaterialShader(vacuum2vacuum));
        scene.getSceneObjects().push_back(so);

        // small object
        so = new SceneObject("small object");
        so->setForm(new Sphere());
        so->scale(Float3(.35f, .35f, .35f));
        so->rotate(Float3(0.f, -20.f, 0.f));
        so->translate(Float3(.35f, -.64f, .35f));
        so->scaleTexture(Float2(1.f, 1.f));
        so->rotateTexture(0.f);
        so->translateTexture(Float2(0.f, 0.f));
        so->setEmissivityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setDiffusionShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setReflectanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, .95f)));
        so->setSpecularityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, .95f)));
        so->setShininessShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 32.f)));
        so->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, .95f)));
        so->setRefractionEtaShader(new Resources::ConstMaterialShader(
            RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Diamond)));
        scene.getSceneObjects().push_back(so);

        // large object
        so = new SceneObject("large object");
        so->setForm(new Box());
        so->scale(Float3(.35f, .60f, .35f));
        so->rotate(Float3(0.f, 20.f, 0.f));
        so->translate(Float3(-.35f, -.39f, -.35f));
        so->scaleTexture(Float2(1.f, 1.f));
        so->rotateTexture(0.f);
        so->translateTexture(Float2(0.f, 0.f));
        so->setEmissivityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setDiffusionShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 1.f)));
        so->setReflectanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setSpecularityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setShininessShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 32.f)));
        so->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setRefractionEtaShader(new Resources::ConstMaterialShader(vacuum2vacuum));
        scene.getSceneObjects().push_back(so);

        // test mesh
        so = new SceneObject("test mesh");
        so->setForm(resources.getMesh("teapot"));
        so->scale(Float3(.25f, .25f, .25f));
        so->rotate(Float3(0.f, 0.f, 0.f));
        so->translate(Float3(-.35f, .455f, -.35f));
        so->scaleTexture(Float2(1.f, 1.f));
        so->rotateTexture(0.f);
        so->translateTexture(Float2(0.f, 0.f));
        so->setEmissivityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 0.f)));
        so->setDiffusionShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, .05f)));
        so->setReflectanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, .95f)));
        so->setSpecularityShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, .95f)));
        so->setShininessShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, 32.f)));
        so->setTransmittanceShader(new Resources::ConstMaterialShader(Float4(1.f, 1.f, 1.f, .95f)));
        so->setRefractionEtaShader(new Resources::ConstMaterialShader(
            RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Glass)));
        scene.getSceneObjects().push_back(so);
    }
}