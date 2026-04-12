#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(BlendMasked128dTest) {
  public:
    TEST_METHOD(variadicSwizzledBlendsIdentityOfTwoFloat2_64WithFloat2_64BitMask) {
      constexpr std::array givenOnBitNotSet{1.0, 2.0};
      const auto givenOnBitNotSetPack = _mm_load_pd(givenOnBitNotSet.data());
      constexpr std::array givenOnBitSet{3.0, 4.0};
      const auto givenOnBitSetPack = _mm_load_pd(givenOnBitSet.data());
      std::array<Float_64, VectorSizes::Y> actual{};

      {
        constexpr std::array expected{givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y)};
        _mm_store_pd(
            actual.data(), swizzledBlendMasked<VectorIndices::X, VectorIndices::Y>(
                               givenOnBitNotSetPack, givenOnBitSetPack, MaskNone<PackedFloat2_128>()
                           )
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y)};
        _mm_store_pd(
            actual.data(), swizzledBlendMasked<VectorIndices::X, VectorIndices::Y>(
                               givenOnBitNotSetPack, givenOnBitSetPack, MaskAll<PackedFloat2_128>()
                           )
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y)};
        _mm_store_pd(actual.data(), blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, MaskXY<PackedFloat2_128>()));
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(variadicBlendsTwoFloat2_64WithBool2_64BitMask) {
      constexpr std::array givenOnBitNotSet{1.0, 2.0};
      const auto givenOnBitNotSetPack = _mm_load_pd(givenOnBitNotSet.data());
      constexpr std::array givenOnBitSet{3.0, 4.0};
      const auto givenOnBitSetPack = _mm_load_pd(givenOnBitSet.data());
      std::array<Float_64, VectorSizes::Y> actual{};

      {
        constexpr std::array expected{givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y)};
        _mm_store_pd(
            actual.data(),
            blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, _mm_castpd_si128(MaskNone<PackedFloat2_128>()))
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y)};
        _mm_store_pd(
            actual.data(),
            blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, _mm_castpd_si128(MaskX<PackedFloat2_128>()))
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y)};
        _mm_store_pd(
            actual.data(),
            blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, _mm_castpd_si128(MaskXY<PackedFloat2_128>()))
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }
    }
  };
}
