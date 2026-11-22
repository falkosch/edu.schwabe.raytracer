#include "tests/LightingScene.h"
#include "../stdafx.h"

#include "tests/SceneObjects.h"

#include <array>

namespace raytracerui
{
    namespace
    {
        void addLight(Scene& scene, LightInfo light, const Float4& position, const Float4& attenuationFactors)
        {
            light.position = position;
            light.attenuationFactors = attenuationFactors;
            scene.getLights().push_back(std::make_unique<LightInfo>(light));
        }
    }

    void LightingScene::setup(Scene& scene, Resources& /*resources*/)
    {
        scene.setBackgroundShader(std::make_unique<RGBBackgroundShader>(RGBS::black()));
        scene.setAmbientLight(RGBS(0.005f, 0.005f, 0.005f));

        // Light pools on the back wall, warm to cold, each close to the wall with quadratic falloff
        const Float4 poolFalloff{0.f, 0.f, 1.f, 1.f};
        const std::array pools{
            LightInfo::blackbody(1900.f, 0.012f), LightInfo::illuminantA(0.012f), LightInfo::blackbody(4000.f, 0.012f),
            LightInfo::d65(0.012f), LightInfo::blackbody(10000.f, 0.012f)
        };
        auto x = -1.2f;
        for (const auto& pool : pools)
        {
            addLight(scene, pool, Float4(x, 0.55f, -1.35f, 1.f), poolFalloff);
            x += 0.6f;
        }

        // Red, green and blue lights low around the sphere: their shadows overlap in colors. Linear plus quadratic
        // falloff keeps them off the light pools above.
        const Float4 rgbFalloff{0.f, 0.5f, 1.f, 1.f};
        LightInfo red{};
        red.emittance = RGBS(0.3f, 0.f, 0.f);
        addLight(scene, red, Float4(-0.8f, -0.45f, 0.2f, 1.f), rgbFalloff);
        LightInfo green{};
        green.emittance = RGBS(0.f, 0.3f, 0.f);
        addLight(scene, green, Float4(0.f, -0.1f, 0.4f, 1.f), rgbFalloff);
        LightInfo blue{};
        blue.emittance = RGBS(0.f, 0.f, 0.3f);
        addLight(scene, blue, Float4(0.8f, -0.45f, 0.2f, 1.f), rgbFalloff);

        auto& objects = scene.getSceneObjects();
        const auto white = SceneMaterial::matte(Float4(0.8f, 0.8f, 0.8f, 0.f));
        objects.push_back(createSceneObject(
            "floor", std::make_unique<Plane>(), white,
            {Float3(1.f, 1.f, 1.f), Float3(-90.f, 0.f, 0.f), Float3(0.f, -1.f, 0.f)}
        ));
        objects.push_back(createSceneObject(
            "back wall", std::make_unique<Plane>(), white,
            {Float3(1.f, 1.f, 1.f), Float3(0.f, 0.f, 0.f), Float3(0.f, 0.f, -1.5f)}
        ));
        objects.push_back(createSceneObject(
            "white sphere", std::make_unique<Sphere>(), white,
            {Float3(0.3f, 0.3f, 0.3f), Float3(0.f, 0.f, 0.f), Float3(0.f, -0.7f, -0.6f)}
        ));
        objects.push_back(createSceneObject(
            "emissive sphere", std::make_unique<Sphere>(), SceneMaterial::emissive(Float4(0.25f, 0.08f, 0.01f, 0.f)),
            {Float3(0.12f, 0.12f, 0.12f), Float3(0.f, 0.f, 0.f), Float3(0.95f, -0.88f, 0.2f)}
        ));
    }
}
