#include "tests/CornellBoxScene.h"
#include "../stdafx.h"

#include "tests/SceneObjects.h"
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
        // spherical texture coordinates: the noise continues around the edges
        sceneObject->setForm(std::make_unique<Box>(BoxTextureMapping::Spherical));
        sceneObject->scale(Float3(0.35f, 0.60f, 0.35f));
        sceneObject->rotate(Float3(0.0f, 20.0f, 0.0f));
        sceneObject->translate(Float3(-0.35f, -0.39f, -0.35f));
        // the noise of the Materials scene's 8-octave Perlin sphere
        sceneObject->scaleTexture(Float2(4.0f, 4.0f));
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
        // Warm white, about 4000 K in linear sRGB at luminance 1
        const auto warmWhite = Float4(1.406f, 0.927f, 0.532f, 0.0f);

        // A box light set into the ceiling, emitting from its bottom face, of luminance 3 pi (about 9.4): the light the
        // camera sees is nearly white, the floor below gets an irradiance of about 0.47
        addBoxLight(
            scene, "ceiling light", Float4(0.0f, 0.994f, 0.0f, 1.0f), Float3(0.25f, 0.005f, 0.2f),
            Float3(0.0f, 0.0f, 0.0f), warmWhite * Float4(3.0f * Pi<Float>()), LightInfo::faceBit(1, false)
        );

        // No ambient light, as in every scene until the object explorer sets it per scene: without diffuse rays, the
        // ceiling and the shadows stay black
        scene.setAmbientLight(RGBS::black());

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
