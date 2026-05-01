#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(AddTest) {
  public:
    TEST_METHOD(addsPackedFloat4) {
      const auto a = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
      const auto b = _mm_set_ps(8.0f, 6.0f, 4.0f, 2.0f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), add(a, b));
      Assert::AreEqual(3.0f, actual[0], L"x lane", LINE_INFO());
      Assert::AreEqual(6.0f, actual[1], L"y lane", LINE_INFO());
      Assert::AreEqual(9.0f, actual[2], L"z lane", LINE_INFO());
      Assert::AreEqual(12.0f, actual[3], L"w lane", LINE_INFO());
    }

    TEST_METHOD(addsPackedFloat2) {
      const auto a = _mm_set_pd(3.0, 1.0);
      const auto b = _mm_set_pd(6.0, 2.0);
      std::array<Float_64, VectorSizes::X2> actual{};
      _mm_store_pd(actual.data(), add(a, b));
      Assert::AreEqual(3.0, actual[0], L"x lane", LINE_INFO());
      Assert::AreEqual(9.0, actual[1], L"y lane", LINE_INFO());
    }

    TEST_METHOD(addsPackedFloat8) {
      const auto a = _mm256_set_ps(8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
      const auto b = _mm256_set_ps(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
      alignas(YMM_ALIGNMENT) std::array<Float_32, VectorSizes::X8> actual{};
      _mm256_store_ps(actual.data(), add(a, b));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(static_cast<Float_32>(i + 2), actual[i], L"lane mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(addsPackedFloat4_256) {
      const auto a = _mm256_set_pd(4.0, 3.0, 2.0, 1.0);
      const auto b = _mm256_set_pd(1.0, 1.0, 1.0, 1.0);
      alignas(YMM_ALIGNMENT) std::array<Float_64, VectorSizes::W> actual{};
      _mm256_store_pd(actual.data(), add(a, b));
      Assert::AreEqual(2.0, actual[0], L"x lane", LINE_INFO());
      Assert::AreEqual(3.0, actual[1], L"y lane", LINE_INFO());
      Assert::AreEqual(4.0, actual[2], L"z lane", LINE_INFO());
      Assert::AreEqual(5.0, actual[3], L"w lane", LINE_INFO());
    }

    TEST_METHOD(addXOperatesOnXLaneOnly) {
      const auto a = _mm_set_ps(40.0f, 30.0f, 20.0f, 10.0f);
      const auto b = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), addX(a, b));
      Assert::AreEqual(11.0f, actual[0], L"x lane should be a.x + b.x", LINE_INFO());
      Assert::AreEqual(20.0f, actual[1], L"y lane should pass through from a", LINE_INFO());
      Assert::AreEqual(30.0f, actual[2], L"z lane should pass through from a", LINE_INFO());
      Assert::AreEqual(40.0f, actual[3], L"w lane should pass through from a", LINE_INFO());
    }

    TEST_METHOD(addXFloat64OperatesOnXLaneOnly) {
      const auto a = _mm_set_pd(20.0, 10.0);
      const auto b = _mm_set_pd(2.0, 1.0);
      std::array<Float_64, VectorSizes::X2> actual{};
      _mm_store_pd(actual.data(), addX(a, b));
      Assert::AreEqual(11.0, actual[0], L"x lane should be a.x + b.x", LINE_INFO());
      Assert::AreEqual(20.0, actual[1], L"y lane should pass through from a", LINE_INFO());
    }

    TEST_METHOD(addsWithInfinity) {
      const auto a = _mm_set_ps(1.0f, Infinity<Float_32>(), NegativeInfinity<Float_32>(), 0.0f);
      const auto b = _mm_set_ps(Infinity<Float_32>(), 1.0f, 1.0f, 0.0f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), add(a, b));
      Assert::AreEqual(0.0f, actual[0], L"0+0", LINE_INFO());
      Assert::AreEqual(NegativeInfinity<Float_32>(), actual[1], L"-inf+1", LINE_INFO());
      Assert::AreEqual(Infinity<Float_32>(), actual[2], L"inf+1", LINE_INFO());
      Assert::AreEqual(Infinity<Float_32>(), actual[3], L"1+inf", LINE_INFO());
    }

    TEST_METHOD(addsInfinityAndNegativeInfinityProducesNaN) {
      const auto a = _mm_set_ps1(Infinity<Float_32>());
      const auto b = _mm_set_ps1(NegativeInfinity<Float_32>());
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), add(a, b));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::IsTrue(std::isnan(actual[i]), L"inf + (-inf) should be NaN", LINE_INFO());
      }
    }

    TEST_METHOD(addsWithSignedZero) {
      const auto pos = _mm_set_ps1(Zero<Float_32>());
      const auto neg = _mm_set_ps1(NegativeZero<Float_32>());
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), add(pos, neg));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(0.0f, actual[i], L"+0 + (-0) should be +0", LINE_INFO());
      }
    }
  };
}
