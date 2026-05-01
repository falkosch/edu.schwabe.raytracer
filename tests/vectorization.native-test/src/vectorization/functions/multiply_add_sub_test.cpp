#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(MultiplyAddSubTest) {
  public:
    TEST_METHOD(multiplyAddSubsPackedFloat4) {
      // multiplyAddSub: even-indexed = sub, odd-indexed = add
      // a*b = {2, 4, 6, 8}, then {2-0.5, 4+0.5, 6-0.5, 8+0.5} = {1.5, 4.5, 5.5, 8.5}
      const auto a = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
      const auto b = _mm_set_ps(2.0f, 2.0f, 2.0f, 2.0f);
      const auto c = _mm_set_ps(0.5f, 0.5f, 0.5f, 0.5f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), multiplyAddSub(a, b, c));
      Assert::AreEqual(1.5f, actual[0], L"x lane: a*b-c", LINE_INFO());
      Assert::AreEqual(4.5f, actual[1], L"y lane: a*b+c", LINE_INFO());
      Assert::AreEqual(5.5f, actual[2], L"z lane: a*b-c", LINE_INFO());
      Assert::AreEqual(8.5f, actual[3], L"w lane: a*b+c", LINE_INFO());
    }

    TEST_METHOD(multiplyAddSubsPackedFloat2) {
      // a*b = {2*5, 3*4} = {10, 12}, then {10-1, 12+1} = {9, 13}
      const auto a = _mm_set_pd(3.0, 2.0);
      const auto b = _mm_set_pd(4.0, 5.0);
      const auto c = _mm_set_pd(1.0, 1.0);
      std::array<Float_64, VectorSizes::X2> actual{};
      _mm_store_pd(actual.data(), multiplyAddSub(a, b, c));
      Assert::AreEqual(9.0, actual[0], L"x lane: a*b-c", LINE_INFO());
      Assert::AreEqual(13.0, actual[1], L"y lane: a*b+c", LINE_INFO());
    }

    TEST_METHOD(multiplyAddSubsPackedFloat8) {
      // a*b = {2, 4, 6, 8, 10, 12, 14, 16}
      // even=sub, odd=add: {2-0.5, 4+0.5, 6-0.5, 8+0.5, 10-0.5, 12+0.5, 14-0.5, 16+0.5}
      const auto a = _mm256_set_ps(8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
      const auto b = _mm256_set1_ps(2.0f);
      const auto c = _mm256_set1_ps(0.5f);
      alignas(YMM_ALIGNMENT) std::array<Float_32, VectorSizes::X8> actual{};
      _mm256_store_ps(actual.data(), multiplyAddSub(a, b, c));
      Assert::AreEqual(1.5f, actual[0], L"lane 0: a*b-c", LINE_INFO());
      Assert::AreEqual(4.5f, actual[1], L"lane 1: a*b+c", LINE_INFO());
      Assert::AreEqual(5.5f, actual[2], L"lane 2: a*b-c", LINE_INFO());
      Assert::AreEqual(8.5f, actual[3], L"lane 3: a*b+c", LINE_INFO());
      Assert::AreEqual(9.5f, actual[4], L"lane 4: a*b-c", LINE_INFO());
      Assert::AreEqual(12.5f, actual[5], L"lane 5: a*b+c", LINE_INFO());
      Assert::AreEqual(13.5f, actual[6], L"lane 6: a*b-c", LINE_INFO());
      Assert::AreEqual(16.5f, actual[7], L"lane 7: a*b+c", LINE_INFO());
    }

    TEST_METHOD(multiplyAddSubsPackedFloat4_256) {
      // a*b = {1*3, 2*3, 3*3, 4*3} = {3, 6, 9, 12}
      // even=sub, odd=add: {3-0.25, 6+0.25, 9-0.25, 12+0.25} = {2.75, 6.25, 8.75, 12.25}
      const auto a = _mm256_set_pd(4.0, 3.0, 2.0, 1.0);
      const auto b = _mm256_set1_pd(3.0);
      const auto c = _mm256_set1_pd(0.25);
      alignas(YMM_ALIGNMENT) std::array<Float_64, VectorSizes::W> actual{};
      _mm256_store_pd(actual.data(), multiplyAddSub(a, b, c));
      Assert::AreEqual(2.75, actual[0], L"x lane: a*b-c", LINE_INFO());
      Assert::AreEqual(6.25, actual[1], L"y lane: a*b+c", LINE_INFO());
      Assert::AreEqual(8.75, actual[2], L"z lane: a*b-c", LINE_INFO());
      Assert::AreEqual(12.25, actual[3], L"w lane: a*b+c", LINE_INFO());
    }

    TEST_METHOD(multiplyAddSubWithZero) {
      const auto a = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
      const auto b = _mm_set_ps(2.0f, 2.0f, 2.0f, 2.0f);
      const auto c = _mm_setzero_ps();
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), multiplyAddSub(a, b, c));
      // With c=0, even=sub and odd=add both give a*b
      Assert::AreEqual(2.0f, actual[0], L"x lane: a*b-0", LINE_INFO());
      Assert::AreEqual(4.0f, actual[1], L"y lane: a*b+0", LINE_INFO());
      Assert::AreEqual(6.0f, actual[2], L"z lane: a*b-0", LINE_INFO());
      Assert::AreEqual(8.0f, actual[3], L"w lane: a*b+0", LINE_INFO());
    }
  };
}
