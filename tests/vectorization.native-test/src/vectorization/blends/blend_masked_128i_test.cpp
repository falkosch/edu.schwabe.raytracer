#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(BlendMasked128iTest) {
  public:
    TEST_METHOD(variadicBlendsTwoBool4_32WithBool4_32BitMask) {
      auto givenOnBitNotSet = _mm_set1_epi32(1);
      auto givenOnBitSet = _mm_set1_epi32(2);

      {
        auto expected = _mm_setr_epi32(1, 1, 1, 1);
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, _mm_castps_si128(MaskNone<PackedFloat4_128>()));
        auto matchExpected = _mm_cmpeq_epi32(expected, actual);
        Assert::AreEqual(
            1, _mm_testc_si128(matchExpected, MaskAll<PackedInts_128>()), L"blend masked value mismatch", LINE_INFO()
        );
      }

      {
        auto expected = _mm_setr_epi32(2, 1, 1, 1);
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, _mm_castps_si128(MaskX<PackedFloat4_128>()));
        auto matchExpected = _mm_cmpeq_epi32(expected, actual);
        Assert::AreEqual(
            1, _mm_testc_si128(matchExpected, MaskAll<PackedInts_128>()), L"blend masked value mismatch", LINE_INFO()
        );
      }

      {
        auto expected = _mm_setr_epi32(2, 2, 1, 1);
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, _mm_castps_si128(MaskXY<PackedFloat4_128>()));
        auto matchExpected = _mm_cmpeq_epi32(expected, actual);
        Assert::AreEqual(
            1, _mm_testc_si128(matchExpected, MaskAll<PackedInts_128>()), L"blend masked value mismatch", LINE_INFO()
        );
      }

      {
        auto expected = _mm_setr_epi32(2, 2, 2, 1);
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, _mm_castps_si128(MaskXYZ<PackedFloat4_128>()));
        auto matchExpected = _mm_cmpeq_epi32(expected, actual);
        Assert::AreEqual(
            1, _mm_testc_si128(matchExpected, MaskAll<PackedInts_128>()), L"blend masked value mismatch", LINE_INFO()
        );
      }

      {
        auto expected = _mm_setr_epi32(2, 2, 2, 2);
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, _mm_castps_si128(MaskXYZW<PackedFloat4_128>()));
        auto matchExpected = _mm_cmpeq_epi32(expected, actual);
        Assert::AreEqual(
            1, _mm_testc_si128(matchExpected, MaskAll<PackedInts_128>()), L"blend masked value mismatch", LINE_INFO()
        );
      }
    }

    TEST_METHOD(variadicBlendsTwoBool_128WithBool_128BitMask) {
      const auto givenOnBitNotSet = MaskNone<PackedInts_128>();
      const auto givenOnBitSet = MaskAll<PackedInts_128>();

      {
        const auto expected = givenOnBitNotSet;
        const auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskNone<PackedInts_128>());
        const auto matchExpected = _mm_cmpeq_epi32(expected, actual);
        Assert::AreEqual(
            1, _mm_testc_si128(matchExpected, MaskAll<PackedInts_128>()), L"blend masked value mismatch", LINE_INFO()
        );
      }

      {
        const auto expected = givenOnBitSet;
        const auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskAll<PackedInts_128>());
        const auto matchExpected = _mm_cmpeq_epi32(expected, actual);
        Assert::AreEqual(
            1, _mm_testc_si128(matchExpected, MaskAll<PackedInts_128>()), L"blend masked value mismatch", LINE_INFO()
        );
      }
    }
  };
}
