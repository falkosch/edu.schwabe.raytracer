#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(SwizzledBlends256sTest) {
  public:
    TEST_METHOD(genericallySwizzleBlendsIdentity) {
      std::array<Float_32, VectorSizes::X8> givenOnBitNotSet{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f};
      auto givenOnBitNotSetPack = _mm256_load_ps(givenOnBitNotSet.data());
      std::array<Float_32, VectorSizes::X8> givenOnBitSet{9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f};
      auto givenOnBitSetPack = _mm256_load_ps(givenOnBitSet.data());
      std::array<Float_32, VectorSizes::X8> actual{};

      {
        std::array<Float_32, VectorSizes::X8> expected{
            givenOnBitNotSet.at(VectorIndices::X1), givenOnBitNotSet.at(VectorIndices::X2),
            givenOnBitNotSet.at(VectorIndices::X3), givenOnBitNotSet.at(VectorIndices::X4),
            givenOnBitNotSet.at(VectorIndices::X5), givenOnBitNotSet.at(VectorIndices::X6),
            givenOnBitNotSet.at(VectorIndices::X7), givenOnBitNotSet.at(VectorIndices::X8)
        };
        _mm256_store_ps(
            actual.data(), swizzledBlend<
                               VectorIndices::X1, VectorIndices::X2, VectorIndices::X3, VectorIndices::X4,
                               VectorIndices::X5, VectorIndices::X6, VectorIndices::X7, VectorIndices::X8, false, false,
                               false, false, false, false, false, false>(givenOnBitNotSetPack, givenOnBitSetPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::X8> expected{
            givenOnBitSet.at(VectorIndices::X1), givenOnBitSet.at(VectorIndices::X2),
            givenOnBitSet.at(VectorIndices::X3), givenOnBitSet.at(VectorIndices::X4),
            givenOnBitSet.at(VectorIndices::X5), givenOnBitSet.at(VectorIndices::X6),
            givenOnBitSet.at(VectorIndices::X7), givenOnBitSet.at(VectorIndices::X8)
        };
        _mm256_store_ps(
            actual.data(), swizzledBlend<
                               VectorIndices::X1, VectorIndices::X2, VectorIndices::X3, VectorIndices::X4,
                               VectorIndices::X5, VectorIndices::X6, VectorIndices::X7, VectorIndices::X8, true, true,
                               true, true, true, true, true, true>(givenOnBitNotSetPack, givenOnBitSetPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallySwizzleBlendsBroadcast) {
      std::array<Float_32, VectorSizes::X8> givenOnBitNotSet{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f};
      auto givenOnBitNotSetPack = _mm256_load_ps(givenOnBitNotSet.data());
      std::array<Float_32, VectorSizes::X8> givenOnBitSet{9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f};
      auto givenOnBitSetPack = _mm256_load_ps(givenOnBitSet.data());
      std::array<Float_32, VectorSizes::X8> actual{};

      {
        std::array<Float_32, VectorSizes::X8> expected{
            givenOnBitNotSet.at(VectorIndices::X1), givenOnBitNotSet.at(VectorIndices::X1),
            givenOnBitNotSet.at(VectorIndices::X1), givenOnBitNotSet.at(VectorIndices::X1),
            givenOnBitNotSet.at(VectorIndices::X1), givenOnBitNotSet.at(VectorIndices::X1),
            givenOnBitNotSet.at(VectorIndices::X1), givenOnBitNotSet.at(VectorIndices::X1)
        };
        _mm256_store_ps(
            actual.data(), swizzledBlend<
                               VectorIndices::X1, VectorIndices::X1, VectorIndices::X1, VectorIndices::X1,
                               VectorIndices::X1, VectorIndices::X1, VectorIndices::X1, VectorIndices::X1, false, false,
                               false, false, false, false, false, false>(givenOnBitNotSetPack, givenOnBitSetPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::X8> expected{
            givenOnBitSet.at(VectorIndices::X2), givenOnBitSet.at(VectorIndices::X2),
            givenOnBitSet.at(VectorIndices::X2), givenOnBitSet.at(VectorIndices::X2),
            givenOnBitSet.at(VectorIndices::X3), givenOnBitSet.at(VectorIndices::X3),
            givenOnBitSet.at(VectorIndices::X3), givenOnBitSet.at(VectorIndices::X3)
        };
        _mm256_store_ps(
            actual.data(), swizzledBlend<
                               VectorIndices::X2, VectorIndices::X2, VectorIndices::X2, VectorIndices::X2,
                               VectorIndices::X3, VectorIndices::X3, VectorIndices::X3, VectorIndices::X3, true, true,
                               true, true, true, true, true, true>(givenOnBitNotSetPack, givenOnBitSetPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::X8> expected{
            givenOnBitNotSet.at(VectorIndices::X5), givenOnBitNotSet.at(VectorIndices::X5),
            givenOnBitNotSet.at(VectorIndices::X5), givenOnBitNotSet.at(VectorIndices::X5),
            givenOnBitSet.at(VectorIndices::X4),    givenOnBitSet.at(VectorIndices::X4),
            givenOnBitSet.at(VectorIndices::X4),    givenOnBitSet.at(VectorIndices::X4)
        };
        _mm256_store_ps(
            actual.data(), swizzledBlend<
                               VectorIndices::X5, VectorIndices::X5, VectorIndices::X5, VectorIndices::X5,
                               VectorIndices::X4, VectorIndices::X4, VectorIndices::X4, VectorIndices::X4, false, false,
                               false, false, true, true, true, true>(givenOnBitNotSetPack, givenOnBitSetPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::X8> expected{
            givenOnBitSet.at(VectorIndices::X6),    givenOnBitSet.at(VectorIndices::X6),
            givenOnBitSet.at(VectorIndices::X6),    givenOnBitSet.at(VectorIndices::X6),
            givenOnBitNotSet.at(VectorIndices::X7), givenOnBitNotSet.at(VectorIndices::X7),
            givenOnBitNotSet.at(VectorIndices::X7), givenOnBitNotSet.at(VectorIndices::X7)
        };
        _mm256_store_ps(
            actual.data(), swizzledBlend<
                               VectorIndices::X6, VectorIndices::X6, VectorIndices::X6, VectorIndices::X6,
                               VectorIndices::X7, VectorIndices::X7, VectorIndices::X7, VectorIndices::X7, true, true,
                               true, true, false, false, false, false>(givenOnBitNotSetPack, givenOnBitSetPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallySwizzleBlendsAny) {
      std::array<Float_32, VectorSizes::X8> givenOnBitNotSet{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f};
      auto givenOnBitNotSetPack = _mm256_load_ps(givenOnBitNotSet.data());
      std::array<Float_32, VectorSizes::X8> givenOnBitSet{9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f};
      auto givenOnBitSetPack = _mm256_load_ps(givenOnBitSet.data());
      std::array<Float_32, VectorSizes::X8> actual{};

      {
        std::array<Float_32, VectorSizes::X8> expected{
            givenOnBitSet.at(VectorIndices::X6),    givenOnBitNotSet.at(VectorIndices::X6),
            givenOnBitSet.at(VectorIndices::X6),    givenOnBitNotSet.at(VectorIndices::X6),
            givenOnBitNotSet.at(VectorIndices::X7), givenOnBitSet.at(VectorIndices::X7),
            givenOnBitNotSet.at(VectorIndices::X7), givenOnBitSet.at(VectorIndices::X7)
        };
        _mm256_store_ps(
            actual.data(), swizzledBlend<
                               VectorIndices::X6, VectorIndices::X6, VectorIndices::X6, VectorIndices::X6,
                               VectorIndices::X7, VectorIndices::X7, VectorIndices::X7, VectorIndices::X7, true, false,
                               true, false, false, true, false, true>(givenOnBitNotSetPack, givenOnBitSetPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::X8> expected{
            givenOnBitNotSet.at(VectorIndices::X8), givenOnBitNotSet.at(VectorIndices::X7),
            givenOnBitSet.at(VectorIndices::X6),    givenOnBitSet.at(VectorIndices::X5),
            givenOnBitSet.at(VectorIndices::X8),    givenOnBitNotSet.at(VectorIndices::X6),
            givenOnBitNotSet.at(VectorIndices::X4), givenOnBitNotSet.at(VectorIndices::X2)
        };
        _mm256_store_ps(
            actual.data(), swizzledBlend<
                               VectorIndices::X8, VectorIndices::X7, VectorIndices::X6, VectorIndices::X5,
                               VectorIndices::X8, VectorIndices::X6, VectorIndices::X4, VectorIndices::X2, false, false,
                               true, true, true, false, false, false>(givenOnBitNotSetPack, givenOnBitSetPack)
        );
        Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
      }
    }
  };
}
