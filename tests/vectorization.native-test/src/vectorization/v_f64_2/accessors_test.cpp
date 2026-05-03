#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f64_2_AccessorsTest) {
  public:
    TEST_METHOD(readsComponents) {
      const v_f64_2 given{1.0, 2.0};

      Assert::AreEqual(1.0, component<VectorIndices::X>(given), L"component<X> mismatch", LINE_INFO());
      Assert::AreEqual(2.0, component<VectorIndices::Y>(given), L"component<Y> mismatch", LINE_INFO());
    }

    TEST_METHOD(readsXY) {
      const v_f64_2 given{3.0, 4.0};

      Assert::AreEqual(3.0, x(given), L"x() mismatch", LINE_INFO());
      Assert::AreEqual(4.0, y(given), L"y() mismatch", LINE_INFO());
      Assert::AreEqual(3.0, x1(given), L"x1() mismatch", LINE_INFO());
      Assert::AreEqual(4.0, x2(given), L"x2() mismatch", LINE_INFO());
    }

    TEST_METHOD(replacesComponentTemplated) {
      const v_f64_2 given{1.0, 2.0};
      const auto replacedX = replaceComponent<VectorIndices::X>(given, 9.0);
      Assert::AreEqual(9.0, x(replacedX), L"replaceComponent<X> X mismatch", LINE_INFO());
      Assert::AreEqual(2.0, y(replacedX), L"replaceComponent<X> Y mismatch", LINE_INFO());

      const auto replacedY = replaceComponent<VectorIndices::Y>(given, 9.0);
      Assert::AreEqual(1.0, x(replacedY), L"replaceComponent<Y> X mismatch", LINE_INFO());
      Assert::AreEqual(9.0, y(replacedY), L"replaceComponent<Y> Y mismatch", LINE_INFO());
    }

    TEST_METHOD(replacesXY) {
      const v_f64_2 given{1.0, 2.0};

      const auto rx = replaceX(given, 5.0);
      Assert::AreEqual(5.0, x(rx), L"replaceX X mismatch", LINE_INFO());
      Assert::AreEqual(2.0, y(rx), L"replaceX Y mismatch", LINE_INFO());

      const auto ry = replaceY(given, 5.0);
      Assert::AreEqual(1.0, x(ry), L"replaceY X mismatch", LINE_INFO());
      Assert::AreEqual(5.0, y(ry), L"replaceY Y mismatch", LINE_INFO());
    }

    TEST_METHOD(replacesComponentAtIndex) {
      const v_f64_2 given{1.0, 2.0};
      const auto replacement = v_f64_2(9.0);

      const auto r0 = replaceComponent(given, replacement, VectorIndices::X);
      Assert::AreEqual(9.0, x(r0), L"replaceComponent(idx=X) X mismatch", LINE_INFO());
      Assert::AreEqual(2.0, y(r0), L"replaceComponent(idx=X) Y mismatch", LINE_INFO());

      const auto r1 = replaceComponent(given, 9.0, VectorIndices::Y);
      Assert::AreEqual(1.0, x(r1), L"replaceComponent(idx=Y) X mismatch", LINE_INFO());
      Assert::AreEqual(9.0, y(r1), L"replaceComponent(idx=Y) Y mismatch", LINE_INFO());
    }
  };
}
