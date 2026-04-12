#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_i32_4_BlendsTest) {
  public:
    TEST_METHOD(genericallyBlendsIdentities) {
      const v_i32_4 givenOnBitNotSet{1, 2, 3, 4};
      const v_i32_4 givenOnBitSet{5, 6, 7, 8};

      {
        const auto actual = blend<false, false, false, false>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(givenOnBitNotSet == actual), L"blend value mismatch", LINE_INFO());
      }

      {
        const auto actual = blend<true, true, true, true>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(givenOnBitSet == actual), L"blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallyBlendsIndividuals) {
      const v_i32_4 givenOnBitNotSet{1, 2, 3, 4};
      const v_i32_4 givenOnBitSet{5, 6, 7, 8};

      {
        const v_i32_4 expected{x(givenOnBitSet), y(givenOnBitNotSet), z(givenOnBitNotSet), w(givenOnBitNotSet)};
        const auto actual = blend<true, false, false, false>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
      }

      {
        const v_i32_4 expected{x(givenOnBitNotSet), y(givenOnBitSet), z(givenOnBitNotSet), w(givenOnBitNotSet)};
        const auto actual = blend<false, true, false, false>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
      }

      {
        const v_i32_4 expected{x(givenOnBitNotSet), y(givenOnBitNotSet), z(givenOnBitSet), w(givenOnBitNotSet)};
        const auto actual = blend<false, false, true, false>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
      }

      {
        const v_i32_4 expected{x(givenOnBitNotSet), y(givenOnBitNotSet), z(givenOnBitNotSet), w(givenOnBitSet)};
        const auto actual = blend<false, false, false, true>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallyBlendsPairs) {
      const v_i32_4 givenOnBitNotSet{1, 2, 3, 4};
      const v_i32_4 givenOnBitSet{5, 6, 7, 8};

      {
        const v_i32_4 expected{x(givenOnBitSet), y(givenOnBitSet), z(givenOnBitNotSet), w(givenOnBitNotSet)};
        const auto actual = blend<true, true, false, false>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
      }

      {
        const v_i32_4 expected{x(givenOnBitNotSet), y(givenOnBitNotSet), z(givenOnBitSet), w(givenOnBitSet)};
        const auto actual = blend<false, false, true, true>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(blendsMaskedWithMasks) {
      const v_i32_4 givenOnBitNotSet{1, 2, 3, 4};
      const v_i32_4 givenOnBitSet{5, 6, 7, 8};

      {
        const auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, Zero<v_i32_4>());
        Assert::IsTrue(allTrue(givenOnBitNotSet == actual), L"blend masked value mismatch", LINE_INFO());
      }

      {
        const v_i32_4 expected{x(givenOnBitSet), y(givenOnBitNotSet), z(givenOnBitNotSet), w(givenOnBitNotSet)};
        const auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskX<v_i32_4>());
        Assert::IsTrue(allTrue(expected == actual), L"blend masked value mismatch", LINE_INFO());
      }

      {
        const v_i32_4 expected{x(givenOnBitSet), y(givenOnBitSet), z(givenOnBitNotSet), w(givenOnBitNotSet)};
        const auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskXY<v_i32_4>());
        Assert::IsTrue(allTrue(expected == actual), L"blend masked value mismatch", LINE_INFO());
      }

      {
        const v_i32_4 expected{x(givenOnBitSet), y(givenOnBitSet), z(givenOnBitSet), w(givenOnBitNotSet)};
        const auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskXYZ<v_i32_4>());
        Assert::IsTrue(allTrue(expected == actual), L"blend masked value mismatch", LINE_INFO());
      }

      {
        const auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskAll<v_i32_4>());
        Assert::IsTrue(allTrue(givenOnBitSet == actual), L"blend masked value mismatch", LINE_INFO());
      }
    }
  };
}
