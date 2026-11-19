#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(SwizzledBlends128dTest) {
  public:
    TEST_METHOD(genericallySwizzleBlendsIdentity) {
      alignas(XMM_ALIGNMENT) constexpr std::array givenOnBitNotSet{1.0, 2.0};
      const auto givenOnBitNotSetPack = _mm_load_pd(givenOnBitNotSet.data());
      alignas(XMM_ALIGNMENT) constexpr std::array givenOnBitSet{3.0, 4.0};
      const auto givenOnBitSetPack = _mm_load_pd(givenOnBitSet.data());
      alignas(XMM_ALIGNMENT) std::array<Float_64, VectorSizes::Y> actual{};

      {
        constexpr std::array expected{givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y)};
        _mm_store_pd(
            actual.data(),
            swizzledBlend<VectorIndices::X, VectorIndices::Y, false, false>(givenOnBitNotSetPack, givenOnBitSetPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y)};
        _mm_store_pd(
            actual.data(),
            swizzledBlend<VectorIndices::X, VectorIndices::Y, true, true>(givenOnBitNotSetPack, givenOnBitSetPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallySwizzleBlendsBroadcast) {
      alignas(XMM_ALIGNMENT) constexpr std::array givenOnBitNotSet{1.0, 2.0};
      const auto givenOnBitNotSetPack = _mm_load_pd(givenOnBitNotSet.data());
      alignas(XMM_ALIGNMENT) constexpr std::array givenOnBitSet{3.0, 4.0};
      const auto givenOnBitSetPack = _mm_load_pd(givenOnBitSet.data());
      alignas(XMM_ALIGNMENT) std::array<Float_64, VectorSizes::Y> actual{};

      {
        constexpr std::array expected{givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::X)};
        _mm_store_pd(
            actual.data(),
            swizzledBlend<VectorIndices::X, VectorIndices::X, false, false>(givenOnBitNotSetPack, givenOnBitSetPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::X)};
        _mm_store_pd(
            actual.data(),
            swizzledBlend<VectorIndices::X, VectorIndices::X, true, true>(givenOnBitNotSetPack, givenOnBitSetPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{givenOnBitNotSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Y)};
        _mm_store_pd(
            actual.data(),
            swizzledBlend<VectorIndices::Y, VectorIndices::Y, false, true>(givenOnBitNotSetPack, givenOnBitSetPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{givenOnBitSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Y)};
        _mm_store_pd(
            actual.data(),
            swizzledBlend<VectorIndices::Y, VectorIndices::Y, true, false>(givenOnBitNotSetPack, givenOnBitSetPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallySwizzleBlendsAny) {
      alignas(XMM_ALIGNMENT) constexpr std::array givenOnBitNotSet{1.0, 2.0};
      const auto givenOnBitNotSetPack = _mm_load_pd(givenOnBitNotSet.data());
      alignas(XMM_ALIGNMENT) constexpr std::array givenOnBitSet{3.0, 4.0};
      const auto givenOnBitSetPack = _mm_load_pd(givenOnBitSet.data());
      alignas(XMM_ALIGNMENT) std::array<Float_64, VectorSizes::Y> actual{};

      {
        constexpr std::array expected{givenOnBitNotSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::X)};
        _mm_store_pd(
            actual.data(),
            swizzledBlend<VectorIndices::Y, VectorIndices::X, false, true>(givenOnBitNotSetPack, givenOnBitSetPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{givenOnBitSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::X)};
        _mm_store_pd(
            actual.data(),
            swizzledBlend<VectorIndices::Y, VectorIndices::X, true, false>(givenOnBitNotSetPack, givenOnBitSetPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }
    }
  };
}
