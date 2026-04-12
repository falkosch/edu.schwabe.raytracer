#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_ui32_4_BlendsTest) {
  public:
    TEST_METHOD(genericallyBlendsIdentities) {
      const v_ui32_4 givenOnBitNotSet{1u, 2u, 3u, 4u};
      const v_ui32_4 givenOnBitSet{5u, 6u, 7u, 8u};

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
      const v_ui32_4 givenOnBitNotSet{1u, 2u, 3u, 4u};
      const v_ui32_4 givenOnBitSet{5u, 6u, 7u, 8u};

      {
        const v_ui32_4 expected{x(givenOnBitSet), y(givenOnBitNotSet), z(givenOnBitNotSet), w(givenOnBitNotSet)};
        const auto actual = blend<true, false, false, false>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
      }

      {
        const v_ui32_4 expected{x(givenOnBitNotSet), y(givenOnBitSet), z(givenOnBitNotSet), w(givenOnBitNotSet)};
        const auto actual = blend<false, true, false, false>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
      }

      {
        const v_ui32_4 expected{x(givenOnBitNotSet), y(givenOnBitNotSet), z(givenOnBitSet), w(givenOnBitNotSet)};
        const auto actual = blend<false, false, true, false>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
      }

      {
        const v_ui32_4 expected{x(givenOnBitNotSet), y(givenOnBitNotSet), z(givenOnBitNotSet), w(givenOnBitSet)};
        const auto actual = blend<false, false, false, true>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallyBlendsPairs) {
      const v_ui32_4 givenOnBitNotSet{1u, 2u, 3u, 4u};
      const v_ui32_4 givenOnBitSet{5u, 6u, 7u, 8u};

      {
        const v_ui32_4 expected{x(givenOnBitSet), y(givenOnBitSet), z(givenOnBitNotSet), w(givenOnBitNotSet)};
        const auto actual = blend<true, true, false, false>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
      }

      {
        const v_ui32_4 expected{x(givenOnBitNotSet), y(givenOnBitNotSet), z(givenOnBitSet), w(givenOnBitSet)};
        const auto actual = blend<false, false, true, true>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
      }
    }
  };
}
