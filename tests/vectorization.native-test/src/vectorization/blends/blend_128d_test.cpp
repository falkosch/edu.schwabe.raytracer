#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(Blend128dTest) {
  public:
    TEST_METHOD(genericallyBlendsIdentities) {
      constexpr std::array givenOnBitNotSet{1.0, 2.0};
      const auto givenOnBitNotSetPack = _mm_load_pd(givenOnBitNotSet.data());
      constexpr std::array givenOnBitSet{3.0, 4.0};
      const auto givenOnBitSetPack = _mm_load_pd(givenOnBitSet.data());
      std::array<Float_64, VectorSizes::Y> actual{};

      {
        constexpr std::array expected{givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y)};
        _mm_store_pd(actual.data(), blend<false, false>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y)};
        _mm_store_pd(actual.data(), blend<true, true>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallyBlendsIndividuals) {
      constexpr std::array givenOnBitNotSet{1.0, 2.0};
      const auto givenOnBitNotSetPack = _mm_load_pd(givenOnBitNotSet.data());
      constexpr std::array givenOnBitSet{3.0, 4.0};
      const auto givenOnBitSetPack = _mm_load_pd(givenOnBitSet.data());
      std::array<Float_64, VectorSizes::Y> actual{};

      {
        constexpr std::array expected{givenOnBitNotSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y)};
        _mm_store_pd(actual.data(), blend<false, true>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y)};
        _mm_store_pd(actual.data(), blend<true, false>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }
    }
  };
}
