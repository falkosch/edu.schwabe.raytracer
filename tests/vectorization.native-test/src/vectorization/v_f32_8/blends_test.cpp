#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f32_8_BlendsTest) {
  public:
    TEST_METHOD(allFalseReturnsA) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_f32_8 b(10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f);
      const auto r = blend<false, false, false, false, false, false, false, false>(a, b);
      Assert::IsTrue(allTrue(a == r), L"all false = a", LINE_INFO());
    }

    TEST_METHOD(allTrueReturnsB) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_f32_8 b(10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f);
      const auto r = blend<true, true, true, true, true, true, true, true>(a, b);
      Assert::IsTrue(allTrue(b == r), L"all true = b", LINE_INFO());
    }

    TEST_METHOD(selectiveBlend) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_f32_8 b(10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f);
      const auto r = blend<true, false, true, false, true, false, true, false>(a, b);
      Assert::AreEqual(10.0f, x1(r), L"x1 from b", LINE_INFO());
      Assert::AreEqual(2.0f, x2(r), L"x2 from a", LINE_INFO());
      Assert::AreEqual(30.0f, x3(r), L"x3 from b", LINE_INFO());
      Assert::AreEqual(4.0f, x4(r), L"x4 from a", LINE_INFO());
      Assert::AreEqual(50.0f, x5(r), L"x5 from b", LINE_INFO());
      Assert::AreEqual(6.0f, x6(r), L"x6 from a", LINE_INFO());
    }

    TEST_METHOD(blendMaskedWithVectorBoolType) {
      const v_f32_8 a(1.0f);
      const v_f32_8 b(2.0f);
      const v_i32_8 mask = v_f32_8(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f) != Zero<v_f32_8>();
      const auto r = blendMasked(a, b, mask);
      Assert::AreEqual(1.0f, x1(r), L"x1 mask off", LINE_INFO());
      Assert::AreEqual(2.0f, x2(r), L"x2 mask on", LINE_INFO());
    }

    TEST_METHOD(blendMaskedWithFloatMask) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_f32_8 b(10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f);
      // Create a float mask where some lanes have all bits set (MaskAll) and others are zero
      const v_f32_8 floatMask = blend<true, false, true, false, true, false, true, false>(
          Zero<v_f32_8>(), MaskAll<v_f32_8>()
      );
      const auto r = blendMasked(a, b, floatMask);
      Assert::AreEqual(10.0f, x1(r), L"x1 mask on -> b", LINE_INFO());
      Assert::AreEqual(2.0f, x2(r), L"x2 mask off -> a", LINE_INFO());
      Assert::AreEqual(30.0f, x3(r), L"x3 mask on -> b", LINE_INFO());
      Assert::AreEqual(4.0f, x4(r), L"x4 mask off -> a", LINE_INFO());
      Assert::AreEqual(50.0f, x5(r), L"x5 mask on -> b", LINE_INFO());
      Assert::AreEqual(6.0f, x6(r), L"x6 mask off -> a", LINE_INFO());
      Assert::AreEqual(70.0f, x7(r), L"x7 mask on -> b", LINE_INFO());
      Assert::AreEqual(8.0f, x8(r), L"x8 mask off -> a", LINE_INFO());
    }
  };
}
