#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_ui64_2_TypeTest) {
  public:
    TEST_METHOD(definesArchitecture) {
      Assert::AreEqual(VectorSizes::Y, v_ui64_2::SIZE, L"Size mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<UInt_64, v_ui64_2::ValueType>, L"Value type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<UInt_64, v_ui64_2::BoolType>, L"Bool type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_ui64_2, v_ui64_2::VectorType>, L"Vector type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_ui64_2, v_ui64_2::VectorBoolType>, L"Vector bool type mismatch", LINE_INFO());
      Assert::IsTrue(
          std::is_same_v<PackedTypes<UInt_64, VectorSizes::Y>::Type, v_ui64_2::PackedType>, L"Packed type mismatch",
          LINE_INFO()
      );
    }

    TEST_METHOD(constructsZero) {
      const v_ui64_2 actual;
      Assert::IsTrue(allTrue(Zero<v_ui64_2>() == actual));
    }

    TEST_METHOD(constructsFromXY) {
      const v_ui64_2 actual{UInt_64{100}, UInt_64{200}};

      Assert::AreEqual(UInt_64{100}, x(actual), L"X mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{200}, y(actual), L"Y mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsBroadcastFromValueType) {
      const v_ui64_2 actual{UInt_64{42}};

      Assert::AreEqual(UInt_64{42}, x(actual), L"X mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{42}, y(actual), L"Y mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsCopy) {
      const v_ui64_2 given{UInt_64{100}, UInt_64{200}};
      const v_ui64_2 actual{given};
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(constructsFromPackedType) {
      const v_ui64_2 given{UInt_64{100}, UInt_64{200}};
      const v_ui64_2 actual{given.components};
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(constructsFromVectorTypePtr) {
      v_ui64_2 given{UInt_64{100}, UInt_64{200}};
      const v_ui64_2 actual{&given};
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(constructsFromValueTypePtr) {
      alignas(16) std::array<UInt_64, 2> given{100ull, 200ull};
      const v_ui64_2 expected{given.at(VectorIndices::X), given.at(VectorIndices::Y)};
      const v_ui64_2 actual{given.data()};
      Assert::IsTrue(allTrue(expected == actual));
    }

    TEST_METHOD(constructsFromPackedTypePtr) {
      const v_ui64_2 given{UInt_64{100}, UInt_64{200}};
      const v_ui64_2 actual{&given.components};
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(implicitlyConvertsFromPackedType) {
      const v_ui64_2 given{UInt_64{100}, UInt_64{200}};
      const v_ui64_2 actual = given.components;
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(readsAtIndex) {
      const v_ui64_2 given{UInt_64{100}, UInt_64{200}};
      Assert::AreEqual(UInt_64{100}, given[VectorIndices::X], L"X mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{200}, given[VectorIndices::Y], L"Y mismatch", LINE_INFO());
    }
  };
}
