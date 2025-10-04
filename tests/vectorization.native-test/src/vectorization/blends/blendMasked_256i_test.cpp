#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(BlendMasked256iTest) {
  public:
    TEST_METHOD(variadicBlendsTwoBool8_32WithBool8_32BitMask) {
      auto givenOnBitNotSet = _mm256_set1_epi32(1);
      auto givenOnBitSet = _mm256_set1_epi32(2);

      {
        auto expected = _mm256_setr_epi32(1, 1, 1, 1, 1, 1, 1, 1);
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, _mm256_castps_si256(MaskNone<PackedFloat8_256>()));
        auto matchExpected = _mm256_cmpeq_epi32(expected, actual);
        Assert::AreEqual(
            1, _mm256_testc_si256(matchExpected, MaskAll<PackedInts_256>()), L"blend masked value mismatch", LINE_INFO()
        );
      }

      {
        auto expected = _mm256_setr_epi32(2, 1, 1, 1, 2, 1, 1, 1);
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, _mm256_castps_si256(MaskX<PackedFloat8_256>()));
        auto matchExpected = _mm256_cmpeq_epi32(expected, actual);
        Assert::AreEqual(
            1, _mm256_testc_si256(matchExpected, MaskAll<PackedInts_256>()), L"blend masked value mismatch", LINE_INFO()
        );
      }

      {
        auto expected = _mm256_setr_epi32(2, 2, 1, 1, 2, 2, 1, 1);
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, _mm256_castps_si256(MaskXY<PackedFloat8_256>()));
        auto matchExpected = _mm256_cmpeq_epi32(expected, actual);
        Assert::AreEqual(
            1, _mm256_testc_si256(matchExpected, MaskAll<PackedInts_256>()), L"blend masked value mismatch", LINE_INFO()
        );
      }

      {
        auto expected = _mm256_setr_epi32(2, 2, 2, 1, 2, 2, 2, 1);
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, _mm256_castps_si256(MaskXYZ<PackedFloat8_256>()));
        auto matchExpected = _mm256_cmpeq_epi32(expected, actual);
        Assert::AreEqual(
            1, _mm256_testc_si256(matchExpected, MaskAll<PackedInts_256>()), L"blend masked value mismatch", LINE_INFO()
        );
      }

      {
        auto expected = _mm256_setr_epi32(2, 2, 2, 2, 2, 2, 2, 2);
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, _mm256_castps_si256(MaskXYZW<PackedFloat8_256>()));
        auto matchExpected = _mm256_cmpeq_epi32(expected, actual);
        Assert::AreEqual(
            1, _mm256_testc_si256(matchExpected, MaskAll<PackedInts_256>()), L"blend masked value mismatch", LINE_INFO()
        );
      }
    }

    TEST_METHOD(variadicBlendsTwoBool_256WithBool_256BitMask) {
      auto givenOnBitNotSet = MaskNone<PackedInts_256>();
      auto givenOnBitSet = MaskAll<PackedInts_256>();

      {
        auto expected = givenOnBitNotSet;
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskNone<PackedInts_256>());
        auto matchExpected = _mm256_cmpeq_epi32(expected, actual);
        Assert::AreEqual(
            1, _mm256_testc_si256(matchExpected, MaskAll<PackedInts_256>()), L"blend masked value mismatch", LINE_INFO()
        );
      }

      {
        auto expected = givenOnBitSet;
        auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskAll<PackedInts_256>());
        auto matchExpected = _mm256_cmpeq_epi32(expected, actual);
        Assert::AreEqual(
            1, _mm256_testc_si256(matchExpected, MaskAll<PackedInts_256>()), L"blend masked value mismatch", LINE_INFO()
        );
      }
    }
  };
}
