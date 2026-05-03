#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f64_4_ConstantsTest) {
  public:
    TEST_METHOD(hasZeroAndOne) {
      const auto zero = Zero<v_f64_4>();
      Assert::AreEqual(0.0, x(zero), L"Zero X mismatch", LINE_INFO());
      Assert::AreEqual(0.0, y(zero), L"Zero Y mismatch", LINE_INFO());
      Assert::AreEqual(0.0, z(zero), L"Zero Z mismatch", LINE_INFO());
      Assert::AreEqual(0.0, w(zero), L"Zero W mismatch", LINE_INFO());

      const auto one = One<v_f64_4>();
      Assert::AreEqual(1.0, x(one), L"One X mismatch", LINE_INFO());
      Assert::AreEqual(1.0, y(one), L"One Y mismatch", LINE_INFO());
      Assert::AreEqual(1.0, z(one), L"One Z mismatch", LINE_INFO());
      Assert::AreEqual(1.0, w(one), L"One W mismatch", LINE_INFO());
    }

    TEST_METHOD(hasTwoHalfAndNegativeOne) {
      const auto two = Two<v_f64_4>();
      Assert::AreEqual(2.0, x(two), L"Two X mismatch", LINE_INFO());
      Assert::AreEqual(2.0, w(two), L"Two W mismatch", LINE_INFO());

      const auto half = Half<v_f64_4>();
      Assert::AreEqual(0.5, x(half), L"Half X mismatch", LINE_INFO());
      Assert::AreEqual(0.5, w(half), L"Half W mismatch", LINE_INFO());

      const auto negOne = NegativeOne<v_f64_4>();
      Assert::AreEqual(-1.0, x(negOne), L"NegativeOne X mismatch", LINE_INFO());
      Assert::AreEqual(-1.0, w(negOne), L"NegativeOne W mismatch", LINE_INFO());
    }

    TEST_METHOD(hasPiAndEpsilon) {
      const auto pi = Pi<v_f64_4>();
      Assert::AreEqual(Pi<Float_64>(), x(pi), L"Pi X mismatch", LINE_INFO());
      Assert::AreEqual(Pi<Float_64>(), w(pi), L"Pi W mismatch", LINE_INFO());

      const auto eps = Epsilon<v_f64_4>();
      Assert::AreEqual(Epsilon<Float_64>(), x(eps), L"Epsilon X mismatch", LINE_INFO());
      Assert::AreEqual(Epsilon<Float_64>(), w(eps), L"Epsilon W mismatch", LINE_INFO());
    }

    TEST_METHOD(hasInfinityAndNaN) {
      const auto inf = Infinity<v_f64_4>();
      Assert::AreEqual(Infinity<Float_64>(), x(inf), L"Infinity X mismatch", LINE_INFO());
      Assert::AreEqual(Infinity<Float_64>(), w(inf), L"Infinity W mismatch", LINE_INFO());

      const auto negInf = NegativeInfinity<v_f64_4>();
      Assert::AreEqual(NegativeInfinity<Float_64>(), x(negInf), L"NegativeInfinity X mismatch", LINE_INFO());

      const auto nan = NotANumber<v_f64_4>();
      Assert::IsTrue(std::isnan(x(nan)), L"NotANumber X should be NaN", LINE_INFO());
      Assert::IsTrue(std::isnan(w(nan)), L"NotANumber W should be NaN", LINE_INFO());
    }

    TEST_METHOD(hasMaskConstants) {
      const auto maskAll = MaskAll<v_f64_4>();
      const auto maskNone = MaskNone<v_f64_4>();

      // MaskAll & ~MaskAll = zero
      const auto result = maskAll & (~maskAll);
      Assert::AreEqual(0.0, x(result), L"MaskAll & ~MaskAll X mismatch", LINE_INFO());
      Assert::AreEqual(0.0, w(result), L"MaskAll & ~MaskAll W mismatch", LINE_INFO());

      // MaskNone should be all zeros
      Assert::AreEqual(0.0, x(maskNone), L"MaskNone X mismatch", LINE_INFO());
      Assert::AreEqual(0.0, w(maskNone), L"MaskNone W mismatch", LINE_INFO());
    }

    TEST_METHOD(hasNegativeZero) {
      const auto negZero = NegativeZero<v_f64_4>();
      Assert::IsTrue(allTrue(Zero<v_f64_4>() == negZero), L"NegativeZero should equal zero", LINE_INFO());
      Assert::IsTrue(std::signbit(x(negZero)), L"NegativeZero X should have sign bit", LINE_INFO());
      Assert::IsTrue(std::signbit(w(negZero)), L"NegativeZero W should have sign bit", LINE_INFO());
    }
  };
}
