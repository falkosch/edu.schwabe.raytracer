#include "raytracing_tests.h"

#include <raytracing/RaytraceConfiguration.h>
#include <raytracing/RaytraceObserver.h>
#include <raytracing/Raytracer.h>
#include <raytracing/common/RefractionIndices.h>
#include <raytracing/geometry/forms/Box.h>
#include <raytracing/geometry/forms/Plane.h>
#include <raytracing/scene/Camera.h>
#include <raytracing/scene/Resources.h>
#include <raytracing/scene/Scene.h>
#include <raytracing/shading/BoxLightFaces.h>
#include <raytracing/shading/LightInfo.h>
#include <raytracing/shading/shaders/RGBBackgroundShader.h>
#include <raytracing/shading/spectral/wavelengths.h>

#include <condition_variable>
#include <memory>
#include <mutex>

namespace raytracer::test {
  TEST_CLASS(RaytracerTest) {
    // Keeps the image of the first render
    struct ImageObserver final : RaytraceObserver {
      std::mutex mutex;
      std::condition_variable rendered;
      std::shared_ptr<HDRImage> image;

      void notifyUpdate(const RaytraceConfiguration &configuration) override {
        std::lock_guard lock(mutex);
        image = configuration.image;
        rendered.notify_all();
      }
    };

    // The wall the camera looks at: white and diffuse, facing +z at z = -1
    static constexpr Float WallZ = -0.999f;

    // The red or white plate beside the wall's center: at x = 0.3, facing the center with its -x face
    static constexpr Float PlateX = 0.299f;

    // A thin box of the diffusion and emittance, nothing else (no highlights, reflections or refraction)
    static std::unique_ptr<SceneObject> panel(
        const Float3 &halfExtents, const Float3 &center, const Float4 &diffusion,
        const Float4 &emittance = Zero<Float4>()
    ) {
      auto object = std::make_unique<SceneObject>("panel");
      object->setForm(std::make_unique<Box>());
      object->scale(halfExtents);
      object->translate(center);
      const auto none = Zero<Float4>();
      object->setDiffusionShader(std::make_unique<Resources::ConstMaterialShader>(diffusion));
      object->setReflectanceShader(std::make_unique<Resources::ConstMaterialShader>(none));
      object->setSpecularShader(std::make_unique<Resources::ConstMaterialShader>(none));
      object->setRoughnessShader(std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 0.0f)));
      object->setEmittanceShader(std::make_unique<Resources::ConstMaterialShader>(emittance));
      object->setTransmittanceShader(std::make_unique<Resources::ConstMaterialShader>(none));
      object->setRefractionEtaShader(std::make_unique<Resources::ConstMaterialShader>(
          RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum)
      ));
      return object;
    }

    static std::unique_ptr<SceneObject> wall() {
      return panel(Float3(2.0f, 2.0f, 0.001f), Float3(0.0f, 0.0f, -1.0f), Float4(1.0f, 1.0f, 1.0f, 0.0f));
    }

    // A plate perpendicular to the wall, from just in front of it to z = 1
    static std::unique_ptr<SceneObject> plate(const Float4 &diffusion) {
      return panel(Float3(0.001f, 2.0f, 0.999f), Float3(PlateX + 0.001f, 0.0f, 0.002f), diffusion);
    }

    // The emitting square above the wall's center, half size 0.5 at y = 0.5, spanning z = -0.9 to 0.1
    static const Float4 &squareCenter() {
      static const auto center = Float4(0.0f, 0.5f, -0.4f, 1.0f);
      return center;
    }

    // A white box light of radiance 1 of the square's shape, emitting downwards
    static std::unique_ptr<LightInfo> squareLight() {
      auto light = std::make_unique<LightInfo>();
      light->position = squareCenter();
      light->emittance = RGBS(1.0f, 1.0f, 1.0f);
      light->halfExtents = Float4(0.5f, 0.0f, 0.5f, 0.0f);
      light->emittingFaces = LightInfo::faceBit(1, false);
      return light;
    }

    // The square as an emissive object of radiance 1
    static std::unique_ptr<SceneObject> squareEmitter() {
      return panel(Float3(0.5f, 0.0001f, 0.5f), Float3(x(squareCenter()), y(squareCenter()), z(squareCenter())),
                   Zero<Float4>(), Float4(1.0f, 1.0f, 1.0f, 0.0f));
    }

    static void setUpEmpty(Scene &scene, const RGBS &background = RGBS::black(), const RGBS &ambient = RGBS::black()) {
      scene.setBackgroundShader(std::make_unique<RGBBackgroundShader>(background));
      scene.setAmbientLight(ambient);
    }

    // A rough opaque panel in the wall's place, with the specular channel (F0) and the reflectance channel (the
    // specular lobe's weight), no diffusion: it reflects by its specular lobe only
    static std::unique_ptr<SceneObject> glossyWall(
        const Float reflectance, const Float specular, const Float roughness
    ) {
      auto object = wall();
      object->setDiffusionShader(std::make_unique<Resources::ConstMaterialShader>(Zero<Float4>()));
      object->setReflectanceShader(
          std::make_unique<Resources::ConstMaterialShader>(Float4(reflectance, reflectance, reflectance, 0.0f)));
      object->setSpecularShader(
          std::make_unique<Resources::ConstMaterialShader>(Float4(specular, specular, specular, 0.0f)));
      object->setRoughnessShader(
          std::make_unique<Resources::ConstMaterialShader>(Float4(roughness, roughness, roughness, 0.0f)));
      return object;
    }

    // Renders the wall's center: one pixel of a 1 degree view from (0, 0, 3.2) towards -z, (n + 1) x (n + 1)
    // sub-samples for the super-sampling n, with up to the diffuse rays per hit, or from irradiance records. Returns
    // the pixel, sRGB-linear.
    static Float4 renderWallCenter(
        Scene &scene, const ASizeT diffuseSamples, const bool irradianceCaching = false, const ASizeT superSampling = 3
    ) {
      RaytraceParameters parameters{};
      parameters.diffuseSamples = diffuseSamples;
      parameters.irradianceCaching = irradianceCaching;
      return renderWallCenter(scene, parameters, superSampling);
    }

    // Renders the wall's center as above, with the sampling of the parameters
    static Float4 renderWallCenter(Scene &scene, RaytraceParameters parameters, const ASizeT superSampling) {
      scene.buildSceneGraph();
      constexpr Float FieldOfView = 1.0f;
      const auto zPlanes = Float2(0.01f, 2.0f);
      Camera camera{};
      camera.setProjection(FieldOfView, Float2(2.0f), zPlanes);
      camera.translate(Float3(0.0f, 0.0f, -3.2f));

      ImageObserver observer{};
      parameters.visibilityCutoff = 1.0f / 512.0f;
      parameters.maxTraceDepth = std::max(parameters.maxTraceDepth, ASizeT{4});
      parameters.sceneShader = &scene;
      parameters.cullingOrientation = -1;
      parameters.resolution = Size2(1, 1);
      parameters.perspectiveZPlanes = zPlanes;
      parameters.perspectiveFOV = FieldOfView;
      parameters.samplingFactor = 1.0f;
      parameters.superSamplingFactor = superSampling;
      parameters.observer = &observer;
      parameters.camera = &camera;

      Raytracer raytracer{};
      std::unique_lock lock(observer.mutex);
      raytracer.trigger(parameters);
      observer.rendered.wait(lock, [&] { return observer.image != nullptr; });
      return Float4(observer.image->getData());
    }

    // The mean colour of a square image of the resolution that sees only the panel of renderWallCenter's scenes, and
    // the spread of its pixels' red (largest minus smallest); denoised, it samples (an area light's rays: the scene
    // has none) and denoises
    static Float4 renderPanelMean(Scene &scene, const ASizeT resolution, const ASizeT superSampling,
                                  const bool denoised, Float &redSpread) {
      scene.buildSceneGraph();
      constexpr Float FieldOfView = 0.4f;
      const auto zPlanes = Float2(0.01f, 2.0f);
      Camera camera{};
      camera.setProjection(FieldOfView, Float2(2.0f), zPlanes);
      camera.translate(Float3(0.0f, 0.0f, -3.2f));

      ImageObserver observer{};
      RaytraceParameters parameters{};
      parameters.visibilityCutoff = 1.0f / 512.0f;
      parameters.maxTraceDepth = 4;
      parameters.sceneShader = &scene;
      parameters.cullingOrientation = -1;
      parameters.resolution = Size2(resolution, resolution);
      parameters.perspectiveZPlanes = zPlanes;
      parameters.perspectiveFOV = FieldOfView;
      parameters.samplingFactor = 1.0f;
      parameters.superSamplingFactor = superSampling;
      parameters.sphereLightSamples = denoised ? 1 : 0;
      parameters.denoise = denoised;
      parameters.observer = &observer;
      parameters.camera = &camera;

      Raytracer raytracer{};
      std::unique_lock lock(observer.mutex);
      raytracer.trigger(parameters);
      observer.rendered.wait(lock, [&] { return observer.image != nullptr; });
      auto sum = Zero<Float4>();
      auto lowest = std::numeric_limits<Float>::max();
      auto highest = std::numeric_limits<Float>::lowest();
      for (auto i = Zero<ASizeT>(); i < resolution * resolution; ++i) {
        const auto pixel = Float4(observer.image->getData() + i);
        sum += pixel;
        lowest = std::min(lowest, x(pixel));
        highest = std::max(highest, x(pixel));
      }
      redSpread = highest - lowest;
      return sum / Float4(static_cast<Float>(resolution * resolution));
    }

    static Float rgbSum(const Float4 &rgb) {
      return x(rgb) + y(rgb) + z(rgb);
    }

  public:
    TEST_METHOD(diffuseRaysEstimateAnEmittersIrradianceAsALightsFormFactor) {
      // a box light of the square lights the wall by its exact form factor, the square as an emissive object (no
      // light) by the diffuse rays that find it: the same irradiance
      Scene lit{};
      setUpEmpty(lit);
      lit.getSceneObjects().push_back(wall());
      lit.getLights().push_back(squareLight());
      const auto byLight = renderWallCenter(lit, 0);

      Scene glowing{};
      setUpEmpty(glowing);
      glowing.getSceneObjects().push_back(wall());
      glowing.getSceneObjects().push_back(squareEmitter());
      const auto byDiffuseRays = renderWallCenter(glowing, 64);

      Assert::IsTrue(rgbSum(byLight) > 0.1f, L"the light reaches the wall", LINE_INFO());
      Assert::AreEqual(1.0f, rgbSum(byDiffuseRays) / rgbSum(byLight), 0.02f, L"the same irradiance", LINE_INFO());
      Assert::AreEqual(0.0f, rgbSum(renderWallCenter(glowing, 0)), 1e-6f, L"no light without diffuse rays",
                       LINE_INFO());
    }

    TEST_METHOD(lightsHighlightIsTheGlossyReflectionOfAnEmitterOfItsRadianceAndShape) {
      // One specular BRDF: the wall's highlight of the square box light (shadow rays alone) and its glossy reflection
      // of the emissive square of the same radiance (glossy rays alone) agree, for a lobe of the weight 0.5 and F0 0.5
      // (Schlick's per microfacet in both) and for a dielectric interface (the indices' Fresnel)
      const auto render = [](std::unique_ptr<SceneObject> wallObject, const bool light) {
        Scene scene{};
        setUpEmpty(scene);
        scene.getSceneObjects().push_back(std::move(wallObject));
        if (light) {
          scene.getLights().push_back(squareLight());
        } else {
          scene.getSceneObjects().push_back(squareEmitter());
        }
        RaytraceParameters parameters{};
        // the glossy rays are many more: they shrink with the lobe's reflectance (0.04 of glass at normal incidence)
        parameters.sphereLightSamples = light ? 256 : 0;
        parameters.glossySamples = light ? 0 : 2048;
        return rgbSum(renderWallCenter(scene, parameters, 15));
      };

      const auto highlight = render(glossyWall(0.5f, 0.5f, 0.5f), true);
      const auto reflection = render(glossyWall(0.5f, 0.5f, 0.5f), false);
      Assert::IsTrue(highlight > 0.01f, L"a highlight", LINE_INFO());
      Assert::AreEqual(1.0f, reflection / highlight, 0.03f, L"opaque: the same light", LINE_INFO());

      // dark glass, seen from outside: it absorbs what it refracts (an absorption length of a 20th of the wall's
      // thickness), so that the pane shows no second reflection of its back, which the shadow rays don't sample
      const auto glass = [] {
        auto object = glossyWall(1.0f, 0.0f, 0.5f);
        object->setTransmittanceShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(1e-4f, 1e-4f, 1e-4f, 0.0f)));
        object->setRefractionEtaShader(std::make_unique<Resources::ConstMaterialShader>(
            RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Glass)
        ));
        return object;
      };
      const auto glassHighlight = render(glass(), true);
      const auto glassReflection = render(glass(), false);
      Assert::IsTrue(glassHighlight > 0.001f, L"a highlight of glass", LINE_INFO());
      Assert::AreEqual(1.0f, glassReflection / glassHighlight, 0.03f, L"glass: the same light", LINE_INFO());
    }

    TEST_METHOD(coatedWhiteSurfaceReflectsAtMostTheLightAroundIt) {
      // White furnace: a white diffuse base under a specular lobe of F0 0.04 in a uniform sky of
      // white (the background and the ambient light): it reflects at most the sky's radiance at every angle, also
      // where the lobe reflects most (towards grazing angles), and about all of it; a mirror lobe (Whitted) as well
      const auto render = [](const Float tilt, const Float roughness, const bool withWall) {
        Scene scene{};
        setUpEmpty(scene, RGBS(1.0f, 1.0f, 1.0f), RGBS(1.0f, 1.0f, 1.0f));
        if (withWall) {
          auto object = std::make_unique<SceneObject>("coated wall");
          object->setForm(std::make_unique<Box>());
          object->scale(Float3(2.0f, 2.0f, 0.001f));
          object->rotate(Float3(0.0f, tilt, 0.0f));
          object->translate(Float3(0.0f, 0.0f, -1.0f));
          const auto none = Zero<Float4>();
          object->setDiffusionShader(
              std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 0.0f)));
          object->setReflectanceShader(
              std::make_unique<Resources::ConstMaterialShader>(Float4(1.0f, 1.0f, 1.0f, 0.0f)));
          object->setSpecularShader(
              std::make_unique<Resources::ConstMaterialShader>(Float4(0.04f, 0.04f, 0.04f, 0.0f)));
          object->setRoughnessShader(
              std::make_unique<Resources::ConstMaterialShader>(Float4(roughness, roughness, roughness, 0.0f)));
          object->setEmittanceShader(std::make_unique<Resources::ConstMaterialShader>(none));
          object->setTransmittanceShader(std::make_unique<Resources::ConstMaterialShader>(none));
          object->setRefractionEtaShader(std::make_unique<Resources::ConstMaterialShader>(
              RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Vacuum)
          ));
          scene.getSceneObjects().push_back(std::move(object));
        } else {
          scene.getSceneObjects().push_back(panel(Float3(0.1f, 0.1f, 0.1f), Float3(50.0f, 0.0f, 0.0f), Zero<Float4>()));
        }
        RaytraceParameters parameters{};
        parameters.glossySamples = 64;
        return rgbSum(renderWallCenter(scene, parameters, 3));
      };

      const auto sky = render(0.0f, 0.0f, false);
      Assert::IsTrue(sky > 0.5f, L"the sky", LINE_INFO());
      for (const auto roughness : {0.0f, 0.3f}) {
        for (const auto tilt : {0.0f, 60.0f, 80.0f}) {
          const auto albedo = render(tilt, roughness, true) / sky;
          Assert::IsTrue(albedo < 1.005f, L"at most the sky", LINE_INFO());
          Assert::IsTrue(albedo > 0.96f, L"about all of it", LINE_INFO());
        }
      }
    }

    TEST_METHOD(roughWhiteConductorReflectsAllTheLightAroundIt) {
      // White furnace: a conductor of F0 1 (white) in a uniform white sky reflects about all of it at
      // every angle and roughness, with the multiple scattering between its microfacets compensated (single
      // scattering loses up to half at roughness 1 and grazing angles)
      const auto render = [](const Float tilt, const Float roughness, const bool withWall) {
        Scene scene{};
        setUpEmpty(scene, RGBS(1.0f, 1.0f, 1.0f));
        if (withWall) {
          auto object = glossyWall(1.0f, 1.0f, roughness);
          object->rotate(Float3(0.0f, tilt, 0.0f));
          scene.getSceneObjects().push_back(std::move(object));
        } else {
          scene.getSceneObjects().push_back(panel(Float3(0.1f, 0.1f, 0.1f), Float3(50.0f, 0.0f, 0.0f), Zero<Float4>()));
        }
        RaytraceParameters parameters{};
        parameters.glossySamples = 256;
        return rgbSum(renderWallCenter(scene, parameters, 3));
      };

      const auto sky = render(0.0f, 0.0f, false);
      for (const auto roughness : {0.3f, 0.7f, 1.0f}) {
        for (const auto tilt : {0.0f, 60.0f, 80.0f}) {
          Assert::AreEqual(1.0f, render(tilt, roughness, true) / sky, 0.02f, L"all of it", LINE_INFO());
        }
      }
    }

    TEST_METHOD(roughGlassPassesAndReflectsAllTheLightAroundIt) {
      // White furnace: a thin pane of rough clear glass in a uniform white sky shows the sky, what its faces reflect
      // and transmit (and reflect totally inside) adding up, with the multiple scattering between the microfacets
      // compensated
      const auto render = [](const bool withPane) {
        Scene scene{};
        setUpEmpty(scene, RGBS(1.0f, 1.0f, 1.0f));
        if (withPane) {
          auto pane = glossyWall(1.0f, 0.0f, 0.7f);
          pane->rotate(Float3(0.0f, 30.0f, 0.0f));
          pane->setTransmittanceShader(
              std::make_unique<Resources::ConstMaterialShader>(Float4(1e6f, 1e6f, 1e6f, 0.0f)));
          pane->setRefractionEtaShader(std::make_unique<Resources::ConstMaterialShader>(
              RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Glass)
          ));
          scene.getSceneObjects().push_back(std::move(pane));
        } else {
          scene.getSceneObjects().push_back(panel(Float3(0.1f, 0.1f, 0.1f), Float3(50.0f, 0.0f, 0.0f), Zero<Float4>()));
        }
        RaytraceParameters parameters{};
        parameters.glossySamples = 64;
        parameters.maxTraceDepth = 16;
        return rgbSum(renderWallCenter(scene, parameters, 3));
      };
      Assert::AreEqual(1.0f, render(true) / render(false), 0.03f, L"the sky", LINE_INFO());
    }

    TEST_METHOD(transmissionSharesWhatTheLobeLeavesWithTheDiffusion) {
      // a thin pane of clear glass in front of an emitting wall: the diffusion 0.5 halves what passes each of its
      // faces (the pane reflects the black sky)
      const auto render = [](const Float diffusion) {
        Scene scene{};
        setUpEmpty(scene);
        scene.getSceneObjects().push_back(
            panel(Float3(2.0f, 2.0f, 0.001f), Float3(0.0f, 0.0f, -2.0f), Zero<Float4>(), Float4(1.0f, 1.0f, 1.0f, 0.0f))
        );
        auto pane = glossyWall(1.0f, 0.0f, 0.0f);
        pane->setDiffusionShader(
            std::make_unique<Resources::ConstMaterialShader>(Float4(diffusion, diffusion, diffusion, 0.0f)));
        pane->setTransmittanceShader(std::make_unique<Resources::ConstMaterialShader>(Float4(1e6f, 1e6f, 1e6f, 0.0f)));
        pane->setRefractionEtaShader(std::make_unique<Resources::ConstMaterialShader>(
            RefractionIndices::refractionEta(RefractionIndices::Vacuum, RefractionIndices::Glass)
        ));
        scene.getSceneObjects().push_back(std::move(pane));
        return rgbSum(renderWallCenter(scene, 0, false, 0));
      };
      const auto clear = render(0.0f);
      Assert::IsTrue(clear > 0.5f, L"the wall through the pane", LINE_INFO());
      Assert::AreEqual(sqr(1.0f - 0.5f), render(0.5f) / clear, 0.005f, L"half at each face", LINE_INFO());
    }

    TEST_METHOD(aFloorDoesNotShadowADirectionalLightAtItsHorizon) {
      // A directional light along the view, at the horizon of an infinite floor below the wall: a plane rotated by -90
      // degrees. The wall's shadow ray runs parallel to the floor and reaches the light; when cos 90 degrees rounded
      // to -4.4e-8, the tilted floor met it 3e7 away.
      const auto render = [](const bool withFloor) {
        Scene scene{};
        setUpEmpty(scene);
        scene.getSceneObjects().push_back(wall());
        if (withFloor) {
          auto floor = panel(Float3(1.0f, 1.0f, 1.0f), Float3(0.0f, -1.5f, 0.0f), Float4(1.0f, 1.0f, 1.0f, 0.0f));
          floor->setForm(std::make_unique<Plane>());
          floor->rotate(Float3(-90.0f, 0.0f, 0.0f));
          scene.getSceneObjects().push_back(std::move(floor));
        }
        auto light = std::make_unique<LightInfo>();
        light->emittance = RGBS(1.0f, 1.0f, 1.0f);
        light->direction = Float4(0.0f, 0.0f, 1.0f, 0.0f);
        scene.getLights().push_back(std::move(light));
        return rgbSum(renderWallCenter(scene, 0, false, 0));
      };
      const auto open = render(false);
      Assert::IsTrue(open > 0.1f, L"the light reaches the wall", LINE_INFO());
      Assert::AreEqual(open, render(true), 1e-6f, L"also above the floor", LINE_INFO());
    }

    TEST_METHOD(glossyRaysSeeTheMaterialOfAnEmittersFacesThatDontEmit) {
      // the square light emits upwards, away from the wall below it: the wall's glossy reflection of the emitter's
      // bottom (a grey housing under the ambient light) is that of the same box as a plain object, not black
      const auto render = [](const bool asLight) {
        Scene scene{};
        setUpEmpty(scene, RGBS::black(), RGBS(1.0f, 1.0f, 1.0f));
        scene.getSceneObjects().push_back(glossyWall(1.0f, 1.0f, 0.3f));
        auto emitter = squareEmitter();
        emitter->setEmittanceShader(std::make_unique<Resources::BoxFacesMaterialShader>(
            LightInfo::faceBit(1, true), Float4(1.0f, 1.0f, 1.0f, 0.0f), Zero<Float4>()
        ));
        emitter->setDiffusionShader(std::make_unique<Resources::BoxFacesMaterialShader>(
            LightInfo::faceBit(1, true), Zero<Float4>(), Float4(0.6f, 0.6f, 0.6f, 0.0f)
        ));
        auto light = squareLight();
        light->emittingFaces = LightInfo::faceBit(1, true);
        if (asLight) {
          emitter->setLightEmitter(light.get());
          scene.getLights().push_back(std::move(light));
        }
        scene.getSceneObjects().push_back(std::move(emitter));
        RaytraceParameters parameters{};
        parameters.glossySamples = 64;
        parameters.sphereLightSamples = 16;
        return rgbSum(renderWallCenter(scene, parameters, 3));
      };
      const auto plain = render(false);
      Assert::IsTrue(plain > 0.01f, L"the housing in the reflection", LINE_INFO());
      Assert::AreEqual(plain, render(true), 1e-4f * plain, L"the same as a light's emitter", LINE_INFO());
    }

    TEST_METHOD(diffuseRaysSkipTheEmissiveObjectsOfAreaLights) {
      // the shadow rays bring the light's light; a diffuse ray that finds its emitter adds nothing
      Scene scene{};
      setUpEmpty(scene);
      scene.getSceneObjects().push_back(wall());
      auto light = squareLight();
      auto emitter = squareEmitter();
      emitter->setLightEmitter(light.get());
      scene.getSceneObjects().push_back(std::move(emitter));
      scene.getLights().push_back(std::move(light));

      const auto withoutDiffuseRays = renderWallCenter(scene, 0);
      const auto withDiffuseRays = renderWallCenter(scene, 64);
      Assert::IsTrue(rgbSum(withoutDiffuseRays) > 0.1f, L"the light reaches the wall", LINE_INFO());
      Assert::AreEqual(rgbSum(withoutDiffuseRays), rgbSum(withDiffuseRays), 1e-6f, L"counted once", LINE_INFO());
    }

    TEST_METHOD(whiteSurfaceUnderAUniformSkyShowsTheSkysRadiance) {
      // the diffuse rays replace the ambient light with what they find, here the background in every direction
      Scene scene{};
      setUpEmpty(scene, RGBS(0.5f, 0.5f, 0.5f), RGBS(0.2f, 0.2f, 0.2f));
      scene.getSceneObjects().push_back(wall());

      const auto ambient = renderWallCenter(scene, 0);
      const auto sky = renderWallCenter(scene, 16);
      Assert::IsTrue(rgbSum(ambient) > 0.1f, L"the ambient light without diffuse rays", LINE_INFO());
      Assert::AreEqual(2.5f, rgbSum(sky) / rgbSum(ambient), 1e-3f, L"the sky instead of the ambient light",
                       LINE_INFO());
    }

    TEST_METHOD(hitsOfDiffuseRaysKeepTheAmbientLight) {
      // One bounce: the plate's hits trace no diffuse rays and show the ambient light, so the wall's center reflects
      // the ambient light times the plate's form factor (the projected solid angle of its face over pi): the plate's
      // white reflects all of it.
      Scene scene{};
      setUpEmpty(scene, RGBS::black(), RGBS(0.2f, 0.2f, 0.2f));
      scene.getSceneObjects().push_back(wall());
      scene.getSceneObjects().push_back(plate(Float4(1.0f, 1.0f, 1.0f, 0.0f)));

      LightInfo plateFace{};
      plateFace.position = Float4(PlateX, 0.0f, 0.002f, 1.0f);
      plateFace.halfExtents = Float4(0.0f, 2.0f, 0.999f, 0.0f);
      plateFace.emittingFaces = LightInfo::faceBit(0, false);
      const auto formFactor = BoxLightFaces(plateFace, Float4(0.0f, 0.0f, WallZ, 1.0f), Float4(0.0f, 0.0f, 1.0f, 0.0f))
                                  .projectedSolidAngle / Pi<Float>();

      const auto ambient = renderWallCenter(scene, 0);
      const auto bounced = renderWallCenter(scene, 64);
      Assert::IsTrue(formFactor > 0.1f && formFactor < 0.5f, L"the plate fills part of the hemisphere", LINE_INFO());
      Assert::AreEqual(formFactor, rgbSum(bounced) / rgbSum(ambient), 0.02f * formFactor, L"one bounce",
                       LINE_INFO());
    }

    TEST_METHOD(irradianceRecordsGiveTheOneBounceOfTheirRays) {
      // the wall's center of hitsOfDiffuseRaysKeepTheAmbientLight from irradiance records (each sub-sample's own, at
      // its hit) instead of its diffuse rays: the same irradiance, from other rays
      Scene scene{};
      setUpEmpty(scene, RGBS::black(), RGBS(0.2f, 0.2f, 0.2f));
      scene.getSceneObjects().push_back(wall());
      scene.getSceneObjects().push_back(plate(Float4(1.0f, 1.0f, 1.0f, 0.0f)));

      const auto traced = renderWallCenter(scene, 64);
      const auto cached = renderWallCenter(scene, 64, true);
      Assert::AreEqual(1.0f, rgbSum(cached) / rgbSum(traced), 0.02f, L"the same irradiance", LINE_INFO());
      Assert::AreNotEqual(rgbSum(traced), rgbSum(cached), L"from the records' rays", LINE_INFO());
    }

    TEST_METHOD(subSampleCountsAgreeOnSaturatedColours) {
      // The sub-samples' hero wavelengths stratify one wavelength step: 2 x 2, 3 x 3 and 4 x 4 sub-samples take 32 or
      // more wavelengths, and a saturated red comes out the same. Strata of the whole visible range gave 2 x 2 the 8
      // wavelengths of a single sub-sample, 10 % darker.
      const auto render = [](const ASizeT superSampling) {
        Scene scene{};
        setUpEmpty(scene, RGBS::black(), RGBS(1.0f, 1.0f, 1.0f));
        scene.getSceneObjects().push_back(
            panel(Float3(2.0f, 2.0f, 0.001f), Float3(0.0f, 0.0f, -1.0f), Float4(0.9f, 0.05f, 0.05f, 0.0f))
        );
        return renderWallCenter(scene, 0, false, superSampling);
      };
      const auto twoByTwo = render(1);
      const auto threeByThree = render(2);
      const auto fourByFour = render(3);
      Assert::AreEqual(x(fourByFour), x(twoByTwo), 0.01f * x(fourByFour), L"2 x 2", LINE_INFO());
      Assert::AreEqual(x(fourByFour), x(threeByThree), 0.01f * x(fourByFour), L"3 x 3", LINE_INFO());
    }

    TEST_METHOD(denoisedPreviewPixelsVaryTheirWavelengths) {
      // A single sub-sample per pixel (the fast preview) takes one of 8 hero wavelengths per pixel where the denoiser
      // averages them, so that every 5 x 5 pixels take 64 wavelengths: the saturated red's mean comes out as from
      // 4 x 4 sub-samples. With the same 8 wavelengths in every pixel, it came out 6 % brighter in the Cornell box's
      // red wall. Undenoised, every pixel keeps the same 8 (their dither would show).
      const auto render = [](const ASizeT superSampling, const bool denoised, Float &redSpread) {
        Scene scene{};
        setUpEmpty(scene, RGBS::black(), RGBS(1.0f, 1.0f, 1.0f));
        scene.getSceneObjects().push_back(
            panel(Float3(2.0f, 2.0f, 0.001f), Float3(0.0f, 0.0f, -1.0f), Float4(0.9f, 0.05f, 0.05f, 0.0f))
        );
        return renderPanelMean(scene, 16, superSampling, denoised, redSpread);
      };
      auto spread = 0.0f;
      const auto single = render(0, true, spread);
      const auto fourByFour = render(3, true, spread);
      Assert::AreEqual(x(fourByFour), x(single), 0.005f * x(fourByFour), L"red", LINE_INFO());
      Assert::AreEqual(y(fourByFour), y(single), 0.02f * y(fourByFour), L"green", LINE_INFO());
      render(0, false, spread);
      Assert::AreEqual(0.0f, spread, L"undenoised: the same wavelengths", LINE_INFO());
    }

    TEST_METHOD(colourBleedsFromARedPlateOntoTheWhiteWall) {
      // a white light at (-0.2, 0, 0) lights the wall's center and the red plate beside it (the pipeline shows the
      // flat spectrum of white light as a warm white: red over green is above 1 already)
      const auto render = [](const ASizeT diffuseSamples) {
        Scene scene{};
        setUpEmpty(scene);
        scene.getSceneObjects().push_back(wall());
        scene.getSceneObjects().push_back(plate(Float4(1.0f, 0.0f, 0.0f, 0.0f)));
        auto light = std::make_unique<LightInfo>();
        light->position = Float4(-0.2f, 0.0f, 0.0f, 1.0f);
        light->emittance = RGBS(1.0f, 1.0f, 1.0f);
        scene.getLights().push_back(std::move(light));
        return renderWallCenter(scene, diffuseSamples);
      };

      const auto direct = render(0);
      const auto bled = render(64);
      Assert::IsTrue(x(bled) / y(bled) > 1.1f * x(direct) / y(direct), L"redder with diffuse rays", LINE_INFO());
      Assert::AreEqual(y(direct), y(bled), 0.02f * y(direct), L"the red plate adds no green", LINE_INFO());
    }
  };
}
