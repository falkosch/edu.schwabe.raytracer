#include "tests/CornellBoxScene.h"
#include "../stdafx.h"

#include "tests/TestResources.h"

#include <raytracing/common/noise/PerlinNoiseGenerator.h>
#include <raytracing/shading/shaders/NoiseGeneratorMap.h>

namespace raytracerui
{
    auto ceiling()
    {
        auto sceneObject = std::make_unique<SceneObject>("ceiling, black");
        sceneObject->setForm(std::make_unique<Box>());
        sceneObject->scale(Float3(1.0f, 0.001f, 1.0f));
        sceneObject->translate(Float3(0.0f, 1.0f, 0.0f));
        sceneObject->setEmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setDiffusionShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 0.0f)));
        sceneObject->setReflectanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->
            setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setRoughnessShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 0.0f)));
        sceneObject->setTransmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f))
        );
        sceneObject->setRefractionEtaShader(
            std::make_unique<Resources::ConstMaterialShader>(
                RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum)
            )
        );
        return sceneObject;
    }

    auto floor()
    {
        auto sceneObject = std::make_unique<SceneObject>("floor, white");
        sceneObject->setForm(std::make_unique<Box>());
        sceneObject->scale(Float3(1.0f, 0.001f, 1.0f));
        sceneObject->translate(Float3(0.0f, -1.0f, 0.0f));
        sceneObject->setEmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setDiffusionShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 0.0f)));
        sceneObject->setReflectanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->
            setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setRoughnessShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 0.0f)));
        sceneObject->setTransmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f))
        );
        sceneObject->setRefractionEtaShader(
            std::make_unique<Resources::ConstMaterialShader>(
                RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum)
            )
        );
        return sceneObject;
    }

    auto backWall()
    {
        auto sceneObject = std::make_unique<SceneObject>("back wall, white");
        sceneObject->setForm(std::make_unique<Box>());
        sceneObject->scale(Float3(1.0f, 1.0f, 0.001f));
        sceneObject->translate(Float3(0.0f, 0.0f, -1.0f));
        sceneObject->setEmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setDiffusionShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 0.0f)));
        sceneObject->setReflectanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->
            setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setRoughnessShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 0.0f)));
        sceneObject->setTransmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f))
        );
        sceneObject->setRefractionEtaShader(
            std::make_unique<Resources::ConstMaterialShader>(
                RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum)
            )
        );
        return sceneObject;
    }

    auto rightWall()
    {
        auto sceneObject = std::make_unique<SceneObject>("right wall, green");
        sceneObject->setForm(std::make_unique<Box>());
        sceneObject->scale(Float3(0.001f, 1.0f, 1.0f));
        sceneObject->translate(Float3(1.0f, 0.0f, 0.0f));
        sceneObject->setEmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setDiffusionShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 1.0f, 0.0f, 0.0f)));
        sceneObject->setReflectanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->
            setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setRoughnessShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 0.0f)));
        sceneObject->setTransmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f))
        );
        sceneObject->setRefractionEtaShader(
            std::make_unique<Resources::ConstMaterialShader>(
                RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum)
            )
        );
        return sceneObject;
    }

    auto leftWall()
    {
        auto sceneObject = std::make_unique<SceneObject>("left wall, red");
        sceneObject->setForm(std::make_unique<Box>());
        sceneObject->scale(Float3(0.001f, 1.0f, 1.0f));
        sceneObject->translate(Float3(-1.0f, 0.0f, 0.0f));
        sceneObject->setEmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setDiffusionShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setReflectanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->
            setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setRoughnessShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 0.0f)));
        sceneObject->setTransmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f))
        );
        sceneObject->setRefractionEtaShader(
            std::make_unique<Resources::ConstMaterialShader>(
                RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum)
            )
        );
        return sceneObject;
    }

    auto mirrorSphere()
    {
        auto sceneObject = std::make_unique<SceneObject>("mirror sphere");
        sceneObject->setForm(std::make_unique<Sphere>());
        sceneObject->scale(Float3(0.35f, 0.35f, 0.35f));
        sceneObject->translate(Float3(0.35f, -0.64f, 0.35f));
        sceneObject->setEmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setDiffusionShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setReflectanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 0.0f)));
        sceneObject->setSpecularShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 0.0f)));
        sceneObject->setRoughnessShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setTransmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setRefractionEtaShader(
            std::make_unique<Resources::ConstMaterialShader>(
                RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum)
            )
        );
        return sceneObject;
    }

    auto roughMirrorSphere()
    {
        auto sceneObject = std::make_unique<SceneObject>("rough mirror sphere");
        sceneObject->setForm(std::make_unique<Sphere>());
        sceneObject->scale(Float3(0.3f, 0.3f, 0.3f));
        sceneObject->translate(Float3(-0.55f, -0.69f, 0.5f));
        sceneObject->setEmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setDiffusionShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.05f, 0.05f, 0.05f, 0.0f)));
        sceneObject->setReflectanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.9f, 0.9f, 0.9f, 0.0f)));
        sceneObject->setSpecularShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.9f, 0.9f, 0.9f, 0.0f)));
        sceneObject->setRoughnessShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.45f, 0.45f, 0.45f, 0.0f)));
        sceneObject->setTransmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setRefractionEtaShader(
            std::make_unique<Resources::ConstMaterialShader>(
                RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum)
            )
        );
        return sceneObject;
    }

    auto largeBox()
    {
        static PerlinNoiseGenerator perlinNoise{42};

        auto sceneObject = std::make_unique<SceneObject>("large box, diffuse perlin noise");
        sceneObject->setForm(std::make_unique<Box>());
        sceneObject->scale(Float3(0.35f, 0.60f, 0.35f));
        sceneObject->rotate(Float3(0.0f, 20.0f, 0.0f));
        sceneObject->translate(Float3(-0.35f, -0.39f, -0.35f));
        sceneObject->scaleTexture(Float2(0.6f, 0.6f));
        sceneObject->setEmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setDiffusionShader(
            std::unique_ptr<const ObjectShader::MaterialShader>(std::make_unique<NoiseGeneratorMap<8>>(perlinNoise))
        );
        sceneObject->setReflectanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->
            setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setRoughnessShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.5f, 0.5f, 0.5f, 0.0f)));
        sceneObject->setTransmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f))
        );
        sceneObject->setRefractionEtaShader(
            std::make_unique<Resources::ConstMaterialShader>(
                RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum)
            )
        );
        return sceneObject;
    }

    auto glassTeapot(Resources& resources)
    {
        auto sceneObject = std::make_unique<SceneObject>("glass teapot");
        sceneObject->setForm(resources.getMesh("teapot"));
        sceneObject->scale(Float3(0.3f, 0.3f, 0.3f));
        sceneObject->rotate(Float3(0.0f, 0.0f, 0.0f));
        sceneObject->translate(Float3(-0.35f, 0.375f, -0.35f));
        sceneObject->setEmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.0f, 0.0f, 0.0f, 0.0f)));
        sceneObject->setDiffusionShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.02f, 0.02f, 0.02f, 0.0f)));
        sceneObject->setReflectanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.1f, 0.1f, 0.1f, 0.0f)));
        sceneObject->setSpecularShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.5f, 0.5f, 0.5f, 0.0f)));
        sceneObject->setRoughnessShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.05f, 0.05f, 0.05f, 0.0f)));
        sceneObject->setTransmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(0.95f, 0.95f, 0.95f, 0.0f)));
        sceneObject->setRefractionEtaShader(
            std::make_unique<Resources::ConstMaterialShader>(
                RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Glass)
            )
        );
        return sceneObject;
    }

    void CornellBoxScene::setup(Scene& scene, Resources& resources)
    {
        TestResources::setup(resources);

        scene.setBackgroundShader(std::make_unique<RGBBackgroundShader>(RGBS::black()));
        scene.setAmbientLight(RGBS::black());

        // Warm tungsten ceiling light (~3000K). Was RGB(0.1, 0.09, 0.08).
        auto ceilingLight = std::make_unique<LightInfo>(LightInfo::blackbody(3000.f, 0.1f));
        ceilingLight->position = Float4(0.0f, 0.98f, 0.0f, 1.0f);
        ceilingLight->glare = RGBS(0.4f, 0.4f, 0.4f);
        ceilingLight->attenuationFactors = Float4(0.0f, 0.13f, 1.0f, 1.0f);
        scene.getLights().push_back(std::move(ceilingLight));

        // Faint D65 daylight fill from the camera. Was RGB(0.01, 0.01, 0.01).
        auto cameraLight = std::make_unique<LightInfo>(LightInfo::d65(0.01f));
        cameraLight->position = Float4(0.0f, 0.0f, 3.2f, 1.0f);
        cameraLight->attenuationFactors = Float4(0.0f, 0.1f, 1.0f, 1.0f);
        scene.getLights().push_back(std::move(cameraLight));

        scene.getSceneObjects().push_back(ceiling());
        scene.getSceneObjects().push_back(floor());
        scene.getSceneObjects().push_back(backWall());
        scene.getSceneObjects().push_back(rightWall());
        scene.getSceneObjects().push_back(leftWall());
        scene.getSceneObjects().push_back(mirrorSphere());
        scene.getSceneObjects().push_back(roughMirrorSphere());
        scene.getSceneObjects().push_back(largeBox());
        scene.getSceneObjects().push_back(glassTeapot(resources));
    }
}
