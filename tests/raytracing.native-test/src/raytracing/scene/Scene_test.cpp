#include "raytracing_tests.h"

#include <raytracing/geometry/forms/Box.h>
#include <raytracing/geometry/forms/Sphere.h>
#include <raytracing/geometry/partitioning/treebalancer/BruteForceSAHKDTreeBalancer.h>
#include <raytracing/geometry/partitioning/treetraverser/WideKDTreeTraverser.h>
#include <raytracing/scene/Scene.h>
#include <raytracing/shading/LightInfo.h>

#include <memory>

namespace raytracer::test {
  TEST_CLASS(SceneTest) {
    // A scene with a culling tree, like raytracerui's
    static std::unique_ptr<Scene> treeScene() {
      return std::make_unique<Scene>(
          std::make_unique<WideKDTreeTraverser<SceneIntersection>>(), std::make_unique<BruteForceSAHKDTreeBalancer>()
      );
    }

    // A sphere light of radius 0.25 at the center, and the sphere showing it
    static LightInfo &addSphereLight(Scene &scene, const std::string &id, const Float3 &center) {
      auto light = std::make_unique<LightInfo>();
      light->position = oneW(center);
      light->radius = 0.25f;
      auto emitter = std::make_unique<SceneObject>(id);
      emitter->setForm(std::make_unique<Sphere>());
      emitter->scale(Float3(0.25f, 0.25f, 0.25f));
      emitter->translate(center);
      emitter->setLightEmitter(light.get());
      auto &result = *light;
      scene.getSceneObjects().push_back(std::move(emitter));
      scene.getLights().push_back(std::move(light));
      return result;
    }

    // The id of the object that a ray from (x, y, -5) along +z meets first, empty for none
    static std::string objectAlongZ(const Scene &scene, const Float x, const Float y) {
      const Ray ray{Float4(x, y, -5.0f, 1.0f), Float4(0.0f, 0.0f, 1.0f, 0.0f)};
      const RayCast rayCast{ray, Zero<Float4::VectorBoolType>(), Size2(0, 0), 100.0f};
      SceneIntersection intersection{};
      intersection.object = nullptr;
      scene.findNearestIntersection(rayCast, nullptr, intersection);
      const auto object = dynamic_cast<const SceneObject *>(intersection.object);
      return object ? object->getId() : std::string();
    }

  public:
    TEST_METHOD(translatingALightMovesItsEmitterAlong) {
      const auto scene = treeScene();
      auto &moved = addSphereLight(*scene, "moved", Float3(0.0f, 0.0f, 0.0f));
      const auto &other = addSphereLight(*scene, "other", Float3(-2.0f, 0.0f, 0.0f));
      scene->buildSceneGraph();

      scene->translateLight(moved, Float3(2.0f, 0.0f, 0.0f));

      Assert::AreEqual(2.0f, x(moved.position), L"the light", LINE_INFO());
      Assert::AreEqual(1.0f, w(moved.position), L"still a point", LINE_INFO());
      Assert::AreEqual(-2.0f, x(other.position), L"the other light stays", LINE_INFO());
      // the emitter left the culling tree's bounds of its first place: found there, the tree was rebuilt
      Assert::AreEqual(std::string("moved"), objectAlongZ(*scene, 2.0f, 0.0f), L"the emitter's new place", LINE_INFO());
      Assert::AreEqual(std::string(), objectAlongZ(*scene, 0.0f, 0.0f), L"its old place", LINE_INFO());
      Assert::AreEqual(std::string("other"), objectAlongZ(*scene, -2.0f, 0.0f), L"the other emitter", LINE_INFO());
    }

    TEST_METHOD(translatingAPointLightLeavesTheObjectsInPlace) {
      const auto scene = treeScene();
      auto box = std::make_unique<SceneObject>("box");
      box->setForm(std::make_unique<Box>());
      box->scale(Float3(0.25f, 0.25f, 0.25f));
      scene->getSceneObjects().push_back(std::move(box));
      auto light = std::make_unique<LightInfo>();
      auto &pointLight = *light;
      scene->getLights().push_back(std::move(light));
      scene->buildSceneGraph();

      scene->translateLight(pointLight, Float3(0.0f, 1.0f, 0.0f));

      Assert::AreEqual(1.0f, y(pointLight.position), L"the light", LINE_INFO());
      Assert::AreEqual(std::string("box"), objectAlongZ(*scene, 0.0f, 0.0f), L"the box stays", LINE_INFO());
    }

    TEST_METHOD(aDirectionalLightDoesNotMove) {
      Scene scene{};
      auto light = std::make_unique<LightInfo>();
      light->direction = Float4(0.0f, 1.0f, 0.0f, 0.0f);
      auto &directional = *light;
      scene.getLights().push_back(std::move(light));

      scene.translateLight(directional, Float3(0.0f, 0.0f, 1.0f));

      Assert::AreEqual(0.0f, z(directional.position), L"the unused position", LINE_INFO());
      Assert::AreEqual(1.0f, y(directional.direction), L"the direction", LINE_INFO());
    }
  };
}
