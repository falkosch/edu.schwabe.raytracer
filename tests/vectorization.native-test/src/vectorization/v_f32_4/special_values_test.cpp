#include "vectorization_tests.h"

#include <cmath>

namespace vectorization::test {
  TEST_CLASS(v_f32_4_SpecialValuesTest) {
  public:
    // Special value vector: {-0.0, NaN, inf, -inf}
    static v_f32_4 specialValues() {
      return v_f32_4(
          NegativeZero<Float_32>(), NotANumber<Float_32>(), Infinity<Float_32>(), NegativeInfinity<Float_32>()
      );
    }

    static void assertSpecialValuesPreserved(const v_f32_4 &actual) {
      Assert::IsTrue(std::signbit(x(actual)), L"X: -0.0 sign bit lost", LINE_INFO());
      Assert::AreEqual(0.0f, x(actual), L"X: -0.0 value mismatch", LINE_INFO());
      Assert::IsTrue(std::isnan(y(actual)), L"Y: NaN not preserved", LINE_INFO());
      Assert::AreEqual(Infinity<Float_32>(), z(actual), L"Z: +inf not preserved", LINE_INFO());
      Assert::AreEqual(NegativeInfinity<Float_32>(), w(actual), L"W: -inf not preserved", LINE_INFO());
    }

    // -- Blend preserves special values --

    TEST_METHOD(blendPreservesSpecialValuesWhenSelected) {
      const auto normal = v_f32_4(1.0f, 2.0f, 3.0f, 4.0f);
      const auto special = specialValues();

      // Select all from special
      const auto actual = blend<true, true, true, true>(normal, special);
      assertSpecialValuesPreserved(actual);
    }

    TEST_METHOD(blendPreservesSpecialValuesWhenNotSelected) {
      const auto special = specialValues();
      const auto normal = v_f32_4(1.0f, 2.0f, 3.0f, 4.0f);

      // Select all from special (first arg, all false)
      const auto actual = blend<false, false, false, false>(special, normal);
      assertSpecialValuesPreserved(actual);
    }

    TEST_METHOD(blendPreservesSpecialValuesPerComponent) {
      const auto special = specialValues();
      const auto normal = v_f32_4(1.0f, 2.0f, 3.0f, 4.0f);

      {
        // X from special, rest from normal
        const auto actual = blend<false, true, true, true>(special, normal);
        Assert::IsTrue(std::signbit(x(actual)), L"-0.0 sign bit lost in blend", LINE_INFO());
        Assert::AreEqual(0.0f, x(actual), L"-0.0 value mismatch in blend", LINE_INFO());
      }

      {
        // Y from special, rest from normal
        const auto actual = blend<true, false, true, true>(special, normal);
        Assert::IsTrue(std::isnan(y(actual)), L"NaN not preserved in blend", LINE_INFO());
      }
    }

    TEST_METHOD(blendMaskedPreservesSpecialValues) {
      const auto normal = v_f32_4(1.0f, 2.0f, 3.0f, 4.0f);
      const auto special = specialValues();

      const auto actual = blendMasked(normal, special, MaskAll<v_f32_4>());
      assertSpecialValuesPreserved(actual);
    }

    // -- Select preserves special values --

    TEST_METHOD(selectPreservesSpecialValuesWithBoolMask) {
      const auto special = specialValues();
      const auto normal = v_f32_4(1.0f, 2.0f, 3.0f, 4.0f);

      const auto actual = select(true, special, normal);
      assertSpecialValuesPreserved(actual);
    }

    TEST_METHOD(selectPreservesSpecialValuesWithVectorBoolMask) {
      const auto special = specialValues();
      const auto normal = v_f32_4(1.0f, 2.0f, 3.0f, 4.0f);

      const auto actual = select(v_f32_4::VectorBoolType(v_f32_4::BoolType{42}), special, normal);
      assertSpecialValuesPreserved(actual);
    }

    // -- Swizzle preserves special values --

    TEST_METHOD(swizzleIdentityPreservesSpecialValues) {
      const auto special = specialValues();
      const auto actual = swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(special);
      assertSpecialValuesPreserved(actual);
    }

    TEST_METHOD(swizzleBroadcastPreservesNaN) {
      const auto special = specialValues();
      const auto actual = yyyy(special);

      Assert::IsTrue(std::isnan(x(actual)), L"NaN not preserved after broadcast X", LINE_INFO());
      Assert::IsTrue(std::isnan(y(actual)), L"NaN not preserved after broadcast Y", LINE_INFO());
      Assert::IsTrue(std::isnan(z(actual)), L"NaN not preserved after broadcast Z", LINE_INFO());
      Assert::IsTrue(std::isnan(w(actual)), L"NaN not preserved after broadcast W", LINE_INFO());
    }

    TEST_METHOD(swizzleBroadcastPreservesNegativeZero) {
      const auto special = specialValues();
      const auto actual = xxxx(special);

      for (ASizeT i = 0; i < v_f32_4::SIZE; ++i) {
        Assert::AreEqual(0.0f, actual[i], L"-0.0 value changed after broadcast", LINE_INFO());
        Assert::IsTrue(std::signbit(actual[i]), L"-0.0 sign bit lost after broadcast", LINE_INFO());
      }
    }

    TEST_METHOD(swizzleBroadcastPreservesInfinity) {
      const auto special = specialValues();

      {
        const auto actual = zzzz(special);
        for (ASizeT i = 0; i < v_f32_4::SIZE; ++i) {
          Assert::AreEqual(Infinity<Float_32>(), actual[i], L"+inf not preserved after broadcast", LINE_INFO());
        }
      }

      {
        const auto actual = wwww(special);
        for (ASizeT i = 0; i < v_f32_4::SIZE; ++i) {
          Assert::AreEqual(NegativeInfinity<Float_32>(), actual[i], L"-inf not preserved after broadcast", LINE_INFO());
        }
      }
    }

    TEST_METHOD(swizzleReversePreservesSpecialValues) {
      const auto special = specialValues();
      const auto actual = swizzle<VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X>(special);

      Assert::AreEqual(NegativeInfinity<Float_32>(), x(actual), L"-inf not preserved", LINE_INFO());
      Assert::AreEqual(Infinity<Float_32>(), y(actual), L"+inf not preserved", LINE_INFO());
      Assert::IsTrue(std::isnan(z(actual)), L"NaN not preserved", LINE_INFO());
      Assert::IsTrue(std::signbit(w(actual)), L"-0.0 sign bit lost", LINE_INFO());
      Assert::AreEqual(0.0f, w(actual), L"-0.0 value mismatch", LINE_INFO());
    }

    // -- Constructor roundtrip preserves special values --

    TEST_METHOD(constructorPreservesSpecialValues) {
      const auto special = specialValues();
      const v_f32_4 copy{special};
      assertSpecialValuesPreserved(copy);
    }

    TEST_METHOD(componentAccessPreservesSpecialValues) {
      const auto special = specialValues();

      Assert::IsTrue(std::signbit(component<VectorIndices::X>(special)), L"-0.0 sign bit lost", LINE_INFO());
      Assert::IsTrue(std::isnan(component<VectorIndices::Y>(special)), L"NaN not preserved", LINE_INFO());
      Assert::AreEqual(Infinity<Float_32>(), component<VectorIndices::Z>(special), L"+inf mismatch", LINE_INFO());
      Assert::AreEqual(NegativeInfinity<Float_32>(), component<VectorIndices::W>(special), L"-inf mismatch",
                       LINE_INFO());
    }

    TEST_METHOD(replaceComponentPreservesOtherSpecialValues) {
      const auto special = specialValues();
      const auto actual = replaceX(special, 42.0f);

      Assert::AreEqual(42.0f, x(actual), L"replaced value mismatch", LINE_INFO());
      Assert::IsTrue(std::isnan(y(actual)), L"NaN not preserved after replace", LINE_INFO());
      Assert::AreEqual(Infinity<Float_32>(), z(actual), L"+inf not preserved after replace", LINE_INFO());
      Assert::AreEqual(NegativeInfinity<Float_32>(), w(actual), L"-inf not preserved after replace", LINE_INFO());
    }

    // -- Arithmetic edge cases with denormals and extremes --

    TEST_METHOD(additionWithDenormals) {
      const auto denorm = v_f32_4(std::numeric_limits<Float_32>::denorm_min());
      const auto actual = denorm + denorm;
      const auto expected = Float_32{2.0f} * std::numeric_limits<Float_32>::denorm_min();
      Assert::AreEqual(expected, x(actual), L"denorm addition mismatch", LINE_INFO());
    }

    TEST_METHOD(multiplicationOverflow) {
      const auto maxVal = v_f32_4(std::numeric_limits<Float_32>::max());
      const auto two = v_f32_4(2.0f);
      const auto actual = maxVal * two;
      Assert::AreEqual(Infinity<Float_32>(), x(actual), L"max*2 should overflow to inf", LINE_INFO());
    }

    TEST_METHOD(divisionByZeroProducesInfinity) {
      const auto one = One<v_f32_4>();
      const auto zero = Zero<v_f32_4>();
      const auto actual = one / zero;
      Assert::AreEqual(Infinity<Float_32>(), x(actual), L"1/0 should be inf", LINE_INFO());
    }

    TEST_METHOD(negativeZeroArithmetic) {
      const auto negZero = NegativeZero<v_f32_4>();
      const auto posZero = Zero<v_f32_4>();

      // -0.0 + 0.0 = 0.0 (positive zero)
      const auto sum = negZero + posZero;
      Assert::AreEqual(0.0f, x(sum), L"sum value mismatch", LINE_INFO());
      Assert::IsFalse(std::signbit(x(sum)), L"-0 + 0 should be positive zero", LINE_INFO());

      // -0.0 * -0.0 = +0.0
      const auto prod = negZero * negZero;
      Assert::AreEqual(0.0f, x(prod), L"product value mismatch", LINE_INFO());
      Assert::IsFalse(std::signbit(x(prod)), L"-0 * -0 should be positive zero", LINE_INFO());

      // 1.0 * -0.0 = -0.0
      const auto scaled = One<v_f32_4>() * negZero;
      Assert::AreEqual(0.0f, x(scaled), L"scaled value mismatch", LINE_INFO());
      Assert::IsTrue(std::signbit(x(scaled)), L"1 * -0 should be negative zero", LINE_INFO());
    }

    TEST_METHOD(infinityArithmetic) {
      const auto inf = v_f32_4(Infinity<Float_32>());
      const auto negInf = v_f32_4(NegativeInfinity<Float_32>());
      const auto one = One<v_f32_4>();

      // inf + 1 = inf
      Assert::AreEqual(Infinity<Float_32>(), x(inf + one), L"inf+1 should be inf", LINE_INFO());
      // inf - inf = NaN
      Assert::IsTrue(std::isnan(x(inf - inf)), L"inf-inf should be NaN", LINE_INFO());
      // inf * 0 = NaN
      Assert::IsTrue(std::isnan(x(inf * Zero<v_f32_4>())), L"inf*0 should be NaN", LINE_INFO());
      // -inf + -inf = -inf
      Assert::AreEqual(NegativeInfinity<Float_32>(), x(negInf + negInf), L"-inf+-inf should be -inf", LINE_INFO());
    }

    TEST_METHOD(nanPropagation) {
      const auto nan = v_f32_4(NotANumber<Float_32>());
      const auto one = One<v_f32_4>();

      Assert::IsTrue(std::isnan(x(nan + one)), L"NaN+1 should be NaN", LINE_INFO());
      Assert::IsTrue(std::isnan(x(nan - one)), L"NaN-1 should be NaN", LINE_INFO());
      Assert::IsTrue(std::isnan(x(nan * one)), L"NaN*1 should be NaN", LINE_INFO());
      Assert::IsTrue(std::isnan(x(nan / one)), L"NaN/1 should be NaN", LINE_INFO());
    }

    // -- Comparison edge cases --

    TEST_METHOD(negativeZeroEqualsPositiveZero) {
      const auto negZero = NegativeZero<v_f32_4>();
      const auto posZero = Zero<v_f32_4>();
      // IEEE 754: -0.0 == 0.0
      Assert::IsTrue(allTrue(negZero == posZero), L"-0 should equal +0", LINE_INFO());
    }
  };
}
