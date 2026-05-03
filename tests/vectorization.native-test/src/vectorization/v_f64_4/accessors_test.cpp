#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f64_4_AccessorsTest) {
  public:
    TEST_METHOD(readsComponents) {
      const v_f64_4 given{1.0, 2.0, 3.0, 4.0};

      Assert::AreEqual(1.0, component<VectorIndices::X>(given), L"component<X> mismatch", LINE_INFO());
      Assert::AreEqual(2.0, component<VectorIndices::Y>(given), L"component<Y> mismatch", LINE_INFO());
      Assert::AreEqual(3.0, component<VectorIndices::Z>(given), L"component<Z> mismatch", LINE_INFO());
      Assert::AreEqual(4.0, component<VectorIndices::W>(given), L"component<W> mismatch", LINE_INFO());
    }

    TEST_METHOD(readsXYZW) {
      const v_f64_4 given{5.0, 6.0, 7.0, 8.0};

      Assert::AreEqual(5.0, x(given), L"x() mismatch", LINE_INFO());
      Assert::AreEqual(6.0, y(given), L"y() mismatch", LINE_INFO());
      Assert::AreEqual(7.0, z(given), L"z() mismatch", LINE_INFO());
      Assert::AreEqual(8.0, w(given), L"w() mismatch", LINE_INFO());
    }

    TEST_METHOD(replacesComponentTemplated) {
      const v_f64_4 given{1.0, 2.0, 3.0, 4.0};

      const auto rx = replaceComponent<VectorIndices::X>(given, 9.0);
      Assert::AreEqual(9.0, x(rx), L"replaceComponent<X> X mismatch", LINE_INFO());
      Assert::AreEqual(2.0, y(rx), L"replaceComponent<X> Y mismatch", LINE_INFO());
      Assert::AreEqual(3.0, z(rx), L"replaceComponent<X> Z mismatch", LINE_INFO());
      Assert::AreEqual(4.0, w(rx), L"replaceComponent<X> W mismatch", LINE_INFO());

      const auto ry = replaceComponent<VectorIndices::Y>(given, 9.0);
      Assert::AreEqual(1.0, x(ry), L"replaceComponent<Y> X mismatch", LINE_INFO());
      Assert::AreEqual(9.0, y(ry), L"replaceComponent<Y> Y mismatch", LINE_INFO());

      const auto rz = replaceComponent<VectorIndices::Z>(given, 9.0);
      Assert::AreEqual(9.0, z(rz), L"replaceComponent<Z> Z mismatch", LINE_INFO());

      const auto rw = replaceComponent<VectorIndices::W>(given, 9.0);
      Assert::AreEqual(9.0, w(rw), L"replaceComponent<W> W mismatch", LINE_INFO());
    }

    TEST_METHOD(replacesXYZW) {
      const v_f64_4 given{1.0, 2.0, 3.0, 4.0};

      const auto rx = replaceX(given, 9.0);
      Assert::AreEqual(9.0, x(rx), L"replaceX mismatch", LINE_INFO());
      Assert::AreEqual(2.0, y(rx), L"replaceX Y preserved", LINE_INFO());

      const auto ry = replaceY(given, 9.0);
      Assert::AreEqual(9.0, y(ry), L"replaceY mismatch", LINE_INFO());

      const auto rz = replaceZ(given, 9.0);
      Assert::AreEqual(9.0, z(rz), L"replaceZ mismatch", LINE_INFO());

      const auto rw = replaceW(given, 9.0);
      Assert::AreEqual(9.0, w(rw), L"replaceW mismatch", LINE_INFO());
      Assert::AreEqual(1.0, x(rw), L"replaceW X preserved", LINE_INFO());
    }
  };
}
