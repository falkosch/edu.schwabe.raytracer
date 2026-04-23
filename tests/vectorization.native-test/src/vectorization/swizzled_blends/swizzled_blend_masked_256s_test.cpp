#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(SwizzledBlendMasked256sTest) {
  public:
    TEST_METHOD(variadicSwizzledBlendsIdentityOfTwoFloat8_32WithFloat8_32BitMask) {
      constexpr std::array givenOnBitNotSet{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f};
      const auto givenOnBitNotSetPack = _mm256_load_ps(givenOnBitNotSet.data());
      constexpr std::array givenOnBitSet{9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f};
      const auto givenOnBitSetPack = _mm256_load_ps(givenOnBitSet.data());
      std::array<Float_32, VectorSizes::X8> actual{};

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::X1), givenOnBitNotSet.at(VectorIndices::X2),
            givenOnBitNotSet.at(VectorIndices::X3), givenOnBitNotSet.at(VectorIndices::X4),
            givenOnBitNotSet.at(VectorIndices::X5), givenOnBitNotSet.at(VectorIndices::X6),
            givenOnBitNotSet.at(VectorIndices::X7), givenOnBitNotSet.at(VectorIndices::X8)
        };
        _mm256_store_ps(
            actual.data(),
            swizzledBlendMasked<
                VectorIndices::X1, VectorIndices::X2, VectorIndices::X3, VectorIndices::X4, VectorIndices::X5,
                VectorIndices::X6, VectorIndices::X7, VectorIndices::X8>(
                givenOnBitNotSetPack, givenOnBitSetPack, MaskNone<PackedFloat8_256>()
            )
        );
        Assert::AreEqual(expected, actual, L"swizzled blend masked value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitSet.at(VectorIndices::X1), givenOnBitSet.at(VectorIndices::X2),
            givenOnBitSet.at(VectorIndices::X3), givenOnBitSet.at(VectorIndices::X4),
            givenOnBitSet.at(VectorIndices::X5), givenOnBitSet.at(VectorIndices::X6),
            givenOnBitSet.at(VectorIndices::X7), givenOnBitSet.at(VectorIndices::X8)
        };
        _mm256_store_ps(
            actual.data(),
            swizzledBlendMasked<
                VectorIndices::X1, VectorIndices::X2, VectorIndices::X3, VectorIndices::X4, VectorIndices::X5,
                VectorIndices::X6, VectorIndices::X7, VectorIndices::X8>(
                givenOnBitNotSetPack, givenOnBitSetPack, MaskXYZW<PackedFloat8_256>()
            )
        );
        Assert::AreEqual(expected, actual, L"swizzled blend masked value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitSet.at(VectorIndices::X1),    givenOnBitSet.at(VectorIndices::X2),
            givenOnBitNotSet.at(VectorIndices::X3), givenOnBitNotSet.at(VectorIndices::X4),
            givenOnBitSet.at(VectorIndices::X5),    givenOnBitSet.at(VectorIndices::X6),
            givenOnBitNotSet.at(VectorIndices::X7), givenOnBitNotSet.at(VectorIndices::X8)
        };
        _mm256_store_ps(
            actual.data(),
            swizzledBlendMasked<
                VectorIndices::X1, VectorIndices::X2, VectorIndices::X3, VectorIndices::X4, VectorIndices::X5,
                VectorIndices::X6, VectorIndices::X7, VectorIndices::X8>(
                givenOnBitNotSetPack, givenOnBitSetPack, MaskXY<PackedFloat8_256>()
            )
        );
        Assert::AreEqual(expected, actual, L"swizzled blend masked value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(variadicSwizzledBlendsSwappedLanesOfTwoFloat8_32WithFloat8_32BitMask) {
      constexpr std::array givenOnBitNotSet{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f};
      const auto givenOnBitNotSetPack = _mm256_load_ps(givenOnBitNotSet.data());
      constexpr std::array givenOnBitSet{9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f};
      const auto givenOnBitSetPack = _mm256_load_ps(givenOnBitSet.data());
      std::array<Float_32, VectorSizes::X8> actual{};

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::X5), givenOnBitNotSet.at(VectorIndices::X6),
            givenOnBitNotSet.at(VectorIndices::X7), givenOnBitNotSet.at(VectorIndices::X8),
            givenOnBitNotSet.at(VectorIndices::X1), givenOnBitNotSet.at(VectorIndices::X2),
            givenOnBitNotSet.at(VectorIndices::X3), givenOnBitNotSet.at(VectorIndices::X4)
        };
        _mm256_store_ps(
            actual.data(),
            swizzledBlendMasked<
                VectorIndices::X5, VectorIndices::X6, VectorIndices::X7, VectorIndices::X8, VectorIndices::X1,
                VectorIndices::X2, VectorIndices::X3, VectorIndices::X4>(
                givenOnBitNotSetPack, givenOnBitSetPack, MaskNone<PackedFloat8_256>()
            )
        );
        Assert::AreEqual(expected, actual, L"swizzled blend masked value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitSet.at(VectorIndices::X5), givenOnBitSet.at(VectorIndices::X6),
            givenOnBitSet.at(VectorIndices::X7), givenOnBitSet.at(VectorIndices::X8),
            givenOnBitSet.at(VectorIndices::X1), givenOnBitSet.at(VectorIndices::X2),
            givenOnBitSet.at(VectorIndices::X3), givenOnBitSet.at(VectorIndices::X4)
        };
        _mm256_store_ps(
            actual.data(),
            swizzledBlendMasked<
                VectorIndices::X5, VectorIndices::X6, VectorIndices::X7, VectorIndices::X8, VectorIndices::X1,
                VectorIndices::X2, VectorIndices::X3, VectorIndices::X4>(
                givenOnBitNotSetPack, givenOnBitSetPack, MaskXYZW<PackedFloat8_256>()
            )
        );
        Assert::AreEqual(expected, actual, L"swizzled blend masked value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitSet.at(VectorIndices::X5),    givenOnBitNotSet.at(VectorIndices::X6),
            givenOnBitNotSet.at(VectorIndices::X7), givenOnBitNotSet.at(VectorIndices::X8),
            givenOnBitSet.at(VectorIndices::X1),    givenOnBitNotSet.at(VectorIndices::X2),
            givenOnBitNotSet.at(VectorIndices::X3), givenOnBitNotSet.at(VectorIndices::X4)
        };
        _mm256_store_ps(
            actual.data(),
            swizzledBlendMasked<
                VectorIndices::X5, VectorIndices::X6, VectorIndices::X7, VectorIndices::X8, VectorIndices::X1,
                VectorIndices::X2, VectorIndices::X3, VectorIndices::X4>(
                givenOnBitNotSetPack, givenOnBitSetPack, MaskX<PackedFloat8_256>()
            )
        );
        Assert::AreEqual(expected, actual, L"swizzled blend masked value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(variadicSwizzledBlendsIdentityOfTwoFloat8_32WithBool8_32BitMask) {
      constexpr std::array givenOnBitNotSet{1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f};
      const auto givenOnBitNotSetPack = _mm256_load_ps(givenOnBitNotSet.data());
      constexpr std::array givenOnBitSet{9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f};
      const auto givenOnBitSetPack = _mm256_load_ps(givenOnBitSet.data());
      std::array<Float_32, VectorSizes::X8> actual{};

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::X1), givenOnBitNotSet.at(VectorIndices::X2),
            givenOnBitNotSet.at(VectorIndices::X3), givenOnBitNotSet.at(VectorIndices::X4),
            givenOnBitNotSet.at(VectorIndices::X5), givenOnBitNotSet.at(VectorIndices::X6),
            givenOnBitNotSet.at(VectorIndices::X7), givenOnBitNotSet.at(VectorIndices::X8)
        };
        _mm256_store_ps(
            actual.data(),
            swizzledBlendMasked<
                VectorIndices::X1, VectorIndices::X2, VectorIndices::X3, VectorIndices::X4, VectorIndices::X5,
                VectorIndices::X6, VectorIndices::X7, VectorIndices::X8>(
                givenOnBitNotSetPack, givenOnBitSetPack, _mm256_castps_si256(MaskNone<PackedFloat8_256>())
            )
        );
        Assert::AreEqual(expected, actual, L"swizzled blend masked value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitSet.at(VectorIndices::X1), givenOnBitSet.at(VectorIndices::X2),
            givenOnBitSet.at(VectorIndices::X3), givenOnBitSet.at(VectorIndices::X4),
            givenOnBitSet.at(VectorIndices::X5), givenOnBitSet.at(VectorIndices::X6),
            givenOnBitSet.at(VectorIndices::X7), givenOnBitSet.at(VectorIndices::X8)
        };
        _mm256_store_ps(
            actual.data(),
            swizzledBlendMasked<
                VectorIndices::X1, VectorIndices::X2, VectorIndices::X3, VectorIndices::X4, VectorIndices::X5,
                VectorIndices::X6, VectorIndices::X7, VectorIndices::X8>(
                givenOnBitNotSetPack, givenOnBitSetPack, _mm256_castps_si256(MaskXYZW<PackedFloat8_256>())
            )
        );
        Assert::AreEqual(expected, actual, L"swizzled blend masked value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitSet.at(VectorIndices::X1),    givenOnBitSet.at(VectorIndices::X2),
            givenOnBitNotSet.at(VectorIndices::X3), givenOnBitNotSet.at(VectorIndices::X4),
            givenOnBitSet.at(VectorIndices::X5),    givenOnBitSet.at(VectorIndices::X6),
            givenOnBitNotSet.at(VectorIndices::X7), givenOnBitNotSet.at(VectorIndices::X8)
        };
        _mm256_store_ps(
            actual.data(),
            swizzledBlendMasked<
                VectorIndices::X1, VectorIndices::X2, VectorIndices::X3, VectorIndices::X4, VectorIndices::X5,
                VectorIndices::X6, VectorIndices::X7, VectorIndices::X8>(
                givenOnBitNotSetPack, givenOnBitSetPack, _mm256_castps_si256(MaskXY<PackedFloat8_256>())
            )
        );
        Assert::AreEqual(expected, actual, L"swizzled blend masked value mismatch", LINE_INFO());
      }
    }
  };
}
