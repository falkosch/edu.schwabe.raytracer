#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f32_8_FunctionsTrigonometryTest) {
  public:
    TEST_METHOD(sinOfZeroIsZero) {
      const auto r = sin(Zero<v_f32_8>());
      Assert::AreEqual(0.0f, x1(r), 1e-6f, L"sin(0) x1", LINE_INFO());
      Assert::AreEqual(0.0f, x8(r), 1e-6f, L"sin(0) x8", LINE_INFO());
    }

    TEST_METHOD(cosOfZeroIsOne) {
      const auto r = cos(Zero<v_f32_8>());
      Assert::AreEqual(1.0f, x1(r), 1e-6f, L"cos(0) x1", LINE_INFO());
      Assert::AreEqual(1.0f, x8(r), 1e-6f, L"cos(0) x8", LINE_INFO());
    }

    TEST_METHOD(sinOfPiHalfIsOne) {
      const auto piHalf = Pi<v_f32_8>() * v_f32_8(0.5f);
      const auto r = sin(piHalf);
      Assert::AreEqual(1.0f, x1(r), 1e-5f, L"sin(pi/2)", LINE_INFO());
    }

    TEST_METHOD(expOfZeroIsOne) {
      const auto r = exp(Zero<v_f32_8>());
      Assert::AreEqual(1.0f, x1(r), 1e-6f, L"exp(0) x1", LINE_INFO());
      Assert::AreEqual(1.0f, x8(r), 1e-6f, L"exp(0) x8", LINE_INFO());
    }

    TEST_METHOD(logOfOneIsZero) {
      const auto r = log(One<v_f32_8>());
      Assert::AreEqual(0.0f, x1(r), 1e-6f, L"log(1) x1", LINE_INFO());
      Assert::AreEqual(0.0f, x8(r), 1e-6f, L"log(1) x8", LINE_INFO());
    }

    TEST_METHOD(expLogRoundTrip) {
      const v_f32_8 v(0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f, 4.0f);
      const auto r = log(exp(v));
      Assert::AreEqual(0.5f, x1(r), 1e-5f, L"x1", LINE_INFO());
      Assert::AreEqual(4.0f, x8(r), 1e-5f, L"x8", LINE_INFO());
    }

    TEST_METHOD(powSquares) {
      const v_f32_8 v(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = pow(v, 2.0f);
      Assert::AreEqual(1.0f, x1(r), 1e-4f, L"1^2", LINE_INFO());
      Assert::AreEqual(4.0f, x2(r), 1e-4f, L"2^2", LINE_INFO());
      Assert::AreEqual(64.0f, x8(r), 1e-3f, L"8^2", LINE_INFO());
    }

    TEST_METHOD(sincosMatchesSeparate) {
      const v_f32_8 v(0.0f, 0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f);
      v_f32_8 s, c;
      sincos(v, s, c);
      const auto sRef = sin(v);
      const auto cRef = cos(v);
      Assert::AreEqual(x1(sRef), x1(s), 1e-6f, L"sin x1", LINE_INFO());
      Assert::AreEqual(x8(sRef), x8(s), 1e-6f, L"sin x8", LINE_INFO());
      Assert::AreEqual(x1(cRef), x1(c), 1e-6f, L"cos x1", LINE_INFO());
      Assert::AreEqual(x8(cRef), x8(c), 1e-6f, L"cos x8", LINE_INFO());
    }

    TEST_METHOD(powWithVectorExponent) {
      const v_f32_8 base(2.0f);
      const v_f32_8 exp(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = pow(base, exp);
      Assert::AreEqual(2.0f, x1(r), 1e-3f, L"2^1", LINE_INFO());
      Assert::AreEqual(4.0f, x2(r), 1e-3f, L"2^2", LINE_INFO());
      Assert::AreEqual(256.0f, x8(r), 1e-1f, L"2^8", LINE_INFO());
    }

    TEST_METHOD(sincosOutputCorrectness) {
      const v_f32_8 v(0.0f, 0.5f, 1.0f, 1.5f, 2.0f, 2.5f, 3.0f, 3.5f);
      v_f32_8 s, c;
      sincos(v, s, c);
      Assert::AreEqual(0.0f, x1(s), 1e-6f, L"sin(0)=0", LINE_INFO());
      Assert::AreEqual(1.0f, x1(c), 1e-6f, L"cos(0)=1", LINE_INFO());
      Assert::AreEqual(std::sin(1.0f), x3(s), 1e-5f, L"sin(1)", LINE_INFO());
      Assert::AreEqual(std::cos(1.0f), x3(c), 1e-5f, L"cos(1)", LINE_INFO());
      Assert::AreEqual(std::sin(3.5f), x8(s), 1e-5f, L"sin(3.5)", LINE_INFO());
      Assert::AreEqual(std::cos(3.5f), x8(c), 1e-5f, L"cos(3.5)", LINE_INFO());
    }
  };
}
