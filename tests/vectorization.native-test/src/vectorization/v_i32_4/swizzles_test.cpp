#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_i32_4_SwizzlesTest) {
  public:
    TEST_METHOD(genericallySwizzlesIdentityAndReverse) {
      const v_i32_4 given{10, 20, 30, 40};

      {
        const auto actual = swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(given);
        Assert::IsTrue(allTrue(given == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        const v_i32_4 expected{w(given), z(given), y(given), x(given)};
        const auto actual = swizzle<VectorIndices::W, VectorIndices::Z, VectorIndices::Y, VectorIndices::X>(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallySwizzlesBroadcast) {
      const v_i32_4 given{10, 20, 30, 40};

      {
        const v_i32_4 expected{x(given), x(given), x(given), x(given)};
        const auto actual = swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        const v_i32_4 expected{w(given), w(given), w(given), w(given)};
        const auto actual = swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(genericallySwizzlesDuplicateForms) {
      const v_i32_4 given{10, 20, 30, 40};

      {
        const v_i32_4 expected{x(given), x(given), y(given), y(given)};
        const auto actual = swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::Y, VectorIndices::Y>(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        const v_i32_4 expected{x(given), y(given), x(given), y(given)};
        const auto actual = swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        const v_i32_4 expected{z(given), z(given), w(given), w(given)};
        const auto actual = swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        const v_i32_4 expected{z(given), w(given), z(given), w(given)};
        const auto actual = swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(hasShortFormsForBroadcasts) {
      const v_i32_4 given{10, 20, 30, 40};

      Assert::IsTrue(allTrue(v_i32_4{x(given), x(given), x(given), x(given)} == xxxx(given)), L"xxxx mismatch",
                     LINE_INFO());
      Assert::IsTrue(allTrue(v_i32_4{y(given), y(given), y(given), y(given)} == yyyy(given)), L"yyyy mismatch",
                     LINE_INFO());
      Assert::IsTrue(allTrue(v_i32_4{z(given), z(given), z(given), z(given)} == zzzz(given)), L"zzzz mismatch",
                     LINE_INFO());
      Assert::IsTrue(allTrue(v_i32_4{w(given), w(given), w(given), w(given)} == wwww(given)), L"wwww mismatch",
                     LINE_INFO());
    }

    TEST_METHOD(hasShortFormsForDuplicates) {
      const v_i32_4 given{10, 20, 30, 40};

      Assert::IsTrue(allTrue(v_i32_4{x(given), x(given), y(given), y(given)} == xxyy(given)), L"xxyy mismatch",
                     LINE_INFO());
      Assert::IsTrue(allTrue(v_i32_4{x(given), y(given), x(given), y(given)} == xyxy(given)), L"xyxy mismatch",
                     LINE_INFO());
      Assert::IsTrue(allTrue(v_i32_4{y(given), x(given), w(given), z(given)} == yxwz(given)), L"yxwz mismatch",
                     LINE_INFO());
      Assert::IsTrue(allTrue(v_i32_4{z(given), z(given), w(given), w(given)} == zzww(given)), L"zzww mismatch",
                     LINE_INFO());
      Assert::IsTrue(allTrue(v_i32_4{z(given), w(given), z(given), w(given)} == zwzw(given)), L"zwzw mismatch",
                     LINE_INFO());
    }
  };
}
