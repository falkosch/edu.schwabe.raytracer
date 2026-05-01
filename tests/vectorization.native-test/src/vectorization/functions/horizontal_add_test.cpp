#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(HorizontalAddTest) {
  public:
    TEST_METHOD(horizontalAddsPackedFloat4) {
      // _mm_hadd_ps semantics:
      // result[0] = a[0]+a[1], result[1] = a[2]+a[3], result[2] = b[0]+b[1], result[3] = b[2]+b[3]
      const auto a = _mm_set_ps(4.0f, 3.0f, 2.0f, 1.0f);
      const auto b = _mm_set_ps(8.0f, 7.0f, 6.0f, 5.0f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), horizontalAdd(a, b));
      Assert::AreEqual(3.0f, actual[0], L"a[0]+a[1]", LINE_INFO());
      Assert::AreEqual(7.0f, actual[1], L"a[2]+a[3]", LINE_INFO());
      Assert::AreEqual(11.0f, actual[2], L"b[0]+b[1]", LINE_INFO());
      Assert::AreEqual(15.0f, actual[3], L"b[2]+b[3]", LINE_INFO());
    }

    TEST_METHOD(horizontalAddsPackedFloat2) {
      // _mm_hadd_pd semantics:
      // result[0] = a[0]+a[1], result[1] = b[0]+b[1]
      const auto a = _mm_set_pd(3.0, 1.0);
      const auto b = _mm_set_pd(7.0, 5.0);
      std::array<Float_64, VectorSizes::X2> actual{};
      _mm_store_pd(actual.data(), horizontalAdd(a, b));
      Assert::AreEqual(4.0, actual[0], L"a[0]+a[1]", LINE_INFO());
      Assert::AreEqual(12.0, actual[1], L"b[0]+b[1]", LINE_INFO());
    }

    TEST_METHOD(horizontalAddsPackedFloat8) {
      // _mm256_hadd_ps operates per 128-bit lane:
      // result[0] = a[0]+a[1], result[1] = a[2]+a[3], result[2] = b[0]+b[1], result[3] = b[2]+b[3]
      // result[4] = a[4]+a[5], result[5] = a[6]+a[7], result[6] = b[4]+b[5], result[7] = b[6]+b[7]
      const auto a = _mm256_set_ps(8.0f, 7.0f, 6.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
      const auto b = _mm256_set_ps(16.0f, 15.0f, 14.0f, 13.0f, 12.0f, 11.0f, 10.0f, 9.0f);
      alignas(YMM_ALIGNMENT) std::array<Float_32, VectorSizes::X8> actual{};
      _mm256_store_ps(actual.data(), horizontalAdd(a, b));
      Assert::AreEqual(3.0f, actual[0], L"a[0]+a[1]", LINE_INFO());
      Assert::AreEqual(7.0f, actual[1], L"a[2]+a[3]", LINE_INFO());
      Assert::AreEqual(19.0f, actual[2], L"b[0]+b[1]", LINE_INFO());
      Assert::AreEqual(23.0f, actual[3], L"b[2]+b[3]", LINE_INFO());
      Assert::AreEqual(11.0f, actual[4], L"a[4]+a[5]", LINE_INFO());
      Assert::AreEqual(15.0f, actual[5], L"a[6]+a[7]", LINE_INFO());
      Assert::AreEqual(27.0f, actual[6], L"b[4]+b[5]", LINE_INFO());
      Assert::AreEqual(31.0f, actual[7], L"b[6]+b[7]", LINE_INFO());
    }

    TEST_METHOD(horizontalAddsPackedFloat4_256) {
      // _mm256_hadd_pd operates per 128-bit lane:
      // result[0] = a[0]+a[1], result[1] = b[0]+b[1], result[2] = a[2]+a[3], result[3] = b[2]+b[3]
      const auto a = _mm256_set_pd(4.0, 3.0, 2.0, 1.0);
      const auto b = _mm256_set_pd(8.0, 7.0, 6.0, 5.0);
      alignas(YMM_ALIGNMENT) std::array<Float_64, VectorSizes::W> actual{};
      _mm256_store_pd(actual.data(), horizontalAdd(a, b));
      Assert::AreEqual(3.0, actual[0], L"a[0]+a[1]", LINE_INFO());
      Assert::AreEqual(11.0, actual[1], L"b[0]+b[1]", LINE_INFO());
      Assert::AreEqual(7.0, actual[2], L"a[2]+a[3]", LINE_INFO());
      Assert::AreEqual(15.0, actual[3], L"b[2]+b[3]", LINE_INFO());
    }

    TEST_METHOD(horizontalAddsWithInfinity) {
      const auto a = _mm_set_ps(0.0f, 0.0f, Infinity<Float_32>(), NegativeInfinity<Float_32>());
      const auto b = _mm_set_ps(1.0f, 2.0f, 3.0f, 4.0f);
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), horizontalAdd(a, b));
      Assert::IsTrue(std::isnan(actual[0]), L"inf + (-inf) should be NaN", LINE_INFO());
      Assert::AreEqual(0.0f, actual[1], L"0+0", LINE_INFO());
      Assert::AreEqual(7.0f, actual[2], L"b[0]+b[1]", LINE_INFO());
      Assert::AreEqual(3.0f, actual[3], L"b[2]+b[3]", LINE_INFO());
    }
  };
}
