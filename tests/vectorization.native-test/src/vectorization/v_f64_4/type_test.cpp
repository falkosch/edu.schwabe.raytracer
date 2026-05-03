#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f64_4_TypeTest) {
  public:
    TEST_METHOD(definesArchitecture) {
      Assert::AreEqual(VectorSizes::W, v_f64_4::SIZE, L"Size mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<Float_64, v_f64_4::ValueType>, L"Value type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_i64_4::ValueType, v_f64_4::BoolType>, L"Bool type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_f64_4, v_f64_4::VectorType>, L"Vector type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_i64_4, v_f64_4::VectorBoolType>, L"Vector bool type mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsZero) {
      const v_f64_4 v{};
      Assert::AreEqual(0.0, x(v), L"Default X not zero", LINE_INFO());
      Assert::AreEqual(0.0, y(v), L"Default Y not zero", LINE_INFO());
      Assert::AreEqual(0.0, z(v), L"Default Z not zero", LINE_INFO());
      Assert::AreEqual(0.0, w(v), L"Default W not zero", LINE_INFO());
    }

    TEST_METHOD(constructsFromScalar) {
      const v_f64_4 v{3.0};
      Assert::AreEqual(3.0, x(v), L"Scalar X mismatch", LINE_INFO());
      Assert::AreEqual(3.0, y(v), L"Scalar Y mismatch", LINE_INFO());
      Assert::AreEqual(3.0, z(v), L"Scalar Z mismatch", LINE_INFO());
      Assert::AreEqual(3.0, w(v), L"Scalar W mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsFromXYZW) {
      const v_f64_4 v{1.0, 2.0, 3.0, 4.0};
      Assert::AreEqual(1.0, x(v), L"X mismatch", LINE_INFO());
      Assert::AreEqual(2.0, y(v), L"Y mismatch", LINE_INFO());
      Assert::AreEqual(3.0, z(v), L"Z mismatch", LINE_INFO());
      Assert::AreEqual(4.0, w(v), L"W mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsFromValueTypePtr) {
      std::array<double, 4> data{5.0, 6.0, 7.0, 8.0};
      const v_f64_4 v{data.data()};
      Assert::AreEqual(5.0, x(v), L"Ptr X mismatch", LINE_INFO());
      Assert::AreEqual(6.0, y(v), L"Ptr Y mismatch", LINE_INFO());
      Assert::AreEqual(7.0, z(v), L"Ptr Z mismatch", LINE_INFO());
      Assert::AreEqual(8.0, w(v), L"Ptr W mismatch", LINE_INFO());
    }

    TEST_METHOD(readsAtIndex) {
      const v_f64_4 v{1.0, 2.0, 3.0, 4.0};
      Assert::AreEqual(1.0, v[VectorIndices::X], L"Index X mismatch", LINE_INFO());
      Assert::AreEqual(2.0, v[VectorIndices::Y], L"Index Y mismatch", LINE_INFO());
      Assert::AreEqual(3.0, v[VectorIndices::Z], L"Index Z mismatch", LINE_INFO());
      Assert::AreEqual(4.0, v[VectorIndices::W], L"Index W mismatch", LINE_INFO());
    }

    TEST_METHOD(storesToVectorTypeAndArray) {
      const v_f64_4 given{1.0, 2.0, 3.0, 4.0};

      v_f64_4 dst;
      store(given, &dst);
      Assert::AreEqual(x(given), x(dst), L"Store vector X mismatch", LINE_INFO());
      Assert::AreEqual(w(given), w(dst), L"Store vector W mismatch", LINE_INFO());

      std::array<v_f64_4::ValueType, v_f64_4::SIZE> arr{};
      store(given, arr.data());
      Assert::AreEqual(1.0, arr.at(0), L"Store array X mismatch", LINE_INFO());
      Assert::AreEqual(4.0, arr.at(3), L"Store array W mismatch", LINE_INFO());
    }
  };
}
