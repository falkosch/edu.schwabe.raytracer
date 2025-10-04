#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(ValueNegativeZeroTest) {
  public:
    TEST_METHOD(returnsFloat_32) {
      Assert::AreEqual(-0.0f, NegativeZero<Float_32>());
      Assert::IsTrue(std::signbit(NegativeZero<Float_32>()));
    }

    TEST_METHOD(returnsFloat_64) {
      Assert::AreEqual(-0.0, NegativeZero<Float_64>());
      Assert::IsTrue(std::signbit(NegativeZero<Float_64>()));
    }

    TEST_METHOD(returnsPackedFloat2_128) {
      constexpr std::array expected{-0.0, -0.0};
      std::array<Float_64, VectorSizes::Y> actual{};
      _mm_store_pd(actual.data(), NegativeZero<PackedFloat2_128>());
      Assert::AreEqual(expected, actual);
      for (auto &&v : actual) {
        Assert::IsTrue(std::signbit(v));
      }
    }

    TEST_METHOD(returnsPackedFloat4_128) {
      constexpr std::array expected{-0.0f, -0.0f, -0.0f, -0.0f};
      std::array<Float_32, VectorSizes::W> actual{};
      _mm_store_ps(actual.data(), NegativeZero<PackedFloat4_128>());
      Assert::AreEqual(expected, actual);
      for (auto &&v : actual) {
        Assert::IsTrue(std::signbit(v));
      }
    }

    TEST_METHOD(returnsPackedFloat4_256) {
      constexpr std::array expected{-0.0, -0.0, -0.0, -0.0};
      std::array<Float_64, VectorSizes::W> actual{};
      _mm256_store_pd(actual.data(), NegativeZero<PackedFloat4_256>());
      Assert::AreEqual(expected, actual);
      for (auto &&v : actual) {
        Assert::IsTrue(std::signbit(v));
      }
    }

    TEST_METHOD(returnsPackedFloat8_256) {
      constexpr std::array expected{-0.0f, -0.0f, -0.0f, -0.0f, -0.0f, -0.0f, -0.0f, -0.0f};
      std::array<Float_32, VectorSizes::X8> actual{};
      _mm256_store_ps(actual.data(), NegativeZero<PackedFloat8_256>());
      Assert::AreEqual(expected, actual);
      for (auto &&v : actual) {
        Assert::IsTrue(std::signbit(v));
      }
    }
  };
}
