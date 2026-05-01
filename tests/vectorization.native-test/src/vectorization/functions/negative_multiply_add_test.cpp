#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(NegativeMultiplyAddTest) {
  public:
    TEST_METHOD(negativeMultiplyAddsPackedFloat4) {
      // negativeMultiplyAdd: -(a*b)+c = c-a*b
      // a*b = {1*4, 2*3, 3*2, 4*1} = {4, 6, 6, 4}
      // -(a*b)+c = {10-4, 10-6, 10-6, 10-4} = {6, 4, 4, 6}
      const auto a = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
      const auto b = _mm_set_ps(1.0f, 2.0f, 3.0f, 4.0f);
      const auto c = _mm_set_ps(10.0f, 10.0f, 10.0f, 10.0f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), negativeMultiplyAdd(a, b, c));
      Assert::AreEqual(6.0f, actual[0], L"x lane", LINE_INFO());
      Assert::AreEqual(4.0f, actual[1], L"y lane", LINE_INFO());
      Assert::AreEqual(4.0f, actual[2], L"z lane", LINE_INFO());
      Assert::AreEqual(6.0f, actual[3], L"w lane", LINE_INFO());
    }

    TEST_METHOD(negativeMultiplyAddsPackedFloat2) {
      // a*b = {2*5, 3*4} = {10, 12}
      // -(a*b)+c = {1-10, 1-12} = {-9, -11}
      const auto a = _mm_set_pd(3.0, 2.0);
      const auto b = _mm_set_pd(4.0, 5.0);
      const auto c = _mm_set_pd(1.0, 1.0);
      std::array<Float_64, VectorSizes::X2> actual{};
      _mm_store_pd(actual.data(), negativeMultiplyAdd(a, b, c));
      Assert::AreEqual(-9.0, actual[0], L"x lane", LINE_INFO());
      Assert::AreEqual(-11.0, actual[1], L"y lane", LINE_INFO());
    }

    TEST_METHOD(negativeMultiplyAddsPackedFloat8) {
      // a*b = {2, 4, 6, 8, 10, 12, 14, 16}
      // -(a*b)+c = {0.5-2, 0.5-4, ...} = {-1.5, -3.5, -5.5, -7.5, -9.5, -11.5, -13.5, -15.5}
      const auto a = _mm256_set_ps(8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
      const auto b = _mm256_set1_ps(2.0f);
      const auto c = _mm256_set1_ps(0.5f);
      alignas(YMM_ALIGNMENT) std::array<Float_32, VectorSizes::X8> actual{};
      _mm256_store_ps(actual.data(), negativeMultiplyAdd(a, b, c));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(0.5f - static_cast<Float_32>((i + 1) * 2), actual[i], L"lane mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(negativeMultiplyAddsPackedFloat4_256) {
      // a*b = {1*3, 2*3, 3*3, 4*3} = {3, 6, 9, 12}
      // -(a*b)+c = {0.25-3, 0.25-6, 0.25-9, 0.25-12} = {-2.75, -5.75, -8.75, -11.75}
      const auto a = _mm256_set_pd(4.0, 3.0, 2.0, 1.0);
      const auto b = _mm256_set1_pd(3.0);
      const auto c = _mm256_set1_pd(0.25);
      alignas(YMM_ALIGNMENT) std::array<Float_64, VectorSizes::W> actual{};
      _mm256_store_pd(actual.data(), negativeMultiplyAdd(a, b, c));
      Assert::AreEqual(-2.75, actual[0], L"x lane", LINE_INFO());
      Assert::AreEqual(-5.75, actual[1], L"y lane", LINE_INFO());
      Assert::AreEqual(-8.75, actual[2], L"z lane", LINE_INFO());
      Assert::AreEqual(-11.75, actual[3], L"w lane", LINE_INFO());
    }

    TEST_METHOD(negativeMultiplyAddXOperatesOnXLaneOnly) {
      // x = -(a.x*b.x)+c.x = -(2*3)+0.5 = -5.5
      const auto a = _mm_set_ps(40.0f, 30.0f, 20.0f, 2.0f);
      const auto b = _mm_set_ps(4.0f, 3.0f, 2.0f, 3.0f);
      const auto c = _mm_set_ps(1.0f, 1.0f, 1.0f, 0.5f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), negativeMultiplyAddX(a, b, c));
      Assert::AreEqual(-5.5f, actual[0], L"x = -(a.x*b.x)+c.x = -(2*3)+0.5", LINE_INFO());
      Assert::AreEqual(20.0f, actual[1], L"y pass through from a", LINE_INFO());
      Assert::AreEqual(30.0f, actual[2], L"z pass through from a", LINE_INFO());
      Assert::AreEqual(40.0f, actual[3], L"w pass through from a", LINE_INFO());
    }

    TEST_METHOD(negativeMultiplyAddXFloat64OperatesOnXLaneOnly) {
      // x = -(3*4)+0.5 = -11.5
      const auto a = _mm_set_pd(20.0, 3.0);
      const auto b = _mm_set_pd(2.0, 4.0);
      const auto c = _mm_set_pd(1.0, 0.5);
      std::array<Float_64, VectorSizes::X2> actual{};
      _mm_store_pd(actual.data(), negativeMultiplyAddX(a, b, c));
      Assert::AreEqual(-11.5, actual[0], L"x = -(3*4)+0.5", LINE_INFO());
      Assert::AreEqual(20.0, actual[1], L"y pass through from a", LINE_INFO());
    }

    TEST_METHOD(negativeMultiplyAddWithInfinity) {
      const auto a = _mm_set_ps1(Infinity<Float_32>());
      const auto b = _mm_set_ps1(Zero<Float_32>());
      const auto c = _mm_set_ps1(1.0f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), negativeMultiplyAdd(a, b, c));
      // -(inf*0)+1 = -NaN+1 = NaN
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::IsTrue(std::isnan(actual[i]), L"-(inf*0)+1 should be NaN", LINE_INFO());
      }
    }
  };
}
