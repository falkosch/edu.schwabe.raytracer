#include "tests/TestLightScene.h"
#include "../stdafx.h"

namespace raytracerui
{
    void TestLightScene::setup(Scene& scene, Resources& /* resources */)
    {
        scene.setBackgroundShader(std::make_unique<RGBBackgroundShader>(RGBS::black()));
        scene.setAmbientLight(RGBS(.01f, .01f, .01f));

        // Warm tungsten (~3000K). Was RGB(1, .75, .5).
        auto light = std::make_unique<LightInfo>(LightInfo::blackbody(3000.f, 1.f));
        light->position = Float4(2.f, 0.f, 2.f, 1.f);
        light->attenuationFactors = Float4(1.f, 0.f, 1.f, 1.f);
        scene.getLights().push_back(std::move(light));

        // Yellow-green tint has no blackbody analog; keep as RGB. Was RGB(.75, 1, .5).
        light = std::make_unique<LightInfo>();
        light->position = Float4(-2.f, 0.f, 2.f, 1.f);
        light->emittance = RGBS(.75f, 1.f, .5f);
        light->attenuationFactors = Float4(1.f, 0.f, 1.f, 1.f);
        scene.getLights().push_back(std::move(light));

        // Cool daylight (~9500K). Was RGB(.5, .75, 1).
        light = std::make_unique<LightInfo>(LightInfo::blackbody(9500.f, 1.f));
        light->position = Float4(0.f, -2.f, 2.f, 1.f);
        light->attenuationFactors = Float4(1.f, 0.f, 1.f, 1.f);
        scene.getLights().push_back(std::move(light));

        // Pink/magenta tint has no blackbody analog; keep as RGB. Was RGB(1, .5, .75).
        light = std::make_unique<LightInfo>();
        light->position = Float4(0.f, 2.f, 2.f, 1.f);
        light->emittance = RGBS(1.f, .5f, .75f);
        light->attenuationFactors = Float4(1.f, 0.f, 1.f, 1.f);
        scene.getLights().push_back(std::move(light));

        // Neutral white camera light → D65 daylight. Was RGB(1, 1, 1).
        light = std::make_unique<LightInfo>(LightInfo::d65(1.f));
        light->position = Float4(0.f, 0.f, 3.f, 1.f);
        light->attenuationFactors = Float4(2.f, 0.f, 0.f, 1.f);
        scene.getLights().push_back(std::move(light));
    }
}
