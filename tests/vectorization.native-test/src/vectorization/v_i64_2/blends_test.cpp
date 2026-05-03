#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_i64_2_BlendsTest) {
  public:
    TEST_METHOD(genericallyBlendsIdentities) {
      const v_i64_2 givenOnBitNotSet{Int_64{10LL}, Int_64{-5LL}};
      const v_i64_2 givenOnBitSet{Int_64{30LL}, Int_64{-15LL}};

      {
        const auto actual = blend<false, false>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(givenOnBitNotSet == actual), L"blend value mismatch", LINE_INFO());
      }

      {
        const auto actual = blend<true, true>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(givenOnBitSet == actual), L"blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallyBlendsIndividuals) {
      const v_i64_2 givenOnBitNotSet{Int_64{10LL}, Int_64{-5LL}};
      const v_i64_2 givenOnBitSet{Int_64{30LL}, Int_64{-15LL}};

      {
        const v_i64_2 expected{x(givenOnBitSet), y(givenOnBitNotSet)};
        const auto actual = blend<true, false>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
      }

      {
        const v_i64_2 expected{x(givenOnBitNotSet), y(givenOnBitSet)};
        const auto actual = blend<false, true>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(blendsMaskedWithMasks) {
      const v_i64_2 givenOnBitNotSet{Int_64{10LL}, Int_64{-5LL}};
      const v_i64_2 givenOnBitSet{Int_64{30LL}, Int_64{-15LL}};

      {
        const auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, Zero<v_i64_2>());
        Assert::IsTrue(allTrue(givenOnBitNotSet == actual), L"blend masked value mismatch", LINE_INFO());
      }

      {
        const v_i64_2 expected{x(givenOnBitSet), y(givenOnBitNotSet)};
        const auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskX<v_i64_2>());
        Assert::IsTrue(allTrue(expected == actual), L"blend masked value mismatch", LINE_INFO());
      }

      {
        const auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskAll<v_i64_2>());
        Assert::IsTrue(allTrue(givenOnBitSet == actual), L"blend masked value mismatch", LINE_INFO());
      }
    }
  };
}
