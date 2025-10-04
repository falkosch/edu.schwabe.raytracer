#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(Select128iTest) {
  public:
    TEST_METHOD(selectsOneOfTwoPackedInts_128WithBoolConditionMask) {
      const auto givenTruthy = _mm_set1_epi32(1);
      const auto givenFalsy = _mm_set1_epi32(2);

      {
        const auto actual = select(true, givenTruthy, givenFalsy);
        const auto matchExpected = _mm_cmpeq_epi32(givenTruthy, actual);
        Assert::AreEqual(
            1, _mm_testc_si128(matchExpected, MaskAll<PackedInts_128>()), L"select value mismatch", LINE_INFO()
        );
      }

      {
        const auto actual = select(false, givenTruthy, givenFalsy);
        const auto matchExpected = _mm_cmpeq_epi32(givenFalsy, actual);
        Assert::AreEqual(
            1, _mm_testc_si128(matchExpected, MaskAll<PackedInts_128>()), L"select value mismatch", LINE_INFO()
        );
      }
    }
  };
}
