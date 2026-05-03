#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_i64_2_SwizzlesTest) {
  public:
    TEST_METHOD(genericallySwizzlesIdentity) {
      const v_i64_2 given{Int_64{10LL}, Int_64{-5LL}};
      const auto actual = swizzle<VectorIndices::X, VectorIndices::Y>(given);
      Assert::IsTrue(allTrue(given == actual), L"swizzle value mismatch", LINE_INFO());
    }

    TEST_METHOD(genericallySwizzlesReverse) {
      const v_i64_2 given{Int_64{10LL}, Int_64{-5LL}};
      const v_i64_2 expected{y(given), x(given)};
      const auto actual = swizzle<VectorIndices::Y, VectorIndices::X>(given);
      Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
    }

    TEST_METHOD(genericallySwizzlesBroadcast) {
      const v_i64_2 given{Int_64{10LL}, Int_64{-5LL}};

      {
        const v_i64_2 expected{x(given), x(given)};
        const auto actual = swizzle<VectorIndices::X, VectorIndices::X>(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }

      {
        const v_i64_2 expected{y(given), y(given)};
        const auto actual = swizzle<VectorIndices::Y, VectorIndices::Y>(given);
        Assert::IsTrue(allTrue(expected == actual), L"swizzle value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(hasShortForms) {
      const v_i64_2 given{Int_64{10LL}, Int_64{-5LL}};

      Assert::IsTrue(allTrue(v_i64_2{x(given), x(given)} == xx(given)), L"xx mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_i64_2{x(given), y(given)} == xy(given)), L"xy mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_i64_2{y(given), x(given)} == yx(given)), L"yx mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(v_i64_2{y(given), y(given)} == yy(given)), L"yy mismatch", LINE_INFO());
    }
  };
}
