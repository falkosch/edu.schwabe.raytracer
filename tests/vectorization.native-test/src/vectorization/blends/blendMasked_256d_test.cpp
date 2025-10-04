#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(BlendMasked256dTest) {
  public:
    TEST_METHOD(variadicBlendsTwoFloat4_64WithFloat4_64BitMask) {
      std::array<Float_64, VectorSizes::W> givenOnBitNotSet{1.0, 2.0, 3.0, 4.0};
      auto givenOnBitNotSetPack = _mm256_load_pd(givenOnBitNotSet.data());
      std::array<Float_64, VectorSizes::W> givenOnBitSet{5.0, 6.0f, 7.0, 8.0};
      auto givenOnBitSetPack = _mm256_load_pd(givenOnBitSet.data());
      std::array<Float_64, VectorSizes::W> actual{};

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm256_store_pd(
            actual.data(), blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, MaskNone<PackedFloat4_256>())
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm256_store_pd(actual.data(), blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, MaskX<PackedFloat4_256>()));
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm256_store_pd(
            actual.data(), blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, MaskXY<PackedFloat4_256>())
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z),
            givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm256_store_pd(
            actual.data(), blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, MaskXYZ<PackedFloat4_256>())
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z),
            givenOnBitSet.at(VectorIndices::W)
        };
        _mm256_store_pd(
            actual.data(), blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, MaskXYZW<PackedFloat4_256>())
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(variadicBlendsTwoFloat4_64WithBool4_64BitMask) {
      std::array<Float_64, VectorSizes::W> givenOnBitNotSet{1.0, 2.0, 3.0, 4.0};
      auto givenOnBitNotSetPack = _mm256_load_pd(givenOnBitNotSet.data());
      std::array<Float_64, VectorSizes::W> givenOnBitSet{5.0, 6.0f, 7.0, 8.0};
      auto givenOnBitSetPack = _mm256_load_pd(givenOnBitSet.data());
      std::array<Float_64, VectorSizes::W> actual{};

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm256_store_pd(
            actual.data(),
            blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, _mm256_castpd_si256(MaskNone<PackedFloat4_256>()))
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm256_store_pd(
            actual.data(),
            blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, _mm256_castpd_si256(MaskX<PackedFloat4_256>()))
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y),
            givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm256_store_pd(
            actual.data(),
            blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, _mm256_castpd_si256(MaskXY<PackedFloat4_256>()))
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z),
            givenOnBitNotSet.at(VectorIndices::W)
        };
        _mm256_store_pd(
            actual.data(),
            blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, _mm256_castpd_si256(MaskXYZ<PackedFloat4_256>()))
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }

      {
        std::array<Float_64, VectorSizes::W> expected{
            givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z),
            givenOnBitSet.at(VectorIndices::W)
        };
        _mm256_store_pd(
            actual.data(),
            blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, _mm256_castpd_si256(MaskXYZW<PackedFloat4_256>()))
        );
        Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
      }
    }
  };
}
