#include "tests/TestLightScene.h"
#include "../stdafx.h"

namespace raytracerui {
  void TestLightScene::setup(Scene &scene, Resources & /* resources */) {
    // scene.setBackgroundShader(new EnvironmentShader(*resources.getPPM("env_eso_bright_night_at_paranal")));
    scene.setBackgroundShader(new ConstShader<SceneShader, Float4, Float4>(Float4(1.f, 1.f, 1.f, 0.f)));
    scene.setAmbientLight(Float4(.01f, .01f, .01f, 1.f));

    auto *light = new LightInfo();
    light->position = Float4(2.f, 0.f, 2.f, 1.f);
    light->emittance = Float4(1.f, .75f, .5f, 1.f);
    light->attenuationFactors = Float4(1.f, 0.f, 1.f, 1.f);
    scene.getLights().push_back(light);

    light = new LightInfo();
    light->position = Float4(-2.f, 0.f, 2.f, 1.f);
    light->emittance = Float4(.75f, 1.f, .5f, 1.f);
    light->attenuationFactors = Float4(1.f, 0.f, 1.f, 1.f);
    scene.getLights().push_back(light);

    light = new LightInfo();
    light->position = Float4(0.f, -2.f, 2.f, 1.f);
    light->emittance = Float4(.5f, .75f, 1.f, 1.f);
    light->attenuationFactors = Float4(1.f, 0.f, 1.f, 1.f);
    scene.getLights().push_back(light);

    light = new LightInfo();
    light->position = Float4(0.f, 2.f, 2.f, 1.f);
    light->emittance = Float4(1.f, .5f, .75f, 1.f);
    light->attenuationFactors = Float4(1.f, 0.f, 1.f, 1.f);
    scene.getLights().push_back(light);

    light = new LightInfo();
    light->position = Float4(0.f, 0.f, 3.f, 1.f);
    light->emittance = Float4(1.f, 1.f, 1.f, 1.f);
    light->attenuationFactors = Float4(2.f, 0.f, 0.f, 1.f);
    scene.getLights().push_back(light);
  }
}