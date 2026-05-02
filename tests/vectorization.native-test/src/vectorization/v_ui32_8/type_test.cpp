#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_ui32_8_TypeTest) {
  public:
    TEST_METHOD(definesArchitecture) {
      Assert::AreEqual(VectorSizes::X8, v_ui32_8::SIZE, L"Size mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<UInt_32, v_ui32_8::ValueType>, L"Value type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<UInt_32, v_ui32_8::BoolType>, L"Bool type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_ui32_8, v_ui32_8::VectorType>, L"Vector type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_ui32_8, v_ui32_8::VectorBoolType>, L"Vector bool type mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsZero) {
      const v_ui32_8 actual;
      Assert::IsTrue(allTrue(Zero<v_ui32_8>() == actual));
    }

    TEST_METHOD(constructsFromX1X2X3X4X5X6X7X8) {
      const v_ui32_8 actual{1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u};

      Assert::AreEqual(UInt_32{1}, x1(actual), L"X1 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{2}, x2(actual), L"X2 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{3}, x3(actual), L"X3 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{4}, x4(actual), L"X4 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{5}, x5(actual), L"X5 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{6}, x6(actual), L"X6 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{7}, x7(actual), L"X7 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{8}, x8(actual), L"X8 mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsFromX1X2X3X4With0000) {
      const v_ui32_8 expected{1u, 2u, 3u, 4u, 0u, 0u, 0u, 0u};
      const v_ui32_8 actual{1u, 2u, 3u, 4u};
      Assert::IsTrue(allTrue(expected == actual));
    }

    TEST_METHOD(constructsBroadcastFromValueType) {
      const v_ui32_8 actual{UInt_32{42}};

      Assert::AreEqual(UInt_32{42}, x1(actual), L"X1 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{42}, x5(actual), L"X5 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{42}, x8(actual), L"X8 mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsCopy) {
      const v_ui32_8 given{1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u};
      const v_ui32_8 actual{given};
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(constructsFromPackedType) {
      const v_ui32_8 given{1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u};
      const v_ui32_8 actual{given.components};
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(constructsFromValueTypePtr) {
      alignas(32) std::array<UInt_32, 8> given{1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u};
      const v_ui32_8 expected{1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u};
      const v_ui32_8 actual{given.data()};
      Assert::IsTrue(allTrue(expected == actual));
    }

    TEST_METHOD(implicitlyConvertsFromPackedType) {
      const v_ui32_8 given{1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u};
      const v_ui32_8 actual = given.components;
      Assert::IsTrue(allTrue(given == actual));
    }

    TEST_METHOD(readsAtIndex) {
      const v_ui32_8 given{1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u};
      Assert::AreEqual(UInt_32{1}, given[VectorIndices::X1], L"X1 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{4}, given[VectorIndices::X4], L"X4 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{8}, given[VectorIndices::X8], L"X8 mismatch", LINE_INFO());
    }

    TEST_METHOD(storesValues) {
      const v_ui32_8 given{10u, 20u, 30u, 40u, 50u, 60u, 70u, 80u};
      alignas(32) std::array<UInt_32, 8> dst{};
      store(given, dst.data());
      Assert::AreEqual(UInt_32{10}, dst[0], L"store X1 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{80}, dst[7], L"store X8 mismatch", LINE_INFO());
    }
  };
}
