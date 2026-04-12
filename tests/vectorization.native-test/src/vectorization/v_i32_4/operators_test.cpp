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
      // _mm_sll_epi32 reads the shift count from the low 64-bit element
      const v_i32_4 shift{1, 0, 0, 0};
      const v_i32_4 expected{2, 4, 8, 16};
      Assert::IsTrue(allTrue(expected == (given << shift)), L"'<<' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testRightShiftOperator) {
      const v_i32_4 given{2, 4, 8, 16};
      // _mm_sra_epi32 reads the shift count from the low 64-bit element
      const v_i32_4 shift{1, 0, 0, 0};
      const v_i32_4 expected{1, 2, 4, 8};
      Assert::IsTrue(allTrue(expected == (given >> shift)), L"'>>' op value mismatch", LINE_INFO());
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
  };
}
