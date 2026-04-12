#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_ui32_4_TypeTest) {
  public:
    TEST_METHOD(definesArchitecture) {
      Assert::AreEqual(VectorSizes::W, v_ui32_4::SIZE, L"Size mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<UInt_32, v_ui32_4::ValueType>, L"Value type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<UInt_32, v_ui32_4::BoolType>, L"Bool type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_ui32_4, v_ui32_4::VectorType>, L"Vector type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_ui32_4, v_ui32_4::VectorBoolType>, L"Vector bool type mismatch", LINE_INFO());
      Assert::IsTrue(
          std::is_same_v<PackedTypes<UInt_32, VectorSizes::W>::Type, v_ui32_4::PackedType>, L"Packed type mismatch",
          LINE_INFO()
      );
    }

    TEST_METHOD(constructsZero) {
      const v_ui32_4 actual;
      Assert::IsTrue(allTrue(Zero<v_ui32_4>() == actual));
    }

    TEST_METHOD(constructsFromXYZW) {
      const v_ui32_4 actual{1u, 2u, 3u, 4u};

      Assert::AreEqual(UInt_32{1}, x(actual), L"X mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{2}, y(actual), L"Y mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{3}, z(actual), L"Z mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{4}, w(actual), L"W mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsFromXY00) {
      const v_ui32_4 expected{1u, 2u, 0u, 0u};
      const v_ui32_4 actual{1u, 2u};
      Assert::IsTrue(allTrue(expected == actual));
    }

    TEST_METHOD(constructsFromXYZ0) {
      const v_ui32_4 expected{1u, 2u, 3u, 0u};
      const v_ui32_4 actual{1u, 2u, 3u};
      Assert::IsTrue(allTrue(expected == actual));
    }

    TEST_METHOD(constructsBroadcastFromValueType) {
      const v_ui32_4 actual{UInt_32{42}};

      Assert::AreEqual(UInt_32{42}, x(actual), L"X mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{42}, y(actual), L"Y mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{42}, z(actual), L"Z mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{42}, w(actual), L"W mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsCopy) {
      const v_ui32_4 given{1u, 2u, 3u, 4u};
      const v_ui32_4 actual{given};
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(constructsFromPackedType) {
      const v_ui32_4 given{1u, 2u, 3u, 4u};
      const v_ui32_4 actual{given.components};
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(constructsFromVectorTypePtr) {
      v_ui32_4 given{1u, 2u, 3u, 4u};
      const v_ui32_4 actual{&given};
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(constructsFromValueTypePtr) {
      alignas(16) std::array<UInt_32, 4> given{1u, 2u, 3u, 4u};
      const v_ui32_4 expected{
          given.at(VectorIndices::X),
          given.at(VectorIndices::Y),
          given.at(VectorIndices::Z),
          given.at(VectorIndices::W),
      };
      const v_ui32_4 actual{given.data()};
      Assert::IsTrue(allTrue(expected == actual));
    }

    TEST_METHOD(constructsFromPackedTypePtr) {
      const v_ui32_4 given{1u, 2u, 3u, 4u};
      const v_ui32_4 actual{&given.components};
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(implicitlyConvertsFromPackedType) {
      const v_ui32_4 given{1u, 2u, 3u, 4u};
      const v_ui32_4 actual = given.components;
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(readsAtIndex) {
      const v_ui32_4 given{1u, 2u, 3u, 4u};
      Assert::AreEqual(UInt_32{1}, given[VectorIndices::X], L"X mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{2}, given[VectorIndices::Y], L"Y mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{3}, given[VectorIndices::Z], L"Z mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{4}, given[VectorIndices::W], L"W mismatch", LINE_INFO());
    }
  };
}
