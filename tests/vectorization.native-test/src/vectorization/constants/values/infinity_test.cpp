#include "vectorization_tests.h"

#include <limits>

namespace vectorization::test {
  TEST_CLASS(ValueInfinityTest) {
  public:
    TEST_METHOD(returnsFloat_32) {
      Assert::AreEqual(std::numeric_limits<Float_32>::infinity(), Infinity<Float_32>());
    }

    TEST_METHOD(returnsFloat_64) {
      Assert::AreEqual(std::numeric_limits<Float_64>::infinity(), Infinity<Float_64>());
    }

    TEST_METHOD(returnsPackedFloat2_128) {
      const std::array expected{std::numeric_limits<Float_64>::infinity(), std::numeric_limits<Float_64>::infinity()};
      std::array<Float_64, VectorSizes::Y> actual{};
      _mm_store_pd(actual.data(), Infinity<PackedFloat2_128>());
      Assert::AreEqual(expected, actual);
    }

    TEST_METHOD(returnsPackedFloat4_128) {
      const std::array expected{
          std::numeric_limits<Float_32>::infinity(), std::numeric_limits<Float_32>::infinity(),
          std::numeric_limits<Float_32>::infinity(), std::numeric_limits<Float_32>::infinity()
      };
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), Infinity<PackedFloat4_128>());
      Assert::AreEqual(expected, actual);
    }

    TEST_METHOD(returnsPackedFloat4_256) {
      const std::array expected{
          std::numeric_limits<Float_64>::infinity(), std::numeric_limits<Float_64>::infinity(),
          std::numeric_limits<Float_64>::infinity(), std::numeric_limits<Float_64>::infinity()
      };
      std::array<Float_64, VectorSizes::W> actual{};
      _mm256_store_pd(actual.data(), Infinity<PackedFloat4_256>());
      Assert::AreEqual(expected, actual);
    }

    TEST_METHOD(returnsPackedFloat8_256) {
      const std::array expected{std::numeric_limits<Float_32>::infinity(), std::numeric_limits<Float_32>::infinity(),
                          std::numeric_limits<Float_32>::infinity(), std::numeric_limits<Float_32>::infinity(),
                          std::numeric_limits<Float_32>::infinity(), std::numeric_limits<Float_32>::infinity(),
                          std::numeric_limits<Float_32>::infinity(), std::numeric_limits<Float_32>::infinity()};
      std::array<Float_32, VectorSizes::X8> actual{};
      _mm256_store_ps(actual.data(), Infinity<PackedFloat8_256>());
      Assert::AreEqual(expected, actual);
    }
  };
}
