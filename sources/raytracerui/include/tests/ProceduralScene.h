#pragma once

#include <raytracing.h>

#include <sstream>

namespace raytracerui {
  using namespace vectorization;
  using namespace raytracer;

  template <int LightsPerAxis = 3, int ObjectsPerAxis = 10>
  struct ProceduralScene {
    static void setupLightScene(Scene &scene, Resources & /*resources*/) {
      scene.setBackgroundShader(std::make_unique<ConstShader<SceneShader, Float4, RGBS>>(RGBS(0.f, 0.f, 0.f)));
      scene.setAmbientLight(RGBS::black());

      auto emittanceScale = reciprocal(Float4(LightsPerAxis * LightsPerAxis * LightsPerAxis + 1.f));
      for (auto z{-LightsPerAxis}; z <= LightsPerAxis; ++z) {
        for (auto y{-LightsPerAxis}; y <= LightsPerAxis; ++y) {
          for (auto x{-LightsPerAxis}; x <= LightsPerAxis; ++x) {
            auto light = std::make_unique<LightInfo>();
            auto position = convert<Float4>(Int4(x, y, z));
            light->position = oneW(position * Float4(4.f));
            light->emittance = zeroW(abs(position * emittanceScale) + emittanceScale);
            light->attenuationFactors = Float4(1.f, 1.f, 0.f, 1.f);
            scene.getLights().push_back(std::move(light));
          }
        }
      }
    }

    static void setup(Scene &scene, Resources &resources) {
      setupLightScene(scene, resources);

      for (auto z{-ObjectsPerAxis}; z < ObjectsPerAxis; ++z) {
        for (auto y{-ObjectsPerAxis}; y < ObjectsPerAxis; ++y) {
          for (auto x{-ObjectsPerAxis}; x < ObjectsPerAxis; ++x) {
            std::stringstream fmt;
            fmt << z << " " << y << " " << x;

            auto sceneObject = std::make_unique<SceneObject>(fmt.str());
            if ((x + y + z) & One<int>()) {
              sceneObject->setForm(std::make_unique<Box>());
            } else {
              sceneObject->setForm(std::make_unique<Sphere>());
            }
            sceneObject->scale(Float3(.25f));
            sceneObject->rotate(Float3(Float(z), Float(x), Float(y)) * Float(30.f));
            sceneObject->translate(Float3(Float(x), Float(y), Float(z)) + Float(.5f));
            sceneObject->scaleTexture(Float2(1.f, 1.f));
            sceneObject->rotateTexture(0.f);
            sceneObject->translateTexture(Float2(0.f, 0.f));
            sceneObject->setEmittanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.f, 0.f, 0.f, 0.f)));
            sceneObject->setDiffusionShader(std::make_unique<HDRImageShader>(*resources.getPPM("marble")));
            sceneObject->setReflectanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(.2f, .2f, .2f, 0.f)));
            sceneObject->setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(Float4(.2f, .2f, .2f, 0.f)));
            sceneObject->setRoughnessShader(std::make_unique<Resources::ConstMaterialShader>(Float4(.3f, .3f, .3f, 0.f)));
            sceneObject->setTransmittanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(0.f, 0.f, 0.f, 0.f)));
            sceneObject->setRefractionEtaShader(std::make_unique<Resources::ConstMaterialShader>(
                RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Water)
            ));
            scene.getSceneObjects().push_back(std::move(sceneObject));
          }
        }
      }
    }
  };
}
