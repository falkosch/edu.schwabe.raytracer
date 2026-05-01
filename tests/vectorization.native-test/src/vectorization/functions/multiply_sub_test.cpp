#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(MultiplySubTest) {
  public:
    TEST_METHOD(multiplySubsPackedFloat4) {
      const auto a = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
      const auto b = _mm_set_ps(1.0f, 2.0f, 3.0f, 4.0f);
      const auto c = _mm_set_ps(10.0f, 10.0f, 10.0f, 10.0f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), multiplySub(a, b, c));
      // a*b-c: {1*4-10, 2*3-10, 3*2-10, 4*1-10} = {-6, -4, -4, -6}
      Assert::AreEqual(-6.0f, actual[0], L"x lane", LINE_INFO());
      Assert::AreEqual(-4.0f, actual[1], L"y lane", LINE_INFO());
      Assert::AreEqual(-4.0f, actual[2], L"z lane", LINE_INFO());
      Assert::AreEqual(-6.0f, actual[3], L"w lane", LINE_INFO());
    }

    TEST_METHOD(multiplySubsPackedFloat2) {
      const auto a = _mm_set_pd(3.0, 2.0);
      const auto b = _mm_set_pd(4.0, 5.0);
      const auto c = _mm_set_pd(1.0, 1.0);
      std::array<Float_64, VectorSizes::X2> actual{};
      _mm_store_pd(actual.data(), multiplySub(a, b, c));
      // a*b-c: {2*5-1, 3*4-1} = {9, 11}
      Assert::AreEqual(9.0, actual[0], L"x lane", LINE_INFO());
      Assert::AreEqual(11.0, actual[1], L"y lane", LINE_INFO());
    }

    TEST_METHOD(multiplySubsPackedFloat8) {
      const auto a = _mm256_set_ps(8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
      const auto b = _mm256_set1_ps(2.0f);
      const auto c = _mm256_set1_ps(0.5f);
      alignas(YMM_ALIGNMENT) std::array<Float_32, VectorSizes::X8> actual{};
      _mm256_store_ps(actual.data(), multiplySub(a, b, c));
      // a*b-c: (i+1)*2 - 0.5
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(static_cast<Float_32>((i + 1) * 2) - 0.5f, actual[i], L"lane mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(multiplySubsPackedFloat4_256) {
      const auto a = _mm256_set_pd(4.0, 3.0, 2.0, 1.0);
      const auto b = _mm256_set1_pd(3.0);
      const auto c = _mm256_set1_pd(0.25);
      alignas(YMM_ALIGNMENT) std::array<Float_64, VectorSizes::W> actual{};
      _mm256_store_pd(actual.data(), multiplySub(a, b, c));
      // a*b-c: {1*3-0.25, 2*3-0.25, 3*3-0.25, 4*3-0.25} = {2.75, 5.75, 8.75, 11.75}
      Assert::AreEqual(2.75, actual[0], L"x lane", LINE_INFO());
      Assert::AreEqual(5.75, actual[1], L"y lane", LINE_INFO());
      Assert::AreEqual(8.75, actual[2], L"z lane", LINE_INFO());
      Assert::AreEqual(11.75, actual[3], L"w lane", LINE_INFO());
    }

    TEST_METHOD(multiplySubXOperatesOnXLaneOnly) {
      const auto a = _mm_set_ps(40.0f, 30.0f, 20.0f, 2.0f);
      const auto b = _mm_set_ps(4.0f, 3.0f, 2.0f, 3.0f);
      const auto c = _mm_set_ps(1.0f, 1.0f, 1.0f, 0.5f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), multiplySubX(a, b, c));
      // x = a.x*b.x-c.x = 2*3-0.5 = 5.5
      Assert::AreEqual(5.5f, actual[0], L"x = a.x*b.x-c.x = 2*3-0.5", LINE_INFO());
      Assert::AreEqual(20.0f, actual[1], L"y pass through from a", LINE_INFO());
      Assert::AreEqual(30.0f, actual[2], L"z pass through from a", LINE_INFO());
      Assert::AreEqual(40.0f, actual[3], L"w pass through from a", LINE_INFO());
    }

    TEST_METHOD(multiplySubXFloat64OperatesOnXLaneOnly) {
      const auto a = _mm_set_pd(20.0, 3.0);
      const auto b = _mm_set_pd(2.0, 4.0);
      const auto c = _mm_set_pd(1.0, 0.5);
      std::array<Float_64, VectorSizes::X2> actual{};
      _mm_store_pd(actual.data(), multiplySubX(a, b, c));
      // x = 3*4-0.5 = 11.5
      Assert::AreEqual(11.5, actual[0], L"x = 3*4-0.5", LINE_INFO());
      Assert::AreEqual(20.0, actual[1], L"y pass through from a", LINE_INFO());
    }

    TEST_METHOD(multiplySubWithZeroC) {
      const auto a = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
      const auto b = _mm_set_ps(1.0f, 2.0f, 3.0f, 4.0f);
      const auto c = _mm_setzero_ps();
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), multiplySub(a, b, c));
      // a*b-0 reduces to a*b
      Assert::AreEqual(4.0f, actual[0], L"1*4-0", LINE_INFO());
      Assert::AreEqual(6.0f, actual[1], L"2*3-0", LINE_INFO());
      Assert::AreEqual(6.0f, actual[2], L"3*2-0", LINE_INFO());
      Assert::AreEqual(4.0f, actual[3], L"4*1-0", LINE_INFO());
    }

    TEST_METHOD(multiplySubWithInfinity) {
      const auto a = _mm_set_ps1(Infinity<Float_32>());
      const auto b = _mm_set_ps1(Zero<Float_32>());
      const auto c = _mm_set_ps1(1.0f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), multiplySub(a, b, c));
      // inf*0-1 = NaN-1 = NaN
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::IsTrue(std::isnan(actual[i]), L"inf*0-1 should be NaN", LINE_INFO());
      }
    }
  };
}
