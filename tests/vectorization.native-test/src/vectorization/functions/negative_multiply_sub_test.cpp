#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(NegativeMultiplySubTest) {
  public:
    TEST_METHOD(negativeMultiplySubsPackedFloat4) {
      // negativeMultiplySub: -(a*b)-c
      // a*b = {1*4, 2*3, 3*2, 4*1} = {4, 6, 6, 4}
      // -(a*b)-c = {-4-10, -6-10, -6-10, -4-10} = {-14, -16, -16, -14}
      const auto a = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
      const auto b = _mm_set_ps(1.0f, 2.0f, 3.0f, 4.0f);
      const auto c = _mm_set_ps(10.0f, 10.0f, 10.0f, 10.0f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), negativeMultiplySub(a, b, c));
      Assert::AreEqual(-14.0f, actual[0], L"x lane", LINE_INFO());
      Assert::AreEqual(-16.0f, actual[1], L"y lane", LINE_INFO());
      Assert::AreEqual(-16.0f, actual[2], L"z lane", LINE_INFO());
      Assert::AreEqual(-14.0f, actual[3], L"w lane", LINE_INFO());
    }

    TEST_METHOD(negativeMultiplySubsPackedFloat2) {
      // a*b = {2*5, 3*4} = {10, 12}
      // -(a*b)-c = {-10-1, -12-1} = {-11, -13}
      const auto a = _mm_set_pd(3.0, 2.0);
      const auto b = _mm_set_pd(4.0, 5.0);
      const auto c = _mm_set_pd(1.0, 1.0);
      std::array<Float_64, VectorSizes::X2> actual{};
      _mm_store_pd(actual.data(), negativeMultiplySub(a, b, c));
      Assert::AreEqual(-11.0, actual[0], L"x lane", LINE_INFO());
      Assert::AreEqual(-13.0, actual[1], L"y lane", LINE_INFO());
    }

    TEST_METHOD(negativeMultiplySubsPackedFloat8) {
      // a*b = {2, 4, 6, 8, 10, 12, 14, 16}
      // -(a*b)-c = {-2-0.5, -4-0.5, ...} = {-2.5, -4.5, -6.5, -8.5, -10.5, -12.5, -14.5, -16.5}
      const auto a = _mm256_set_ps(8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
      const auto b = _mm256_set1_ps(2.0f);
      const auto c = _mm256_set1_ps(0.5f);
      alignas(YMM_ALIGNMENT) std::array<Float_32, VectorSizes::X8> actual{};
      _mm256_store_ps(actual.data(), negativeMultiplySub(a, b, c));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(-static_cast<Float_32>((i + 1) * 2) - 0.5f, actual[i], L"lane mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(negativeMultiplySubsPackedFloat4_256) {
      // a*b = {1*3, 2*3, 3*3, 4*3} = {3, 6, 9, 12}
      // -(a*b)-c = {-3-0.25, -6-0.25, -9-0.25, -12-0.25} = {-3.25, -6.25, -9.25, -12.25}
      const auto a = _mm256_set_pd(4.0, 3.0, 2.0, 1.0);
      const auto b = _mm256_set1_pd(3.0);
      const auto c = _mm256_set1_pd(0.25);
      alignas(YMM_ALIGNMENT) std::array<Float_64, VectorSizes::W> actual{};
      _mm256_store_pd(actual.data(), negativeMultiplySub(a, b, c));
      Assert::AreEqual(-3.25, actual[0], L"x lane", LINE_INFO());
      Assert::AreEqual(-6.25, actual[1], L"y lane", LINE_INFO());
      Assert::AreEqual(-9.25, actual[2], L"z lane", LINE_INFO());
      Assert::AreEqual(-12.25, actual[3], L"w lane", LINE_INFO());
    }

    TEST_METHOD(negativeMultiplySubXOperatesOnXLaneOnly) {
      // x = -(a.x*b.x)-c.x = -(2*3)-0.5 = -6.5
      const auto a = _mm_set_ps(40.0f, 30.0f, 20.0f, 2.0f);
      const auto b = _mm_set_ps(4.0f, 3.0f, 2.0f, 3.0f);
      const auto c = _mm_set_ps(1.0f, 1.0f, 1.0f, 0.5f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), negativeMultiplySubX(a, b, c));
      Assert::AreEqual(-6.5f, actual[0], L"x = -(a.x*b.x)-c.x = -(2*3)-0.5", LINE_INFO());
      Assert::AreEqual(20.0f, actual[1], L"y pass through from a", LINE_INFO());
      Assert::AreEqual(30.0f, actual[2], L"z pass through from a", LINE_INFO());
      Assert::AreEqual(40.0f, actual[3], L"w pass through from a", LINE_INFO());
    }

    TEST_METHOD(negativeMultiplySubXFloat64OperatesOnXLaneOnly) {
      // x = -(3*4)-0.5 = -12.5
      const auto a = _mm_set_pd(20.0, 3.0);
      const auto b = _mm_set_pd(2.0, 4.0);
      const auto c = _mm_set_pd(1.0, 0.5);
      std::array<Float_64, VectorSizes::X2> actual{};
      _mm_store_pd(actual.data(), negativeMultiplySubX(a, b, c));
      Assert::AreEqual(-12.5, actual[0], L"x = -(3*4)-0.5", LINE_INFO());
      Assert::AreEqual(20.0, actual[1], L"y pass through from a", LINE_INFO());
    }

    TEST_METHOD(negativeMultiplySubWithInfinity) {
      const auto a = _mm_set_ps1(Infinity<Float_32>());
      const auto b = _mm_set_ps1(Zero<Float_32>());
      const auto c = _mm_set_ps1(1.0f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), negativeMultiplySub(a, b, c));
      // -(inf*0)-1 = -NaN-1 = NaN
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::IsTrue(std::isnan(actual[i]), L"-(inf*0)-1 should be NaN", LINE_INFO());
      }
    }
  };
}
