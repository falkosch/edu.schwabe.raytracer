#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f64_4_FunctionsTrigonometryTest) {
  public:
    TEST_METHOD(computesSinAtZero) {
      const v_f64_4 zero{0.0, 0.0, 0.0, 0.0};
      const auto result = vectorization::sin(zero);
      Assert::AreEqual(0.0, x(result), 1e-10, L"sin(0) X mismatch", LINE_INFO());
      Assert::AreEqual(0.0, y(result), 1e-10, L"sin(0) Y mismatch", LINE_INFO());
      Assert::AreEqual(0.0, z(result), 1e-10, L"sin(0) Z mismatch", LINE_INFO());
      Assert::AreEqual(0.0, w(result), 1e-10, L"sin(0) W mismatch", LINE_INFO());
    }

    TEST_METHOD(computesCosAtZero) {
      const v_f64_4 zero{0.0, 0.0, 0.0, 0.0};
      const auto result = vectorization::cos(zero);
      Assert::AreEqual(1.0, x(result), 1e-10, L"cos(0) X mismatch", LINE_INFO());
      Assert::AreEqual(1.0, y(result), 1e-10, L"cos(0) Y mismatch", LINE_INFO());
      Assert::AreEqual(1.0, z(result), 1e-10, L"cos(0) Z mismatch", LINE_INFO());
      Assert::AreEqual(1.0, w(result), 1e-10, L"cos(0) W mismatch", LINE_INFO());
    }

    TEST_METHOD(computesExpAtZero) {
      const v_f64_4 zero{0.0, 0.0, 0.0, 0.0};
      const auto result = vectorization::exp(zero);
      Assert::AreEqual(1.0, x(result), 1e-10, L"exp(0) X mismatch", LINE_INFO());
      Assert::AreEqual(1.0, y(result), 1e-10, L"exp(0) Y mismatch", LINE_INFO());
      Assert::AreEqual(1.0, z(result), 1e-10, L"exp(0) Z mismatch", LINE_INFO());
      Assert::AreEqual(1.0, w(result), 1e-10, L"exp(0) W mismatch", LINE_INFO());
    }

    TEST_METHOD(computesLogAtOne) {
      const v_f64_4 one{1.0, 1.0, 1.0, 1.0};
      const auto result = vectorization::log(one);
      Assert::AreEqual(0.0, x(result), 1e-10, L"log(1) X mismatch", LINE_INFO());
      Assert::AreEqual(0.0, y(result), 1e-10, L"log(1) Y mismatch", LINE_INFO());
      Assert::AreEqual(0.0, z(result), 1e-10, L"log(1) Z mismatch", LINE_INFO());
      Assert::AreEqual(0.0, w(result), 1e-10, L"log(1) W mismatch", LINE_INFO());
    }

    TEST_METHOD(computesPow) {
      const v_f64_4 base{2.0, 3.0, 4.0, 10.0};
      const v_f64_4 exponent{3.0, 2.0, 0.5, 1.0};
      const auto result = pow(base, exponent);
      // 2^3=8, 3^2=9, 4^0.5=2, 10^1=10
      Assert::AreEqual(8.0, x(result), 1e-10, L"pow X mismatch", LINE_INFO());
      Assert::AreEqual(9.0, y(result), 1e-10, L"pow Y mismatch", LINE_INFO());
      Assert::AreEqual(2.0, z(result), 1e-10, L"pow Z mismatch", LINE_INFO());
      Assert::AreEqual(10.0, w(result), 1e-10, L"pow W mismatch", LINE_INFO());

      // scalar exponent
      const auto result2 = pow(v_f64_4{1.0, 2.0, 3.0, 4.0}, 2.0);
      Assert::AreEqual(1.0, x(result2), 1e-10, L"pow(scalar) X mismatch", LINE_INFO());
      Assert::AreEqual(4.0, y(result2), 1e-10, L"pow(scalar) Y mismatch", LINE_INFO());
      Assert::AreEqual(9.0, z(result2), 1e-10, L"pow(scalar) Z mismatch", LINE_INFO());
      Assert::AreEqual(16.0, w(result2), 1e-10, L"pow(scalar) W mismatch", LINE_INFO());
    }

    TEST_METHOD(computesSincos) {
      const v_f64_4 angles{0.0, Pi<Float_64>() * 0.5, Pi<Float_64>(), Pi<Float_64>() * 2.0};
      v_f64_4 outSin;
      v_f64_4 outCos;
      vectorization::sincos(angles, outSin, outCos);

      // sin: 0, 1, 0, 0
      Assert::AreEqual(0.0, x(outSin), 1e-10, L"sincos sin(0) mismatch", LINE_INFO());
      Assert::AreEqual(1.0, y(outSin), 1e-10, L"sincos sin(pi/2) mismatch", LINE_INFO());
      Assert::AreEqual(0.0, z(outSin), 1e-10, L"sincos sin(pi) mismatch", LINE_INFO());
      Assert::AreEqual(0.0, w(outSin), 1e-10, L"sincos sin(2pi) mismatch", LINE_INFO());

      // cos: 1, 0, -1, 1
      Assert::AreEqual(1.0, x(outCos), 1e-10, L"sincos cos(0) mismatch", LINE_INFO());
      Assert::AreEqual(0.0, y(outCos), 1e-10, L"sincos cos(pi/2) mismatch", LINE_INFO());
      Assert::AreEqual(-1.0, z(outCos), 1e-10, L"sincos cos(pi) mismatch", LINE_INFO());
      Assert::AreEqual(1.0, w(outCos), 1e-10, L"sincos cos(2pi) mismatch", LINE_INFO());
    }
  };
}
