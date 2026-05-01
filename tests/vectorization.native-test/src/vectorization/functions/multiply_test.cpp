#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(MultiplyTest) {
  public:
    TEST_METHOD(multipliesPackedFloat4) {
      const auto a = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
      const auto b = _mm_set_ps(5.0f, 4.0f, 3.0f, 2.0f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), multiply(a, b));
      Assert::AreEqual(2.0f, actual[0], L"x lane", LINE_INFO());
      Assert::AreEqual(6.0f, actual[1], L"y lane", LINE_INFO());
      Assert::AreEqual(12.0f, actual[2], L"z lane", LINE_INFO());
      Assert::AreEqual(20.0f, actual[3], L"w lane", LINE_INFO());
    }

    TEST_METHOD(multipliesPackedFloat2) {
      const auto a = _mm_set_pd(3.0, 2.0);
      const auto b = _mm_set_pd(4.0, 5.0);
      std::array<Float_64, VectorSizes::X2> actual{};
      _mm_store_pd(actual.data(), multiply(a, b));
      Assert::AreEqual(10.0, actual[0], L"x lane", LINE_INFO());
      Assert::AreEqual(12.0, actual[1], L"y lane", LINE_INFO());
    }

    TEST_METHOD(multipliesPackedFloat8) {
      const auto a = _mm256_set_ps(8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
      const auto b = _mm256_set_ps(2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f);
      alignas(YMM_ALIGNMENT) std::array<Float_32, VectorSizes::X8> actual{};
      _mm256_store_ps(actual.data(), multiply(a, b));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(static_cast<Float_32>((i + 1) * 2), actual[i], L"lane mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(multipliesPackedFloat4_256) {
      const auto a = _mm256_set_pd(4.0, 3.0, 2.0, 1.0);
      const auto b = _mm256_set_pd(5.0, 4.0, 3.0, 2.0);
      alignas(YMM_ALIGNMENT) std::array<Float_64, VectorSizes::W> actual{};
      _mm256_store_pd(actual.data(), multiply(a, b));
      Assert::AreEqual(2.0, actual[0], L"x lane", LINE_INFO());
      Assert::AreEqual(6.0, actual[1], L"y lane", LINE_INFO());
      Assert::AreEqual(12.0, actual[2], L"z lane", LINE_INFO());
      Assert::AreEqual(20.0, actual[3], L"w lane", LINE_INFO());
    }

    TEST_METHOD(multiplyXOperatesOnXLaneOnly) {
      const auto a = _mm_set_ps(40.0f, 30.0f, 20.0f, 10.0f);
      const auto b = _mm_set_ps(4.0f, 3.0f, 2.0f, 3.0f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), multiplyX(a, b));
      Assert::AreEqual(30.0f, actual[0], L"x lane should be a.x * b.x", LINE_INFO());
      Assert::AreEqual(20.0f, actual[1], L"y lane should pass through from a", LINE_INFO());
      Assert::AreEqual(30.0f, actual[2], L"z lane should pass through from a", LINE_INFO());
      Assert::AreEqual(40.0f, actual[3], L"w lane should pass through from a", LINE_INFO());
    }

    TEST_METHOD(multiplyXFloat64OperatesOnXLaneOnly) {
      const auto a = _mm_set_pd(20.0, 10.0);
      const auto b = _mm_set_pd(2.0, 3.0);
      std::array<Float_64, VectorSizes::X2> actual{};
      _mm_store_pd(actual.data(), multiplyX(a, b));
      Assert::AreEqual(30.0, actual[0], L"x lane should be a.x * b.x", LINE_INFO());
      Assert::AreEqual(20.0, actual[1], L"y lane should pass through from a", LINE_INFO());
    }

    TEST_METHOD(multipliesWithZero) {
      const auto a = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
      const auto b = _mm_set_ps1(Zero<Float_32>());
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), multiply(a, b));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(0.0f, actual[i], L"a * 0 should be 0", LINE_INFO());
      }
    }

    TEST_METHOD(multipliesWithInfinity) {
      const auto a = _mm_set_ps(1.0f, -1.0f, 0.0f, Infinity<Float_32>());
      const auto b = _mm_set_ps(Infinity<Float_32>(), Infinity<Float_32>(), Infinity<Float_32>(), 1.0f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), multiply(a, b));
      Assert::AreEqual(Infinity<Float_32>(), actual[0], L"inf * 1 should be inf", LINE_INFO());
      Assert::IsTrue(std::isnan(actual[1]), L"0 * inf should be NaN", LINE_INFO());
      Assert::AreEqual(NegativeInfinity<Float_32>(), actual[2], L"-1 * inf should be -inf", LINE_INFO());
      Assert::AreEqual(Infinity<Float_32>(), actual[3], L"1 * inf should be inf", LINE_INFO());
    }
  };
}
