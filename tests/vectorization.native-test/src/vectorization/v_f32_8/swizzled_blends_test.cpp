#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f32_8_SwizzledBlendsTest) {
  public:
    TEST_METHOD(swizzledBlendCombinesSwizzleAndBlend) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_f32_8 b(10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f);
      const auto r = swizzledBlend<
          VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X,
          true, false, true, false, true, false, true, false
      >(a, b);
      Assert::AreEqual(40.0f, x1(r), L"x1: b swizzled W=40, selected", LINE_INFO());
      Assert::AreEqual(2.0f, x2(r), L"x2: from a", LINE_INFO());
      Assert::AreEqual(20.0f, x3(r), L"x3: b swizzled Y=20, selected", LINE_INFO());
      Assert::AreEqual(4.0f, x4(r), L"x4: from a", LINE_INFO());
    }
  };
}
