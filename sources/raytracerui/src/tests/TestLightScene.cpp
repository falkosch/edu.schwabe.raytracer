#include "tests/TestLightScene.h"
#include "../stdafx.h"

namespace raytracerui
{
    void TestLightScene::setup(Scene& scene, Resources& /* resources */)
    {
        scene.setBackgroundShader(std::make_unique<ConstShader<SceneShader, Float4, RGBS>>(RGBS(0.f, 0.f, 0.f)));
        scene.setAmbientLight(RGBS(.01f, .01f, .01f));

        auto light = std::make_unique<LightInfo>();
        light->position = Float4(2.f, 0.f, 2.f, 1.f);
        light->emittance = RGBS(1.f, .75f, .5f);
        light->attenuationFactors = Float4(1.f, 0.f, 1.f, 1.f);
        scene.getLights().push_back(std::move(light));

        light = std::make_unique<LightInfo>();
        light->position = Float4(-2.f, 0.f, 2.f, 1.f);
        light->emittance = RGBS(.75f, 1.f, .5f);
        light->attenuationFactors = Float4(1.f, 0.f, 1.f, 1.f);
        scene.getLights().push_back(std::move(light));

        light = std::make_unique<LightInfo>();
        light->position = Float4(0.f, -2.f, 2.f, 1.f);
        light->emittance = RGBS(.5f, .75f, 1.f);
        light->attenuationFactors = Float4(1.f, 0.f, 1.f, 1.f);
        scene.getLights().push_back(std::move(light));

        light = std::make_unique<LightInfo>();
        light->position = Float4(0.f, 2.f, 2.f, 1.f);
        light->emittance = RGBS(1.f, .5f, .75f);
        light->attenuationFactors = Float4(1.f, 0.f, 1.f, 1.f);
        scene.getLights().push_back(std::move(light));

        light = std::make_unique<LightInfo>();
        light->position = Float4(0.f, 0.f, 3.f, 1.f);
        light->emittance = RGBS(1.f, 1.f, 1.f);
        light->attenuationFactors = Float4(2.f, 0.f, 0.f, 1.f);
        scene.getLights().push_back(std::move(light));
    }
}
