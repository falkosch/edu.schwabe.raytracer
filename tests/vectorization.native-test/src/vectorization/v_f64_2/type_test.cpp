#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f64_2_TypeTest) {
  public:
    TEST_METHOD(definesArchitecture) {
      Assert::AreEqual(VectorSizes::Y, v_f64_2::SIZE, L"Size mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<Float_64, v_f64_2::ValueType>, L"Value type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_ui64_2::ValueType, v_f64_2::BoolType>, L"Bool type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_f64_2, v_f64_2::VectorType>, L"Vector type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_ui64_2, v_f64_2::VectorBoolType>, L"Vector bool type mismatch", LINE_INFO());
      Assert::IsTrue(
          std::is_same_v<PackedFloat2_128, v_f64_2::PackedType>, L"Packed type mismatch", LINE_INFO()
      );
    }

    TEST_METHOD(constructsZero) {
      const v_f64_2 v{};
      Assert::AreEqual(0.0, x(v), L"Default X not zero", LINE_INFO());
      Assert::AreEqual(0.0, y(v), L"Default Y not zero", LINE_INFO());
    }

    TEST_METHOD(constructsFromScalar) {
      const v_f64_2 v{3.0};
      Assert::AreEqual(3.0, x(v), L"Scalar X mismatch", LINE_INFO());
      Assert::AreEqual(3.0, y(v), L"Scalar Y mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsFromXY) {
      const v_f64_2 v{1.0, 2.0};
      Assert::AreEqual(1.0, x(v), L"X mismatch", LINE_INFO());
      Assert::AreEqual(2.0, y(v), L"Y mismatch", LINE_INFO());
    }

    TEST_METHOD(readsAtIndex) {
      const v_f64_2 v{1.0, 2.0};
      Assert::AreEqual(1.0, v[VectorIndices::X], L"Index X mismatch", LINE_INFO());
      Assert::AreEqual(2.0, v[VectorIndices::Y], L"Index Y mismatch", LINE_INFO());
    }

    TEST_METHOD(storesToVectorType) {
      const v_f64_2 given{1.0, 2.0};
      v_f64_2 actual;
      store(given, &actual);
      Assert::AreEqual(x(given), x(actual), L"Store X mismatch", LINE_INFO());
      Assert::AreEqual(y(given), y(actual), L"Store Y mismatch", LINE_INFO());
    }

    TEST_METHOD(storesToValueTypeArray) {
      const v_f64_2 given{3.0, 4.0};
      std::array<v_f64_2::ValueType, v_f64_2::SIZE> actual{};
      store(given, actual.data());
      Assert::AreEqual(3.0, actual.at(VectorIndices::X), L"Store array X mismatch", LINE_INFO());
      Assert::AreEqual(4.0, actual.at(VectorIndices::Y), L"Store array Y mismatch", LINE_INFO());
    }
  };
}
