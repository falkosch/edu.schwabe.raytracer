#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(Select256iTest) {
  public:
    TEST_METHOD(selectsOneOfTwoPackedInts_128WithBoolConditionMask) {
      auto givenTruthy = _mm256_set1_epi32(1);
      auto givenFalsy = _mm256_set1_epi32(2);

      {
        auto actual = select(true, givenTruthy, givenFalsy);
        auto matchExpected = _mm256_cmpeq_epi32(givenTruthy, actual);
        Assert::AreEqual(
            1, _mm256_testc_si256(matchExpected, MaskAll<PackedInts_256>()), L"select value mismatch", LINE_INFO()
        );
      }

      {
        auto actual = select(false, givenTruthy, givenFalsy);
        auto matchExpected = _mm256_cmpeq_epi32(givenFalsy, actual);
        Assert::AreEqual(
            1, _mm256_testc_si256(matchExpected, MaskAll<PackedInts_256>()), L"select value mismatch", LINE_INFO()
        );
      }
    }
  };
}
