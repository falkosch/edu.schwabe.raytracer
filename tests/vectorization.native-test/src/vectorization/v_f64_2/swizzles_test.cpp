#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f64_2_SwizzlesTest) {
  public:
    TEST_METHOD(swizzlesIdentity) {
      const v_f64_2 given{1.0, 2.0};
      const auto result = swizzle<VectorIndices::X, VectorIndices::Y>(given);
      Assert::AreEqual(1.0, x(result), L"identity X mismatch", LINE_INFO());
      Assert::AreEqual(2.0, y(result), L"identity Y mismatch", LINE_INFO());
    }

    TEST_METHOD(swizzlesReverse) {
      const v_f64_2 given{1.0, 2.0};
      const auto result = swizzle<VectorIndices::Y, VectorIndices::X>(given);
      Assert::AreEqual(2.0, x(result), L"reverse X mismatch", LINE_INFO());
      Assert::AreEqual(1.0, y(result), L"reverse Y mismatch", LINE_INFO());
    }

    TEST_METHOD(xxBroadcastsX) {
      const v_f64_2 given{3.0, 7.0};
      const auto result = xx(given);
      Assert::AreEqual(3.0, x(result), L"xx X mismatch", LINE_INFO());
      Assert::AreEqual(3.0, y(result), L"xx Y mismatch", LINE_INFO());
    }

    TEST_METHOD(yyBroadcastsY) {
      const v_f64_2 given{3.0, 7.0};
      const auto result = yy(given);
      Assert::AreEqual(7.0, x(result), L"yy X mismatch", LINE_INFO());
      Assert::AreEqual(7.0, y(result), L"yy Y mismatch", LINE_INFO());
    }

    TEST_METHOD(yxSwapsComponents) {
      const v_f64_2 given{5.0, 9.0};
      const auto result = yx(given);
      Assert::AreEqual(9.0, x(result), L"yx X mismatch", LINE_INFO());
      Assert::AreEqual(5.0, y(result), L"yx Y mismatch", LINE_INFO());
    }
  };
}
