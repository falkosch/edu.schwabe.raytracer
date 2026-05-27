#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_i32_4_OperatorsTest) {
  public:
    TEST_METHOD(testNegationOperator) {
      const v_i32_4 given{1, -2, 3, -4};
      const v_i32_4 expected{-1, 2, -3, 4};
      const auto actual = -given;
      Assert::IsTrue(allTrue(expected == actual), L"'-' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testOnesComplementOperator) {
      const auto actual = ~MaskAll<v_i32_4>();
      Assert::IsTrue(allTrue(Zero<v_i32_4>() == actual), L"'~' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testLogicalNegationOperator) {
      const v_i32_4 given{0, 1, 0, -1};
      const auto actual = !given;
      Assert::AreEqual(MaskAll<v_i32_4::BoolType>(), x(actual), L"'!' op value mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, y(actual), L"'!' op value mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_i32_4::BoolType>(), z(actual), L"'!' op value mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, w(actual), L"'!' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testAddVectorOperator) {
      const v_i32_4 a{1, 2, 3, 4};
      const v_i32_4 b{10, 20, 30, 40};
      const v_i32_4 expected{11, 22, 33, 44};
      Assert::IsTrue(allTrue(expected == (a + b)), L"'+' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testSubtractVectorOperator) {
      const v_i32_4 a{10, 20, 30, 40};
      const v_i32_4 b{1, 2, 3, 4};
      const v_i32_4 expected{9, 18, 27, 36};
      Assert::IsTrue(allTrue(expected == (a - b)), L"'-' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testMultiplyVectorOperator) {
      const v_i32_4 a{2, 3, 4, 5};
      const v_i32_4 b{10, 20, 30, 40};
      const v_i32_4 expected{20, 60, 120, 200};
      Assert::IsTrue(allTrue(expected == (a * b)), L"'*' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testDivideVectorOperator) {
      const v_i32_4 a{10, 20, 30, 40};
      const v_i32_4 b{2, 5, 10, 8};
      const v_i32_4 expected{5, 4, 3, 5};
      Assert::IsTrue(allTrue(expected == (a / b)), L"'/' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testModuloVectorOperator) {
      const v_i32_4 a{10, 21, 33, 47};
      const v_i32_4 b{3, 5, 10, 8};
      const v_i32_4 expected{1, 1, 3, 7};
      Assert::IsTrue(allTrue(expected == (a % b)), L"'%' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testBitwiseAndOperator) {
      const auto a = MaskAll<v_i32_4>();
      const auto b = MaskX<v_i32_4>();
      const auto actual = a & b;
      Assert::IsTrue(allTrue(b == actual), L"'&' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testBitwiseOrOperator) {
      const auto a = MaskX<v_i32_4>();
      const auto b = MaskY<v_i32_4>();
      const auto expected = MaskXY<v_i32_4>();
      const auto actual = a | b;
      Assert::IsTrue(allTrue(expected == actual), L"'|' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testBitwiseXorOperator) {
      const auto a = MaskAll<v_i32_4>();
      const auto actual = a ^ a;
      Assert::IsTrue(allTrue(Zero<v_i32_4>() == actual), L"'^' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testLeftShiftOperator) {
      const v_i32_4 given{1, 2, 4, 8};
      const v_i32_4 shift{0, 1, 2, 3};
      const v_i32_4 expected{1, 4, 16, 64};
      Assert::IsTrue(allTrue(expected == (given << shift)), L"'<<' op per-lane mismatch", LINE_INFO());
    }

    TEST_METHOD(testRightShiftOperator) {
      const v_i32_4 given{-8, -16, -32, -64};
      const v_i32_4 shift{1, 2, 3, 4};
      const v_i32_4 expected{-4, -4, -4, -4};
      Assert::IsTrue(allTrue(expected == (given >> shift)), L"'>>' op arithmetic shift mismatch", LINE_INFO());
    }

    TEST_METHOD(testEqualsOperator) {
      const v_i32_4 a{1, 2, 3, 4};
      const v_i32_4 b{1, 9, 3, 9};
      const auto actual = a == b;

      Assert::AreEqual(MaskAll<v_i32_4::BoolType>(), x(actual), L"Equals value mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, y(actual), L"Equals value mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_i32_4::BoolType>(), z(actual), L"Equals value mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, w(actual), L"Equals value mismatch", LINE_INFO());
    }

    TEST_METHOD(testNotEqualsOperator) {
      const v_i32_4 a{1, 2, 3, 4};
      const v_i32_4 b{1, 9, 3, 9};
      const auto actual = a != b;

      Assert::AreEqual(Int_32{0}, x(actual), L"NotEquals value mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_i32_4::BoolType>(), y(actual), L"NotEquals value mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, z(actual), L"NotEquals value mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_i32_4::BoolType>(), w(actual), L"NotEquals value mismatch", LINE_INFO());
    }

    TEST_METHOD(testLessThanOperator) {
      const v_i32_4 a{1, 5, 3, 4};
      const v_i32_4 b{2, 5, 1, 9};
      const auto actual = a < b;

      Assert::AreEqual(MaskAll<v_i32_4::BoolType>(), x(actual), L"'<' value mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, y(actual), L"'<' value mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, z(actual), L"'<' value mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_i32_4::BoolType>(), w(actual), L"'<' value mismatch", LINE_INFO());
    }

    TEST_METHOD(testGreaterThanOperator) {
      const v_i32_4 a{2, 5, 1, 9};
      const v_i32_4 b{1, 5, 3, 4};
      const auto actual = a > b;

      Assert::AreEqual(MaskAll<v_i32_4::BoolType>(), x(actual), L"'>' value mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, y(actual), L"'>' value mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, z(actual), L"'>' value mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_i32_4::BoolType>(), w(actual), L"'>' value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundAddAssignOperator) {
      v_i32_4 actual{1, 2, 3, 4};
      actual += v_i32_4{10, 20, 30, 40};
      const v_i32_4 expected{11, 22, 33, 44};
      Assert::IsTrue(allTrue(expected == actual), L"'+=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundSubtractAssignOperator) {
      v_i32_4 actual{10, 20, 30, 40};
      actual -= v_i32_4{1, 2, 3, 4};
      const v_i32_4 expected{9, 18, 27, 36};
      Assert::IsTrue(allTrue(expected == actual), L"'-=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundBitwiseAndAssignOperator) {
      v_i32_4 actual = MaskAll<v_i32_4>();
      actual &= MaskX<v_i32_4>();
      Assert::IsTrue(allTrue(MaskX<v_i32_4>() == actual), L"'&=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testLessThanOrEqualOperator) {
      const v_i32_4 a{1, 2, 3, 4};
      const v_i32_4 b{2, 2, 2, 5};
      const auto result = a <= b;
      // X: 1<=2 true, Y: 2<=2 true, Z: 3<=2 false, W: 4<=5 true
      Assert::AreEqual(Int_32{-1}, x(result), L"X mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{-1}, y(result), L"Y mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, z(result), L"Z mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{-1}, w(result), L"W mismatch", LINE_INFO());
    }

    TEST_METHOD(testGreaterThanOrEqualOperator) {
      const v_i32_4 a{1, 2, 3, 4};
      const v_i32_4 b{2, 2, 2, 5};
      const auto result = a >= b;
      // X: 1>=2 false, Y: 2>=2 true, Z: 3>=2 true, W: 4>=5 false
      Assert::AreEqual(Int_32{0}, x(result), L"X mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{-1}, y(result), L"Y mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{-1}, z(result), L"Z mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, w(result), L"W mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundMultiplyAssignOperator) {
      v_i32_4 a{10, 20, 30, 40};
      const v_i32_4 b{2, 3, 5, 4};
      const auto expected = a * b;
      a *= b;
      Assert::IsTrue(allTrue(expected == a), L"'*=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundDivideAssignOperator) {
      v_i32_4 a{10, 20, 30, 40};
      const v_i32_4 b{2, 3, 5, 4};
      const auto expected = a / b;
      a /= b;
      Assert::IsTrue(allTrue(expected == a), L"'/=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundModuloAssignOperator) {
      v_i32_4 a{10, 20, 30, 40};
      const v_i32_4 b{2, 3, 5, 4};
      const auto expected = a % b;
      a %= b;
      Assert::IsTrue(allTrue(expected == a), L"'%%=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundBitwiseOrAssignOperator) {
      v_i32_4 a{10, 20, 30, 40};
      const v_i32_4 b{2, 3, 5, 4};
      const auto expected = a | b;
      a |= b;
      Assert::IsTrue(allTrue(expected == a), L"'|=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundBitwiseXorAssignOperator) {
      v_i32_4 a{10, 20, 30, 40};
      const v_i32_4 b{2, 3, 5, 4};
      const auto expected = a ^ b;
      a ^= b;
      Assert::IsTrue(allTrue(expected == a), L"'^=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundLeftShiftAssignOperator) {
      v_i32_4 a{16, 32, 64, 128};
      const v_i32_4 b{1, 0, 0, 0};
      const auto expected = a << b;
      a <<= b;
      Assert::IsTrue(allTrue(expected == a), L"'<<=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundRightShiftAssignOperator) {
      v_i32_4 a{16, 32, 64, 128};
      const v_i32_4 b{1, 0, 0, 0};
      const auto expected = a >> b;
      a >>= b;
      Assert::IsTrue(allTrue(expected == a), L"'>>=' op value mismatch", LINE_INFO());
    }
  };
}
