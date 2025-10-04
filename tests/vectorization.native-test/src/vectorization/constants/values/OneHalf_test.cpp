#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(ValueOneHalfTest) {
  public:
    TEST_METHOD(returnsFloat_32) {
      Assert::AreEqual(1.5f, OneHalf<Float_32>());
    }

    TEST_METHOD(returnsFloat_64) {
      Assert::AreEqual(1.5, OneHalf<Float_64>());
    }

    TEST_METHOD(returnsPackedFloat2_128) {
      constexpr std::array expected{1.5, 1.5};
      std::array<Float_64, VectorSizes::Y> actual{};
      _mm_store_pd(actual.data(), OneHalf<PackedFloat2_128>());
      Assert::AreEqual(expected, actual);
    }

    TEST_METHOD(returnsPackedFloat4_128) {
      constexpr std::array expected{1.5f, 1.5f, 1.5f, 1.5f};
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), OneHalf<PackedFloat4_128>());
      Assert::AreEqual(expected, actual);
    }

    TEST_METHOD(returnsPackedFloat4_256) {
      constexpr std::array expected{1.5, 1.5, 1.5, 1.5};
      std::array<Float_64, VectorSizes::W> actual{};
      _mm256_store_pd(actual.data(), OneHalf<PackedFloat4_256>());
      Assert::AreEqual(expected, actual);
    }

    TEST_METHOD(returnsPackedFloat8_256) {
      constexpr std::array expected{1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f};
      std::array<Float_32, VectorSizes::X8> actual{};
      _mm256_store_ps(actual.data(), OneHalf<PackedFloat8_256>());
      Assert::AreEqual(expected, actual);
    }
  };
}
