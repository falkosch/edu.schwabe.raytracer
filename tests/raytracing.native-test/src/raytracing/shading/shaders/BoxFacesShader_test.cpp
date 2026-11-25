#include "raytracing_tests.h"

#include <raytracing/scene/Resources.h>
#include <raytracing/scene/SceneObject.h>
#include <raytracing/shading/LightInfo.h>

namespace raytracer::test {
  TEST_CLASS(BoxFacesShaderTest) {
    // The shader's value at the face of the model-space normal
    static Float4 atFace(const Resources::BoxFacesMaterialShader &shader, const Float4 &normal) {
      const SceneObject object{"box"};
      FacetIntersection intersection{};
      intersection.msSurfaceNormal = normal;
      return shader.sample(object, intersection);
    }

  public:
    TEST_METHOD(onlyTheMaskedFacesTakeTheirValue) {
      // a box light emitting downwards and to the front: its other faces see the other value
      const auto shader = Resources::BoxFacesMaterialShader(
          LightInfo::faceBit(1, false) | LightInfo::faceBit(2, true), One<Float4>(), Zero<Float4>()
      );
      Assert::AreEqual(1.0f, x(atFace(shader, Float4(0.0f, -1.0f, 0.0f, 0.0f))), L"bottom", LINE_INFO());
      Assert::AreEqual(1.0f, x(atFace(shader, Float4(0.0f, 0.0f, 1.0f, 0.0f))), L"front", LINE_INFO());
      Assert::AreEqual(0.0f, x(atFace(shader, Float4(0.0f, 1.0f, 0.0f, 0.0f))), L"top", LINE_INFO());
      Assert::AreEqual(0.0f, x(atFace(shader, Float4(0.0f, 0.0f, -1.0f, 0.0f))), L"back", LINE_INFO());
      Assert::AreEqual(0.0f, x(atFace(shader, Float4(1.0f, 0.0f, 0.0f, 0.0f))), L"right", LINE_INFO());
      Assert::AreEqual(0.0f, x(atFace(shader, Float4(-1.0f, 0.0f, 0.0f, 0.0f))), L"left", LINE_INFO());
    }

    TEST_METHOD(aNonEmittingFaceOfAnEmitterSeesNoEmittance) {
      SceneObject emitter{"emitter"};
      emitter.setEmittanceShader(std::make_unique<Resources::BoxFacesMaterialShader>(
          LightInfo::faceBit(1, false), Float4(3.0f, 3.0f, 3.0f, 0.0f), Zero<Float4>()
      ));
      FacetIntersection side{};
      side.msSurfaceNormal = Float4(1.0f, 0.0f, 0.0f, 0.0f);
      FacetIntersection bottom{};
      bottom.msSurfaceNormal = Float4(0.0f, -1.0f, 0.0f, 0.0f);
      Assert::IsFalse(emitter.emitsAt(side), L"the side", LINE_INFO());
      Assert::IsTrue(emitter.emitsAt(bottom), L"the emitting bottom", LINE_INFO());
    }
  };
}
