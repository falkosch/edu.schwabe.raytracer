#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f32_8_SelectsTest) {
  public:
    TEST_METHOD(selectBoolTrueReturnsOnTrue) {
      const v_f32_8 a(1.0f);
      const v_f32_8 b(2.0f);
      const auto r = select(true, a, b);
      Assert::AreEqual(1.0f, x1(r), L"true -> onTrue", LINE_INFO());
    }

    TEST_METHOD(selectBoolFalseReturnsOnFalse) {
      const v_f32_8 a(1.0f);
      const v_f32_8 b(2.0f);
      const auto r = select(false, a, b);
      Assert::AreEqual(2.0f, x1(r), L"false -> onFalse", LINE_INFO());
    }

    TEST_METHOD(selectVectorBoolTypeMask) {
      const v_f32_8 a(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
      const v_f32_8 b(2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f);
      const v_f32_8 cond(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
      const auto mask = cond != Zero<v_f32_8>();
      const auto r = select(mask, b, a);
      Assert::AreEqual(1.0f, x1(r), L"x1 false->a", LINE_INFO());
      Assert::AreEqual(2.0f, x2(r), L"x2 true->b", LINE_INFO());
      Assert::AreEqual(1.0f, x5(r), L"x5 false->a", LINE_INFO());
      Assert::AreEqual(2.0f, x6(r), L"x6 true->b", LINE_INFO());
    }

    TEST_METHOD(selectWithFloatMask) {
      const v_f32_8 a(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
      const v_f32_8 b(2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f);
      // Use MaskAll for lanes that should select onTrue, Zero for onFalse
      // Create a float mask by comparing: non-zero lanes have all bits set
      const v_f32_8 condVals(0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f);
      // The float mask overload uses the sign bit / all-bits interpretation
      // We can construct a proper float mask from a comparison result cast
      const v_f32_8 floatMask = blend<false, true, false, true, false, true, false, true>(
          Zero<v_f32_8>(), MaskAll<v_f32_8>()
      );
      const auto r = select(floatMask, b, a);
      Assert::AreEqual(1.0f, x1(r), L"x1 zero mask -> onFalse", LINE_INFO());
      Assert::AreEqual(2.0f, x2(r), L"x2 all-bits mask -> onTrue", LINE_INFO());
      Assert::AreEqual(1.0f, x3(r), L"x3 zero mask -> onFalse", LINE_INFO());
      Assert::AreEqual(2.0f, x4(r), L"x4 all-bits mask -> onTrue", LINE_INFO());
      Assert::AreEqual(1.0f, x5(r), L"x5 zero mask -> onFalse", LINE_INFO());
      Assert::AreEqual(2.0f, x6(r), L"x6 all-bits mask -> onTrue", LINE_INFO());
      Assert::AreEqual(1.0f, x7(r), L"x7 zero mask -> onFalse", LINE_INFO());
      Assert::AreEqual(2.0f, x8(r), L"x8 all-bits mask -> onTrue", LINE_INFO());
    }
  };
}
