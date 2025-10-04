#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(BlendMasked128sTest) {
  public:
    TEST_METHOD(variadicBlendsTwoFloat4_32WithFloat4_32BitMask) {
      constexpr std::array givenOnBitNotSet{1.0f, 2.0f, 3.0f, 4.0f};
      const auto givenOnBitNotSetPack = _mm_load_ps(givenOnBitNotSet.data());
      constexpr std::array givenOnBitSet{5.0f, 6.0f, 7.0f, 8.0f};
      const auto givenOnBitSetPack = _mm_load_ps(givenOnBitSet.data());
      std::array<Float_32, VectorSizes::W> actual{};

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, MaskNone<PackedFloat4_128>()));
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, MaskX<PackedFloat4_128>()));
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, MaskXY<PackedFloat4_128>()));
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z),
            givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, MaskXYZ<PackedFloat4_128>()));
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z),
            givenOnBitSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, MaskXYZW<PackedFloat4_128>()));
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(variadicBlendsTwoFloat4_32WithBool4_32BitMask) {
      constexpr std::array givenOnBitNotSet{1.0f, 2.0f, 3.0f, 4.0f};
      const auto givenOnBitNotSetPack = _mm_load_ps(givenOnBitNotSet.data());
      constexpr std::array givenOnBitSet{5.0f, 6.0f, 7.0f, 8.0f};
      const auto givenOnBitSetPack = _mm_load_ps(givenOnBitSet.data());
      std::array<Float_32, VectorSizes::W> actual{};

      {
        constexpr std::array expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm_store_ps(
            actual.data(),
            blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, _mm_castps_si128(MaskNone<PackedFloat4_128>()))
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm_store_ps(
            actual.data(),
            blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, _mm_castps_si128(MaskX<PackedFloat4_128>()))
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm_store_ps(
            actual.data(),
            blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, _mm_castps_si128(MaskXY<PackedFloat4_128>()))
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z),
            givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm_store_ps(
            actual.data(),
            blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, _mm_castps_si128(MaskXYZ<PackedFloat4_128>()))
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        constexpr std::array expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z),
            givenOnBitSet.at(VectorIndices::W)
        };
        _mm_store_ps(actual.data(), blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, MaskXYZW<PackedFloat4_128>()));
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }
    }
  };
}
