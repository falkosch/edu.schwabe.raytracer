#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_i64_4_SwizzlesTest) {
  public:
    TEST_METHOD(genericallySwizzlesIdentity) {
      const v_i64_4 given{10LL, 20LL, 30LL, 40LL};
      const auto actual = swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W>(given);
      Assert::IsTrue(allTrue(given == actual), L"identity swizzle value mismatch", LINE_INFO());
    }

    TEST_METHOD(genericallySwizzlesBroadcast) {
      const v_i64_4 given{10LL, 20LL, 30LL, 40LL};

      {
        const v_i64_4 expected{x(given), x(given), x(given), x(given)};
        const auto actual = swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(given);
        Assert::IsTrue(allTrue(expected == actual), L"xxxx swizzle value mismatch", LINE_INFO());
      }

      {
        const v_i64_4 expected{w(given), w(given), w(given), w(given)};
        const auto actual = swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(given);
        Assert::IsTrue(allTrue(expected == actual), L"wwww swizzle value mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(hasShortFormXxxx) {
      const v_i64_4 given{10LL, 20LL, 30LL, 40LL};
      const v_i64_4 expected{x(given), x(given), x(given), x(given)};
      Assert::IsTrue(allTrue(expected == xxxx(given)), L"xxxx mismatch", LINE_INFO());
    }

    TEST_METHOD(hasShortFormYyyy) {
      const v_i64_4 given{10LL, 20LL, 30LL, 40LL};
      const v_i64_4 expected{y(given), y(given), y(given), y(given)};
      Assert::IsTrue(allTrue(expected == yyyy(given)), L"yyyy mismatch", LINE_INFO());
    }

    TEST_METHOD(hasShortFormZzzz) {
      const v_i64_4 given{10LL, 20LL, 30LL, 40LL};
      const v_i64_4 expected{z(given), z(given), z(given), z(given)};
      Assert::IsTrue(allTrue(expected == zzzz(given)), L"zzzz mismatch", LINE_INFO());
    }

    TEST_METHOD(hasShortFormWwww) {
      const v_i64_4 given{10LL, 20LL, 30LL, 40LL};
      const v_i64_4 expected{w(given), w(given), w(given), w(given)};
      Assert::IsTrue(allTrue(expected == wwww(given)), L"wwww mismatch", LINE_INFO());
    }
  };
}
