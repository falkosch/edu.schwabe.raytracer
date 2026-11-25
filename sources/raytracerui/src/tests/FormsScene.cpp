#include "tests/FormsScene.h"
#include "../stdafx.h"

#include "tests/SceneObjects.h"

namespace raytracerui
{
    namespace
    {
        std::unique_ptr<SceneObject> checkered(
            std::unique_ptr<SceneObject> object, Resources& resources, const Float2& textureScale
        )
        {
            object->setDiffusionShader(std::make_unique<HDRImageShader>(*resources.getPPM("checker")));
            object->scaleTexture(textureScale);
            return object;
        }
    }

    void FormsScene::setup(Scene& scene, Resources& resources)
    {
        scene.setBackgroundShader(std::make_unique<RGBBackgroundShader>(RGBS(0.02f, 0.025f, 0.035f)));
        scene.setAmbientLight(RGBS::black());

        // Light values times pi: a white surface facing a light reflects its irradiance / pi (Lambert)
        // Neutral key light from the upper left, a directional light (without falloff, hard shadows)
        auto key = std::make_unique<LightInfo>(LightInfo::blackbody(5500.f, 0.45f * Pi<Float>()));
        key->direction = normalize3(Float4(-2.5f, 3.f, 3.f, 0.f));
        scene.getLights().push_back(std::move(key));

        // Cooler, weaker fill from the right
        auto fill = std::make_unique<LightInfo>(LightInfo::d65(0.12f * Pi<Float>()));
        fill->direction = normalize3(Float4(3.f, 1.f, 2.5f, 0.f));
        scene.getLights().push_back(std::move(fill));

        auto& objects = scene.getSceneObjects();
        const auto paper = SceneMaterial::matte(Float4(0.8f, 0.8f, 0.8f, 0.f));
        const auto glossy = SceneMaterial::plastic(Float4(0.8f, 0.8f, 0.8f, 0.f), 0.04f, 0.2f);

        // Planes: the tiled floor faces up, the plain back wall faces the camera and shows the shadows
        auto floor = createSceneObject(
            "floor plane", std::make_unique<Plane>(), paper,
            {Float3(1.f, 1.f, 1.f), Float3(-90.f, 0.f, 0.f), Float3(0.f, -1.f, 0.f)}
        );
        floor->setDiffusionShader(std::make_unique<HDRImageShader>(*resources.getPPM("seamless_floor_concrete")));
        floor->scaleTexture(Float2(0.5f, 0.5f));
        objects.push_back(std::move(floor));
        objects.push_back(createSceneObject(
            "back wall plane", std::make_unique<Plane>(), paper,
            {Float3(1.f, 1.f, 1.f), Float3(0.f, 0.f, 0.f), Float3(0.f, 0.f, -2.f)}
        ));

        // Back row: analytic sphere, upright box, mesh sphere
        // A shelf of boxes: board on two legs
        const auto wood = SceneMaterial::plastic(Float4(0.45f, 0.3f, 0.18f, 0.f), 0.04f, 0.4f);
        objects.push_back(createSceneObject(
            "shelf board", std::make_unique<Box>(), wood,
            {Float3(1.5f, 0.04f, 0.35f), Float3(0.f, 0.f, 0.f), Float3(0.f, -0.25f, -1.1f)}
        ));
        for (const auto x : {-1.3f, 1.3f})
        {
            objects.push_back(createSceneObject(
                "shelf leg", std::make_unique<Box>(), wood,
                {Float3(0.04f, 0.355f, 0.3f), Float3(0.f, 0.f, 0.f), Float3(x, -0.645f, -1.1f)}
            ));
        }

        // Back row on the shelf: analytic sphere, upright box, mesh sphere
        objects.push_back(checkered(
            createSceneObject(
                "sphere", std::make_unique<Sphere>(), glossy,
                {Float3(0.3f, 0.3f, 0.3f), Float3(0.f, 0.f, 0.f), Float3(-0.9f, 0.09f, -1.1f)}
            ),
            resources, Float2(1.f, 1.f)
        ));
        objects.push_back(checkered(
            createSceneObject(
                "box", std::make_unique<Box>(), glossy,
                {Float3(0.25f, 0.25f, 0.25f), Float3(0.f, 30.f, 0.f), Float3(0.f, 0.04f, -1.1f)}
            ),
            resources, Float2(1.f, 1.f)
        ));
        const auto* meshSphere = resources.getMesh("sphere_high");
        const auto meshSphereScale = fitMeshScale(*meshSphere, 0.6f);
        objects.push_back(checkered(
            createSceneObject(
                "mesh sphere", meshSphere, glossy,
                {Float3(meshSphereScale), Float3(0.f, 0.f, 0.f), Float3(0.9f, 0.09f, -1.1f)}
            ),
            resources, Float2(1.f, 1.f)
        ));

        // Front row: ellipsoid, tilted cube standing on a corner, low-poly mesh
        objects.push_back(checkered(
            createSceneObject(
                "ellipsoid", std::make_unique<Sphere>(), glossy,
                {Float3(0.4f, 0.2f, 0.25f), Float3(0.f, 0.f, 15.f), Float3(-0.75f, -0.8f, 0.1f)}
            ),
            resources, Float2(1.f, 1.f)
        ));
        objects.push_back(checkered(
            createSceneObject(
                "tilted cube", std::make_unique<Box>(), glossy,
                {Float3(0.2f, 0.2f, 0.2f), Float3(35.26f, 45.f, 0.f), Float3(0.f, -0.65f, 0.1f)}
            ),
            resources, Float2(1.f, 1.f)
        ));
        const auto* cone = resources.getMesh("cone");
        const auto coneScale = fitMeshScale(*cone, 0.6f);
        objects.push_back(checkered(
            createSceneObject(
                "cone mesh", cone, glossy, {Float3(coneScale), Float3(0.f, 0.f, 0.f), Float3(0.75f, -0.7f, 0.1f)}
            ),
            resources, Float2(1.f, 1.f)
        ));
    }
}
