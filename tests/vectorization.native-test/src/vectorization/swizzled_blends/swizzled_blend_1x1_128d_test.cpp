#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(SwizzledBlends1x1_128dTest) {
  public:
    TEST_METHOD(genericallySwizzleBlendsIdentityAndItsReverse) {
      constexpr std::array givenOnBitNotSet{1.0, 2.0};
      const auto givenOnBitNotSetPack = _mm_load_pd(givenOnBitNotSet.data());
      constexpr std::array givenOnBitSet{3.0, 4.0};
      const auto givenOnBitSetPack = _mm_load_pd(givenOnBitSet.data());
      std::array<Float_64, VectorSizes::Y> actual{};

      {
        constexpr std::array expected{givenOnBitNotSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y)};
        _mm_store_pd(
            actual.data(), swizzledBlend1x1<VectorIndices::X, VectorIndices::Y>(givenOnBitNotSetPack, givenOnBitSetPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{givenOnBitNotSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::X)};
        _mm_store_pd(
            actual.data(), swizzledBlend1x1<VectorIndices::Y, VectorIndices::X>(givenOnBitNotSetPack, givenOnBitSetPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallySwizzleBlendsBroadcast) {
      constexpr std::array givenOnBitNotSet{1.0, 2.0};
      const auto givenOnBitNotSetPack = _mm_load_pd(givenOnBitNotSet.data());
      constexpr std::array givenOnBitSet{3.0, 4.0};
      const auto givenOnBitSetPack = _mm_load_pd(givenOnBitSet.data());
      std::array<Float_64, VectorSizes::Y> actual{};

      {
        constexpr std::array expected{givenOnBitNotSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::X)};
        _mm_store_pd(
            actual.data(), swizzledBlend1x1<VectorIndices::X, VectorIndices::X>(givenOnBitNotSetPack, givenOnBitSetPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{givenOnBitNotSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Y)};
        _mm_store_pd(
            actual.data(), swizzledBlend1x1<VectorIndices::Y, VectorIndices::Y>(givenOnBitNotSetPack, givenOnBitSetPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(hasShortFormsForSwizzleBlends) {
      constexpr std::array givenOnBitNotSet{1.0, 2.0};
      const auto givenOnBitNotSetPack = _mm_load_pd(givenOnBitNotSet.data());
      constexpr std::array givenOnBitSet{3.0, 4.0};
      const auto givenOnBitSetPack = _mm_load_pd(givenOnBitSet.data());
      std::array<Float_64, VectorSizes::Y> actual{};

      {
        constexpr std::array expected{givenOnBitNotSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::X)};
        _mm_store_pd(actual.data(), x_x(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{givenOnBitNotSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y)};
        _mm_store_pd(actual.data(), x_y(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{givenOnBitNotSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::X)};
        _mm_store_pd(actual.data(), y_x(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{givenOnBitNotSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Y)};
        _mm_store_pd(actual.data(), y_y(givenOnBitNotSetPack, givenOnBitSetPack));
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }
    }
  };
}
