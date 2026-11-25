#include "tests/TestLightScene.h"
#include "../stdafx.h"

namespace raytracerui
{
    void TestLightScene::setup(Scene& scene, Resources& /* resources */)
    {
        scene.setBackgroundShader(std::make_unique<RGBBackgroundShader>(RGBS::black()));
        scene.setAmbientLight(RGBS::black());

        // Light values times pi: a white surface facing a light reflects its irradiance / pi (Lambert). The colored
        // lights fall off as the inverse square of the distance; 8/9 of the intensities that fell off as 1 / (1 + d^2)
        // light the origin (d^2 = 8) as those did.
        constexpr auto Retuned = 8.f / 9.f;
        // Warm tungsten (~3000K). Was RGB(1, .75, .5).
        auto light = std::make_unique<LightInfo>(LightInfo::blackbody(3000.f, Retuned * Pi<Float>()));
        light->position = Float4(2.f, 0.f, 2.f, 1.f);
        scene.getLights().push_back(std::move(light));

        // Yellow-green tint has no blackbody analog; keep as RGB. Was RGB(.75, 1, .5).
        light = std::make_unique<LightInfo>();
        light->position = Float4(-2.f, 0.f, 2.f, 1.f);
        light->emittance = RGBS(Float4(.75f, 1.f, .5f, 0.f) * Float4(Retuned * Pi<Float>()));
        scene.getLights().push_back(std::move(light));

        // Cool daylight (~9500K). Was RGB(.5, .75, 1).
        light = std::make_unique<LightInfo>(LightInfo::blackbody(9500.f, Retuned * Pi<Float>()));
        light->position = Float4(0.f, -2.f, 2.f, 1.f);
        scene.getLights().push_back(std::move(light));

        // Pink/magenta tint has no blackbody analog; keep as RGB. Was RGB(1, .5, .75).
        light = std::make_unique<LightInfo>();
        light->position = Float4(0.f, 2.f, 2.f, 1.f);
        light->emittance = RGBS(Float4(1.f, .5f, .75f, 0.f) * Float4(Retuned * Pi<Float>()));
        scene.getLights().push_back(std::move(light));

        // Neutral white camera light → D65 daylight. Was RGB(1, 1, 1). It had no falloff, at half its value: falling
        // off as the inverse square, 4.5 times the intensity lights the origin (3 away) as it did.
        light = std::make_unique<LightInfo>(LightInfo::d65(4.5f * Pi<Float>()));
        light->position = Float4(0.f, 0.f, 3.f, 1.f);
        scene.getLights().push_back(std::move(light));
    }
}
