#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_ui32_4_SelectsTest) {
  public:
    TEST_METHOD(selectsOneOfTwov_ui32_4WithBoolConditionMask) {
      const v_ui32_4 givenTruthy{1u, 2u, 3u, 4u};
      const v_ui32_4 givenFalsy{5u, 6u, 7u, 8u};

      {
        const v_ui32_4 actual = select(true, givenTruthy.components, givenFalsy.components);
        Assert::IsTrue(allTrue(givenTruthy == actual), L"select value mismatch", LINE_INFO());
      }

      {
        const v_ui32_4 actual = select(false, givenTruthy.components, givenFalsy.components);
        Assert::IsTrue(allTrue(givenFalsy == actual), L"select value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(selectsComponentsOfTwov_ui32_4WithPackedConditionMask) {
      const v_ui32_4 givenTruthy{1u, 2u, 3u, 4u};
      const v_ui32_4 givenFalsy{5u, 6u, 7u, 8u};

      {
        const v_ui32_4 actual = select(MaskAll<v_ui32_4>().components, givenTruthy.components, givenFalsy.components);
        Assert::IsTrue(allTrue(givenTruthy == actual), L"select value mismatch", LINE_INFO());
      }

      {
        const v_ui32_4 actual = select(Zero<v_ui32_4>().components, givenTruthy.components, givenFalsy.components);
        Assert::IsTrue(allTrue(givenFalsy == actual), L"select value mismatch", LINE_INFO());
      }
    }
  };
}
