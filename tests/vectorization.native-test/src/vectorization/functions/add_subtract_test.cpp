#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(AddSubtractTest) {
  public:
    TEST_METHOD(addSubtractsPackedFloat4) {
      // _mm_addsub_ps semantics (alternating sub/add):
      // result[0] = a[0]-b[0], result[1] = a[1]+b[1], result[2] = a[2]-b[2], result[3] = a[3]+b[3]
      const auto a = _mm_set_ps(10.0f, 10.0f, 10.0f, 10.0f);
      const auto b = _mm_set_ps(3.0f, 3.0f, 3.0f, 3.0f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), addSubtract(a, b));
      Assert::AreEqual(7.0f, actual[0], L"x lane: a[0]-b[0]", LINE_INFO());
      Assert::AreEqual(13.0f, actual[1], L"y lane: a[1]+b[1]", LINE_INFO());
      Assert::AreEqual(7.0f, actual[2], L"z lane: a[2]-b[2]", LINE_INFO());
      Assert::AreEqual(13.0f, actual[3], L"w lane: a[3]+b[3]", LINE_INFO());
    }

    TEST_METHOD(addSubtractsPackedFloat2) {
      // _mm_addsub_pd semantics:
      // result[0] = a[0]-b[0], result[1] = a[1]+b[1]
      const auto a = _mm_set_pd(10.0, 10.0);
      const auto b = _mm_set_pd(3.0, 3.0);
      std::array<Float_64, VectorSizes::X2> actual{};
      _mm_store_pd(actual.data(), addSubtract(a, b));
      Assert::AreEqual(7.0, actual[0], L"x lane: a[0]-b[0]", LINE_INFO());
      Assert::AreEqual(13.0, actual[1], L"y lane: a[1]+b[1]", LINE_INFO());
    }

    TEST_METHOD(addSubtractsPackedFloat8) {
      // _mm256_addsub_ps operates per 128-bit lane with the same alternating pattern:
      // result[0]=a[0]-b[0], result[1]=a[1]+b[1], result[2]=a[2]-b[2], result[3]=a[3]+b[3]
      // result[4]=a[4]-b[4], result[5]=a[5]+b[5], result[6]=a[6]-b[6], result[7]=a[7]+b[7]
      const auto a = _mm256_set_ps(20.0f, 20.0f, 20.0f, 20.0f, 10.0f, 10.0f, 10.0f, 10.0f);
      const auto b = _mm256_set_ps(5.0f, 5.0f, 5.0f, 5.0f, 3.0f, 3.0f, 3.0f, 3.0f);
      alignas(YMM_ALIGNMENT) std::array<Float_32, VectorSizes::X8> actual{};
      _mm256_store_ps(actual.data(), addSubtract(a, b));
      Assert::AreEqual(7.0f, actual[0], L"a[0]-b[0]", LINE_INFO());
      Assert::AreEqual(13.0f, actual[1], L"a[1]+b[1]", LINE_INFO());
      Assert::AreEqual(7.0f, actual[2], L"a[2]-b[2]", LINE_INFO());
      Assert::AreEqual(13.0f, actual[3], L"a[3]+b[3]", LINE_INFO());
      Assert::AreEqual(15.0f, actual[4], L"a[4]-b[4]", LINE_INFO());
      Assert::AreEqual(25.0f, actual[5], L"a[5]+b[5]", LINE_INFO());
      Assert::AreEqual(15.0f, actual[6], L"a[6]-b[6]", LINE_INFO());
      Assert::AreEqual(25.0f, actual[7], L"a[7]+b[7]", LINE_INFO());
    }

    TEST_METHOD(addSubtractsPackedFloat4_256) {
      // _mm256_addsub_pd semantics (per 128-bit lane):
      // result[0] = a[0]-b[0], result[1] = a[1]+b[1], result[2] = a[2]-b[2], result[3] = a[3]+b[3]
      const auto a = _mm256_set_pd(10.0, 10.0, 10.0, 10.0);
      const auto b = _mm256_set_pd(4.0, 4.0, 4.0, 4.0);
      alignas(YMM_ALIGNMENT) std::array<Float_64, VectorSizes::W> actual{};
      _mm256_store_pd(actual.data(), addSubtract(a, b));
      Assert::AreEqual(6.0, actual[0], L"a[0]-b[0]", LINE_INFO());
      Assert::AreEqual(14.0, actual[1], L"a[1]+b[1]", LINE_INFO());
      Assert::AreEqual(6.0, actual[2], L"a[2]-b[2]", LINE_INFO());
      Assert::AreEqual(14.0, actual[3], L"a[3]+b[3]", LINE_INFO());
    }

    TEST_METHOD(addSubtractsWithZero) {
      const auto a = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
      const auto b = _mm_set_ps1(Zero<Float_32>());
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), addSubtract(a, b));
      // Even lanes subtract zero, odd lanes add zero, so all should equal the original
      Assert::AreEqual(1.0f, actual[0], L"x lane: a[0]-0", LINE_INFO());
      Assert::AreEqual(2.0f, actual[1], L"y lane: a[1]+0", LINE_INFO());
      Assert::AreEqual(3.0f, actual[2], L"z lane: a[2]-0", LINE_INFO());
      Assert::AreEqual(4.0f, actual[3], L"w lane: a[3]+0", LINE_INFO());
    }
  };
}
