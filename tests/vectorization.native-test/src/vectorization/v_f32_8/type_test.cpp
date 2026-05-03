#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f32_8_TypeTest) {
  public:
    TEST_METHOD(definesArchitecture) {
      Assert::AreEqual(VectorSizes::X8, v_f32_8::SIZE, L"Size mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<Float_32, v_f32_8::ValueType>, L"Value type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<Int_32, v_f32_8::BoolType>, L"Bool type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_f32_8, v_f32_8::VectorType>, L"Vector type mismatch", LINE_INFO());
      Assert::IsTrue(std::is_same_v<v_i32_8, v_f32_8::VectorBoolType>, L"Vector bool type mismatch", LINE_INFO());
    }

    TEST_METHOD(hasCorrectSizeInBytes) {
      Assert::AreEqual(static_cast<size_t>(32), sizeof(v_f32_8), L"sizeof mismatch", LINE_INFO());
    }

    TEST_METHOD(constructsZero) {
      const v_f32_8 v;
      Assert::AreEqual(0.0f, x1(v), L"x1", LINE_INFO());
      Assert::AreEqual(0.0f, x5(v), L"x5", LINE_INFO());
      Assert::AreEqual(0.0f, x8(v), L"x8", LINE_INFO());
    }

    TEST_METHOD(constructsBroadcast) {
      const v_f32_8 v(3.0f);
      Assert::AreEqual(3.0f, x1(v), L"x1", LINE_INFO());
      Assert::AreEqual(3.0f, x4(v), L"x4", LINE_INFO());
      Assert::AreEqual(3.0f, x5(v), L"x5", LINE_INFO());
      Assert::AreEqual(3.0f, x8(v), L"x8", LINE_INFO());
    }

    TEST_METHOD(constructsFrom4Components) {
      const v_f32_8 v(1.0f, 2.0f, 3.0f, 4.0f);
      Assert::AreEqual(1.0f, x1(v), L"x1", LINE_INFO());
      Assert::AreEqual(2.0f, x2(v), L"x2", LINE_INFO());
      Assert::AreEqual(3.0f, x3(v), L"x3", LINE_INFO());
      Assert::AreEqual(4.0f, x4(v), L"x4", LINE_INFO());
      Assert::AreEqual(0.0f, x5(v), L"x5", LINE_INFO());
      Assert::AreEqual(0.0f, x8(v), L"x8", LINE_INFO());
    }

    TEST_METHOD(constructsFrom8Components) {
      const v_f32_8 v(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      Assert::AreEqual(1.0f, x1(v), L"x1", LINE_INFO());
      Assert::AreEqual(2.0f, x2(v), L"x2", LINE_INFO());
      Assert::AreEqual(3.0f, x3(v), L"x3", LINE_INFO());
      Assert::AreEqual(4.0f, x4(v), L"x4", LINE_INFO());
      Assert::AreEqual(5.0f, x5(v), L"x5", LINE_INFO());
      Assert::AreEqual(6.0f, x6(v), L"x6", LINE_INFO());
      Assert::AreEqual(7.0f, x7(v), L"x7", LINE_INFO());
      Assert::AreEqual(8.0f, x8(v), L"x8", LINE_INFO());
    }

    TEST_METHOD(constructsFromPackedType) {
      const v_f32_8 given(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_f32_8 actual(given.components);
      Assert::IsTrue(allTrue(given == actual), L"packed type round-trip", LINE_INFO());
    }

    TEST_METHOD(readsAtIndex) {
      const v_f32_8 v(10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f);
      Assert::AreEqual(10.0f, v[0], L"[0]", LINE_INFO());
      Assert::AreEqual(40.0f, v[3], L"[3]", LINE_INFO());
      Assert::AreEqual(50.0f, v[4], L"[4]", LINE_INFO());
      Assert::AreEqual(80.0f, v[7], L"[7]", LINE_INFO());
    }

    TEST_METHOD(writesAtIndex) {
      v_f32_8 v(0.0f);
      v[0] = 1.0f;
      v[7] = 8.0f;
      Assert::AreEqual(1.0f, x1(v), L"x1", LINE_INFO());
      Assert::AreEqual(8.0f, x8(v), L"x8", LINE_INFO());
    }

    TEST_METHOD(storesToVectorType) {
      const v_f32_8 given(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      v_f32_8 actual;
      store(given, &actual);
      Assert::IsTrue(allTrue(given == actual), L"store vector", LINE_INFO());
    }

    TEST_METHOD(storesToValueTypeArray) {
      const v_f32_8 given(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      alignas(32) std::array<v_f32_8::ValueType, v_f32_8::SIZE> buf{};
      store(given, buf.data());
      Assert::AreEqual(1.0f, buf[0], L"[0]", LINE_INFO());
      Assert::AreEqual(8.0f, buf[7], L"[7]", LINE_INFO());
    }

    TEST_METHOD(implicitlyConvertsFromPackedType) {
      const v_f32_8 given(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_f32_8 actual = given.components;
      Assert::IsTrue(allTrue(given == actual), L"implicit convert", LINE_INFO());
    }

    TEST_METHOD(assignsFromPackedType) {
      const v_f32_8 given(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      v_f32_8 actual;
      actual = given.components;
      Assert::IsTrue(allTrue(given == actual), L"assign", LINE_INFO());
    }
  };
}
