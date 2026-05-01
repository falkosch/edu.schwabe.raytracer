#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(HorizontalSubtractTest) {
  public:
    TEST_METHOD(horizontalSubtractsPackedFloat4) {
      // _mm_hsub_ps semantics:
      // result[0] = a[0]-a[1], result[1] = a[2]-a[3], result[2] = b[0]-b[1], result[3] = b[2]-b[3]
      const auto a = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
      const auto b = _mm_set_ps(8.0f, 7.0f, 6.0f, 5.0f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), horizontalSubtract(a, b));
      Assert::AreEqual(-1.0f, actual[0], L"a[0]-a[1]", LINE_INFO());
      Assert::AreEqual(-1.0f, actual[1], L"a[2]-a[3]", LINE_INFO());
      Assert::AreEqual(-1.0f, actual[2], L"b[0]-b[1]", LINE_INFO());
      Assert::AreEqual(-1.0f, actual[3], L"b[2]-b[3]", LINE_INFO());
    }

    TEST_METHOD(horizontalSubtractsPackedFloat2) {
      // _mm_hsub_pd semantics:
      // result[0] = a[0]-a[1], result[1] = b[0]-b[1]
      const auto a = _mm_set_pd(3.0, 10.0);
      const auto b = _mm_set_pd(7.0, 20.0);
      std::array<Float_64, VectorSizes::X2> actual{};
      _mm_store_pd(actual.data(), horizontalSubtract(a, b));
      Assert::AreEqual(7.0, actual[0], L"a[0]-a[1]", LINE_INFO());
      Assert::AreEqual(13.0, actual[1], L"b[0]-b[1]", LINE_INFO());
    }

    TEST_METHOD(horizontalSubtractsPackedFloat8) {
      // _mm256_hsub_ps operates per 128-bit lane:
      // result[0] = a[0]-a[1], result[1] = a[2]-a[3], result[2] = b[0]-b[1], result[3] = b[2]-b[3]
      // result[4] = a[4]-a[5], result[5] = a[6]-a[7], result[6] = b[4]-b[5], result[7] = b[6]-b[7]
      const auto a = _mm256_set_ps(8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 3.0f, 10.0f);
      const auto b = _mm256_set_ps(16.0f, 15.0f, 14.0f, 13.0f, 12.0f, 11.0f, 10.0f, 20.0f);
      alignas(YMM_ALIGNMENT) std::array<Float_32, VectorSizes::X8> actual{};
      _mm256_store_ps(actual.data(), horizontalSubtract(a, b));
      Assert::AreEqual(7.0f, actual[0], L"a[0]-a[1]", LINE_INFO());
      Assert::AreEqual(-1.0f, actual[1], L"a[2]-a[3]", LINE_INFO());
      Assert::AreEqual(10.0f, actual[2], L"b[0]-b[1]", LINE_INFO());
      Assert::AreEqual(-1.0f, actual[3], L"b[2]-b[3]", LINE_INFO());
      Assert::AreEqual(-1.0f, actual[4], L"a[4]-a[5]", LINE_INFO());
      Assert::AreEqual(-1.0f, actual[5], L"a[6]-a[7]", LINE_INFO());
      Assert::AreEqual(-1.0f, actual[6], L"b[4]-b[5]", LINE_INFO());
      Assert::AreEqual(-1.0f, actual[7], L"b[6]-b[7]", LINE_INFO());
    }

    TEST_METHOD(horizontalSubtractsPackedFloat4_256) {
      // _mm256_hsub_pd operates per 128-bit lane:
      // result[0] = a[0]-a[1], result[1] = b[0]-b[1], result[2] = a[2]-a[3], result[3] = b[2]-b[3]
      const auto a = _mm256_set_pd(4.0, 3.0, 2.0, 10.0);
      const auto b = _mm256_set_pd(8.0, 7.0, 6.0, 20.0);
      alignas(YMM_ALIGNMENT) std::array<Float_64, VectorSizes::W> actual{};
      _mm256_store_pd(actual.data(), horizontalSubtract(a, b));
      Assert::AreEqual(8.0, actual[0], L"a[0]-a[1]", LINE_INFO());
      Assert::AreEqual(14.0, actual[1], L"b[0]-b[1]", LINE_INFO());
      Assert::AreEqual(-1.0, actual[2], L"a[2]-a[3]", LINE_INFO());
      Assert::AreEqual(-1.0, actual[3], L"b[2]-b[3]", LINE_INFO());
    }

    TEST_METHOD(horizontalSubtractsUniformProducesZero) {
      const auto a = _mm_set_ps1(5.0f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), horizontalSubtract(a, a));
      for (ASizeT i = 0; i < actual.size(); ++i) {
        Assert::AreEqual(0.0f, actual[i], L"uniform hsub should be zero", LINE_INFO());
      }
    }
  };
}
