#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_i64_4_BlendsTest) {
  public:
    TEST_METHOD(genericallyBlendsIdentities) {
      const v_i64_4 givenOnBitNotSet{10LL, 20LL, 30LL, 40LL};
      const v_i64_4 givenOnBitSet{50LL, 60LL, 70LL, 80LL};

      {
        const auto actual = blend<false, false, false, false>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(givenOnBitNotSet == actual), L"blend all-false value mismatch", LINE_INFO());
      }

      {
        const auto actual = blend<true, true, true, true>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(givenOnBitSet == actual), L"blend all-true value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallyBlendsIndividuals) {
      const v_i64_4 givenOnBitNotSet{10LL, 20LL, 30LL, 40LL};
      const v_i64_4 givenOnBitSet{50LL, 60LL, 70LL, 80LL};

      {
        const v_i64_4 expected{x(givenOnBitSet), y(givenOnBitNotSet), z(givenOnBitNotSet), w(givenOnBitNotSet)};
        const auto actual = blend<true, false, false, false>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(expected == actual), L"blend<true,f,f,f> value mismatch", LINE_INFO());
      }

      {
        const v_i64_4 expected{x(givenOnBitNotSet), y(givenOnBitSet), z(givenOnBitNotSet), w(givenOnBitNotSet)};
        const auto actual = blend<false, true, false, false>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(expected == actual), L"blend<f,true,f,f> value mismatch", LINE_INFO());
      }

      {
        const v_i64_4 expected{x(givenOnBitNotSet), y(givenOnBitNotSet), z(givenOnBitSet), w(givenOnBitNotSet)};
        const auto actual = blend<false, false, true, false>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(expected == actual), L"blend<f,f,true,f> value mismatch", LINE_INFO());
      }

      {
        const v_i64_4 expected{x(givenOnBitNotSet), y(givenOnBitNotSet), z(givenOnBitNotSet), w(givenOnBitSet)};
        const auto actual = blend<false, false, false, true>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(expected == actual), L"blend<f,f,f,true> value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallyBlendsPairs) {
      const v_i64_4 givenOnBitNotSet{10LL, 20LL, 30LL, 40LL};
      const v_i64_4 givenOnBitSet{50LL, 60LL, 70LL, 80LL};

      {
        const v_i64_4 expected{x(givenOnBitSet), y(givenOnBitSet), z(givenOnBitNotSet), w(givenOnBitNotSet)};
        const auto actual = blend<true, true, false, false>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(expected == actual), L"blend<t,t,f,f> value mismatch", LINE_INFO());
      }

      {
        const v_i64_4 expected{x(givenOnBitNotSet), y(givenOnBitNotSet), z(givenOnBitSet), w(givenOnBitSet)};
        const auto actual = blend<false, false, true, true>(givenOnBitNotSet, givenOnBitSet);
        Assert::IsTrue(allTrue(expected == actual), L"blend<f,f,t,t> value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(blendsMaskedWithZeroMask) {
      const v_i64_4 givenOnBitNotSet{10LL, 20LL, 30LL, 40LL};
      const v_i64_4 givenOnBitSet{50LL, 60LL, 70LL, 80LL};

      const auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, Zero<v_i64_4>());
      Assert::IsTrue(allTrue(givenOnBitNotSet == actual), L"blendMasked zero mask value mismatch", LINE_INFO());
    }

    TEST_METHOD(blendsMaskedWithFullMask) {
      const v_i64_4 givenOnBitNotSet{10LL, 20LL, 30LL, 40LL};
      const v_i64_4 givenOnBitSet{50LL, 60LL, 70LL, 80LL};

      const auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, NegativeOne<v_i64_4>());
      Assert::IsTrue(allTrue(givenOnBitSet == actual), L"blendMasked full mask value mismatch", LINE_INFO());
    }
  };
}
