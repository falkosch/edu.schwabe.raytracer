#include "tests/LightingScene.h"
#include "../stdafx.h"

#include "tests/SceneObjects.h"

#include <array>

namespace raytracerui
{
    namespace
    {
        // Scales every light of the scene: the ambient light, the lights and their emitters
        constexpr auto LIGHT_SCALE = 0.85f;

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
        scene.setAmbientLight(RGBS(0.005f * LIGHT_SCALE, 0.005f * LIGHT_SCALE, 0.005f * LIGHT_SCALE));
        // The lights times pi: a white surface facing a light reflects its irradiance / pi (Lambert), the ambient
        // light (a radiance from all directions) its value
        const auto lightScale = LIGHT_SCALE * Pi<Float>();

        // Light pools on the back wall, warm to cold, each close to the wall with quadratic falloff
        const Float4 poolFalloff{0.f, 0.f, 1.f, 1.f};
        const auto poolBrightness = 0.012f * lightScale;
        const std::array pools{
            LightInfo::blackbody(1900.f, poolBrightness), LightInfo::illuminantA(poolBrightness),
            LightInfo::blackbody(4000.f, poolBrightness), LightInfo::d65(poolBrightness),
            LightInfo::blackbody(10000.f, poolBrightness)
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
        red.emittance = RGBS(0.3f * lightScale, 0.f, 0.f);
        addLight(scene, red, Float4(-0.8f, -0.45f, 0.2f, 1.f), rgbFalloff);
        LightInfo green{};
        green.emittance = RGBS(0.f, 0.3f * lightScale, 0.f);
        addLight(scene, green, Float4(0.f, -0.1f, 0.4f, 1.f), rgbFalloff);
        LightInfo blue{};
        blue.emittance = RGBS(0.f, 0.f, 0.3f * lightScale);
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

        // A cyan light bar leaning in the back on the left (a box light glowing on all faces): a pool on the wall and
        // cyan on the sphere. Its box casts thin shadows of the colored lights on the wall right behind it; further to
        // the front it would shade the scene from them.
        addBoxLight(
            scene, "cyan light bar", Float4(-1.f, -0.68f, -0.9f, 1.f), Float3(0.025f, 0.3f, 0.025f),
            Float3(0.f, 0.f, -15.f), Float4(0.45f, 3.3f, 7.5f, 0.f) * Float4(lightScale)
        );

        // An orange sphere light on the floor: its emissive sphere, and soft shadows
        addSphereLight(
            scene, "orange sphere light", Float4(0.95f, -0.88f, 0.2f, 1.f), 0.12f,
            Float4(3.f, 1.f, 0.12f, 0.f) * Float4(lightScale)
        );
    }
}
