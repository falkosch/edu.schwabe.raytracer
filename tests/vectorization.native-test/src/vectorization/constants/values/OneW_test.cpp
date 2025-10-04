#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(ValueOneWTest) {
  public:
    TEST_METHOD(returnsPackedFloat4_128) {
      std::array<Float_32, VectorSizes::W> expected{0.0f, 0.0f, 0.0f, 1.0f};
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), OneW<PackedFloat4_128>());
      Assert::AreEqual(expected, actual);
    }

    TEST_METHOD(returnsPackedFloat4_256) {
      std::array<Float_64, VectorSizes::W> expected{0.0, 0.0, 0.0, 1.0};
      std::array<Float_64, VectorSizes::W> actual{};
      _mm256_store_pd(actual.data(), OneW<PackedFloat4_256>());
      Assert::AreEqual(expected, actual);
    }

    TEST_METHOD(returnsPackedFloat8_256) {
      std::array<Float_32, VectorSizes::X8> expected{0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
      std::array<Float_32, VectorSizes::X8> actual{};
      _mm256_store_ps(actual.data(), OneW<PackedFloat8_256>());
      Assert::AreEqual(expected, actual);
    }
  };
}
