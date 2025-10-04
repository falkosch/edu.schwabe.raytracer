#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(Blend128dTest) {
  public:
    TEST_METHOD(genericallyBlendsIdentities) {
      std::array<Float_64, VectorSizes::Y> givenOnBitNotSet{1.0, 2.0};
      auto givenOnBitNotSetPack = _mm_load_pd(givenOnBitNotSet.data());
      std::array<Float_64, VectorSizes::Y> givenOnBitSet{3.0, 4.0};
      auto givenOnBitSetPack = _mm_load_pd(givenOnBitSet.data());
      std::array<Float_64, VectorSizes::Y> actual{};

      {
        std::array<Float_64, VectorSizes::Y> expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y)
        };
        _mm_store_pd(actual.data(), blend<false, false>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::Y> expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y)
        };
        _mm_store_pd(actual.data(), blend<true, true>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallyBlendsIndividuals) {
      std::array<Float_64, VectorSizes::Y> givenOnBitNotSet{1.0, 2.0};
      auto givenOnBitNotSetPack = _mm_load_pd(givenOnBitNotSet.data());
      std::array<Float_64, VectorSizes::Y> givenOnBitSet{3.0, 4.0};
      auto givenOnBitSetPack = _mm_load_pd(givenOnBitSet.data());
      std::array<Float_64, VectorSizes::Y> actual{};

      {
        std::array<Float_64, VectorSizes::Y> expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y)
        };
        _mm_store_pd(actual.data(), blend<false, true>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::Y> expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y)
        };
        _mm_store_pd(actual.data(), blend<true, false>(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
      }
    }
  };
}
