#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f64_2_FunctionsTrigonometryTest) {
  public:
    TEST_METHOD(computesSinAtZero) {
      const v_f64_2 zero{0.0, 0.0};
      const auto result = vectorization::sin(zero);
      Assert::AreEqual(0.0, x(result), 1e-10, L"sin(0) X mismatch", LINE_INFO());
      Assert::AreEqual(0.0, y(result), 1e-10, L"sin(0) Y mismatch", LINE_INFO());
    }

    TEST_METHOD(computesCosAtZero) {
      const v_f64_2 zero{0.0, 0.0};
      const auto result = vectorization::cos(zero);
      Assert::AreEqual(1.0, x(result), 1e-10, L"cos(0) X mismatch", LINE_INFO());
      Assert::AreEqual(1.0, y(result), 1e-10, L"cos(0) Y mismatch", LINE_INFO());
    }

    TEST_METHOD(computesExpAtZero) {
      const v_f64_2 zero{0.0, 0.0};
      const auto result = vectorization::exp(zero);
      Assert::AreEqual(1.0, x(result), 1e-10, L"exp(0) X mismatch", LINE_INFO());
      Assert::AreEqual(1.0, y(result), 1e-10, L"exp(0) Y mismatch", LINE_INFO());
    }

    TEST_METHOD(computesLogAtOne) {
      const v_f64_2 one{1.0, 1.0};
      const auto result = vectorization::log(one);
      Assert::AreEqual(0.0, x(result), 1e-10, L"log(1) X mismatch", LINE_INFO());
      Assert::AreEqual(0.0, y(result), 1e-10, L"log(1) Y mismatch", LINE_INFO());
    }

    TEST_METHOD(computesPow) {
      const v_f64_2 base{2.0, 3.0};
      const v_f64_2 exponent{3.0, 2.0};
      const auto result = pow(base, exponent);
      // 2^3 = 8, 3^2 = 9
      Assert::AreEqual(8.0, x(result), 1e-10, L"pow X mismatch", LINE_INFO());
      Assert::AreEqual(9.0, y(result), 1e-10, L"pow Y mismatch", LINE_INFO());

      // scalar exponent
      const auto result2 = pow(base, 2.0);
      // 2^2 = 4, 3^2 = 9
      Assert::AreEqual(4.0, x(result2), 1e-10, L"pow(scalar) X mismatch", LINE_INFO());
      Assert::AreEqual(9.0, y(result2), 1e-10, L"pow(scalar) Y mismatch", LINE_INFO());
    }

    TEST_METHOD(computesSincos) {
      const v_f64_2 angles{0.0, Pi<Float_64>() * 0.5};
      v_f64_2 outSin;
      v_f64_2 outCos;
      vectorization::sincos(angles, outSin, outCos);

      // sin(0)=0, sin(pi/2)=1
      Assert::AreEqual(0.0, x(outSin), 1e-10, L"sincos sin(0) mismatch", LINE_INFO());
      Assert::AreEqual(1.0, y(outSin), 1e-10, L"sincos sin(pi/2) mismatch", LINE_INFO());

      // cos(0)=1, cos(pi/2)=0
      Assert::AreEqual(1.0, x(outCos), 1e-10, L"sincos cos(0) mismatch", LINE_INFO());
      Assert::AreEqual(0.0, y(outCos), 1e-10, L"sincos cos(pi/2) mismatch", LINE_INFO());
    }
  };
}
