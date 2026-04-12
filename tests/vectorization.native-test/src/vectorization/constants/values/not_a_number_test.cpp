#include "vectorization_tests.h"

#include <cmath>

namespace vectorization::test {
  TEST_CLASS(ValueNotANumberTest) {
  public:
    TEST_METHOD(returnsFloat_32) {
      Assert::IsTrue(std::isnan(NotANumber<Float_32>()), L"expected NaN", LINE_INFO());
    }

    TEST_METHOD(returnsFloat_64) {
      Assert::IsTrue(std::isnan(NotANumber<Float_64>()), L"expected NaN", LINE_INFO());
    }

    TEST_METHOD(returnsPackedFloat2_128) {
      std::array<Float_64, VectorSizes::Y> actual{};
      _mm_store_pd(actual.data(), NotANumber<PackedFloat2_128>());
      for (const auto v : actual)
        Assert::IsTrue(std::isnan(v), L"expected NaN", LINE_INFO());
    }

    TEST_METHOD(returnsPackedFloat4_128) {
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), NotANumber<PackedFloat4_128>());
      for (const auto v : actual)
        Assert::IsTrue(std::isnan(v), L"expected NaN", LINE_INFO());
    }

    TEST_METHOD(returnsPackedFloat4_256) {
      std::array<Float_64, VectorSizes::W> actual{};
      _mm256_store_pd(actual.data(), NotANumber<PackedFloat4_256>());
      for (const auto v : actual)
        Assert::IsTrue(std::isnan(v), L"expected NaN", LINE_INFO());
    }

    TEST_METHOD(returnsPackedFloat8_256) {
      std::array<Float_32, VectorSizes::X8> actual{};
      _mm256_store_ps(actual.data(), NotANumber<PackedFloat8_256>());
      for (const auto v : actual)
        Assert::IsTrue(std::isnan(v), L"expected NaN", LINE_INFO());
    }
  };
}
