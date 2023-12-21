#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(BlendMasked256sTest) {
  public:
    TEST_METHOD(variadicBlendsTwoFloat8_32WithFloat8_32BitMask) {
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
            actual.data(), blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, MaskNone<PackedFloat8_256>())
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::X8> expected{
            givenOnBitSet.at(VectorIndices::X1),    givenOnBitNotSet.at(VectorIndices::X2),
            givenOnBitNotSet.at(VectorIndices::X3), givenOnBitNotSet.at(VectorIndices::X4),
            givenOnBitSet.at(VectorIndices::X5),    givenOnBitNotSet.at(VectorIndices::X6),
            givenOnBitNotSet.at(VectorIndices::X7), givenOnBitNotSet.at(VectorIndices::X8)
        };
        _mm256_store_ps(actual.data(), blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, MaskX<PackedFloat8_256>()));
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::X8> expected{
            givenOnBitSet.at(VectorIndices::X1),    givenOnBitSet.at(VectorIndices::X2),
            givenOnBitNotSet.at(VectorIndices::X3), givenOnBitNotSet.at(VectorIndices::X4),
            givenOnBitSet.at(VectorIndices::X5),    givenOnBitSet.at(VectorIndices::X6),
            givenOnBitNotSet.at(VectorIndices::X7), givenOnBitNotSet.at(VectorIndices::X8)
        };
        _mm256_store_ps(
            actual.data(), blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, MaskXY<PackedFloat8_256>())
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::X8> expected{
            givenOnBitSet.at(VectorIndices::X1), givenOnBitSet.at(VectorIndices::X2),
            givenOnBitSet.at(VectorIndices::X3), givenOnBitNotSet.at(VectorIndices::X4),
            givenOnBitSet.at(VectorIndices::X5), givenOnBitSet.at(VectorIndices::X6),
            givenOnBitSet.at(VectorIndices::X7), givenOnBitNotSet.at(VectorIndices::X8)
        };
        _mm256_store_ps(
            actual.data(), blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, MaskXYZ<PackedFloat8_256>())
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::X8> expected{
            givenOnBitSet.at(VectorIndices::X1), givenOnBitSet.at(VectorIndices::X2),
            givenOnBitSet.at(VectorIndices::X3), givenOnBitSet.at(VectorIndices::X4),
            givenOnBitSet.at(VectorIndices::X5), givenOnBitSet.at(VectorIndices::X6),
            givenOnBitSet.at(VectorIndices::X7), givenOnBitSet.at(VectorIndices::X8)
        };
        _mm256_store_ps(
            actual.data(), blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, MaskXYZW<PackedFloat8_256>())
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(variadicBlendsTwoFloat8_32WithBool8_32BitMask) {
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
            actual.data(),
            blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, _mm256_castps_si256(MaskNone<PackedFloat8_256>()))
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::X8> expected{
            givenOnBitSet.at(VectorIndices::X1),    givenOnBitNotSet.at(VectorIndices::X2),
            givenOnBitNotSet.at(VectorIndices::X3), givenOnBitNotSet.at(VectorIndices::X4),
            givenOnBitSet.at(VectorIndices::X5),    givenOnBitNotSet.at(VectorIndices::X6),
            givenOnBitNotSet.at(VectorIndices::X7), givenOnBitNotSet.at(VectorIndices::X8)
        };
        _mm256_store_ps(
            actual.data(),
            blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, _mm256_castps_si256(MaskX<PackedFloat8_256>()))
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::X8> expected{
            givenOnBitSet.at(VectorIndices::X1),    givenOnBitSet.at(VectorIndices::X2),
            givenOnBitNotSet.at(VectorIndices::X3), givenOnBitNotSet.at(VectorIndices::X4),
            givenOnBitSet.at(VectorIndices::X5),    givenOnBitSet.at(VectorIndices::X6),
            givenOnBitNotSet.at(VectorIndices::X7), givenOnBitNotSet.at(VectorIndices::X8)
        };
        _mm256_store_ps(
            actual.data(),
            blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, _mm256_castps_si256(MaskXY<PackedFloat8_256>()))
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::X8> expected{
            givenOnBitSet.at(VectorIndices::X1), givenOnBitSet.at(VectorIndices::X2),
            givenOnBitSet.at(VectorIndices::X3), givenOnBitNotSet.at(VectorIndices::X4),
            givenOnBitSet.at(VectorIndices::X5), givenOnBitSet.at(VectorIndices::X6),
            givenOnBitSet.at(VectorIndices::X7), givenOnBitNotSet.at(VectorIndices::X8)
        };
        _mm256_store_ps(
            actual.data(),
            blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, _mm256_castps_si256(MaskXYZ<PackedFloat8_256>()))
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        std::array<Float_32, VectorSizes::X8> expected{
            givenOnBitSet.at(VectorIndices::X1), givenOnBitSet.at(VectorIndices::X2),
            givenOnBitSet.at(VectorIndices::X3), givenOnBitSet.at(VectorIndices::X4),
            givenOnBitSet.at(VectorIndices::X5), givenOnBitSet.at(VectorIndices::X6),
            givenOnBitSet.at(VectorIndices::X7), givenOnBitSet.at(VectorIndices::X8)
        };
        _mm256_store_ps(
            actual.data(),
            blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, _mm256_castps_si256(MaskXYZW<PackedFloat8_256>()))
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }
    }
  };
}
