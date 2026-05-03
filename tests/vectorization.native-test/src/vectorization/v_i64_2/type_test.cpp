#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_i64_2_TypeTest) {
  public:
    TEST_METHOD(definesArchitecture) {
      Assert::AreEqual(VectorSizes::Y, v_i64_2::SIZE, L"Size mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<Int_64, v_i64_2::ValueType>, L"Value type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<Int_64, v_i64_2::BoolType>, L"Bool type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_i64_2, v_i64_2::VectorType>, L"Vector type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_i64_2, v_i64_2::VectorBoolType>, L"Vector bool type mismatch", LINE_INFO());
      Assert::IsTrue(
          std::is_same_v<PackedTypes<Int_64, VectorSizes::Y>::Type, v_i64_2::PackedType>, L"Packed type mismatch",
          LINE_INFO()
      );
    }

    TEST_METHOD(constructsZero) {
      const v_i64_2 actual;
      Assert::IsTrue(allTrue(Zero<v_i64_2>() == actual));
    }

    TEST_METHOD(constructsFromXY) {
      const v_i64_2 actual{Int_64{10LL}, Int_64{-5LL}};

      Assert::AreEqual(Int_64{10LL}, x(actual), L"X mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{-5LL}, y(actual), L"Y mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsBroadcastFromValueType) {
      const v_i64_2 actual{Int_64{30LL}};

      Assert::AreEqual(Int_64{30LL}, x(actual), L"X mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{30LL}, y(actual), L"Y mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsCopy) {
      const v_i64_2 given{Int_64{10LL}, Int_64{-5LL}};
      const v_i64_2 actual{given};
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(constructsFromPackedType) {
      const v_i64_2 given{Int_64{10LL}, Int_64{-5LL}};
      const v_i64_2 actual{given.components};
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(constructsFromVectorTypePtr) {
      v_i64_2 given{Int_64{10LL}, Int_64{-5LL}};
      const v_i64_2 actual{&given};
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(constructsFromValueTypePtr) {
      alignas(16) std::array<Int_64, 2> given{10LL, -5LL};
      const v_i64_2 expected{given.at(VectorIndices::X), given.at(VectorIndices::Y)};
      const v_i64_2 actual{given.data()};
      Assert::IsTrue(allTrue(expected == actual));
    }

    TEST_METHOD(constructsFromPackedTypePtr) {
      const v_i64_2 given{Int_64{10LL}, Int_64{-5LL}};
      const v_i64_2 actual{&given.components};
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(implicitlyConvertsFromPackedType) {
      const v_i64_2 given{Int_64{10LL}, Int_64{-5LL}};
      const v_i64_2 actual = given.components;
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(readsAtIndex) {
      const v_i64_2 given{Int_64{10LL}, Int_64{-5LL}};
      Assert::AreEqual(Int_64{10LL}, given[VectorIndices::X], L"X mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{-5LL}, given[VectorIndices::Y], L"Y mismatch", LINE_INFO());
    }
  };
}
