#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_i32_8_OperatorsTest) {
  public:
    TEST_METHOD(testNegationOperator) {
      const v_i32_8 given{1, -2, 3, -4, 5, -6, 7, -8};
      const v_i32_8 expected{-1, 2, -3, 4, -5, 6, -7, 8};
      const auto actual = -given;
      Assert::IsTrue(allTrue(expected == actual), L"'-' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testOnesComplementOperator) {
      const v_i32_8 allOnes{Int_32{-1}};
      const auto actual = ~allOnes;
      Assert::IsTrue(allTrue(Zero<v_i32_8>() == actual), L"'~' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testLogicalNegationOperator) {
      const v_i32_8 given{0, 1, 0, -1, 0, 42, 0, -99};
      const auto actual = !given;
      Assert::AreEqual(Int_32{-1}, x1(actual), L"X1 mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, x2(actual), L"X2 mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{-1}, x3(actual), L"X3 mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, x4(actual), L"X4 mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{-1}, x5(actual), L"X5 mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, x6(actual), L"X6 mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{-1}, x7(actual), L"X7 mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, x8(actual), L"X8 mismatch", LINE_INFO());
    }

    TEST_METHOD(testAddVectorOperator) {
      const v_i32_8 a{1, 2, 3, 4, 5, 6, 7, 8};
      const v_i32_8 b{10, 20, 30, 40, 50, 60, 70, 80};
      const v_i32_8 expected{11, 22, 33, 44, 55, 66, 77, 88};
      Assert::IsTrue(allTrue(expected == (a + b)), L"'+' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testSubtractVectorOperator) {
      const v_i32_8 a{10, 20, 30, 40, 50, 60, 70, 80};
      const v_i32_8 b{1, 2, 3, 4, 5, 6, 7, 8};
      const v_i32_8 expected{9, 18, 27, 36, 45, 54, 63, 72};
      Assert::IsTrue(allTrue(expected == (a - b)), L"'-' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testMultiplyVectorOperator) {
      const v_i32_8 a{2, 3, 4, 5, 6, 7, 8, 9};
      const v_i32_8 b{10, 20, 30, 40, 50, 60, 70, 80};
      const v_i32_8 expected{20, 60, 120, 200, 300, 420, 560, 720};
      Assert::IsTrue(allTrue(expected == (a * b)), L"'*' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testBitwiseAndOperator) {
      const v_i32_8 allOnes{Int_32{-1}};
      const v_i32_8 pattern{1, 0, 1, 0, 1, 0, 1, 0};
      const auto actual = allOnes & pattern;
      Assert::IsTrue(allTrue(pattern == actual), L"'&' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testBitwiseOrOperator) {
      const v_i32_8 a{1, 0, 0, 0, 0, 0, 0, 0};
      const v_i32_8 b{0, 0, 0, 0, 0, 0, 0, 1};
      const v_i32_8 expected{1, 0, 0, 0, 0, 0, 0, 1};
      Assert::IsTrue(allTrue(expected == (a | b)), L"'|' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testBitwiseXorOperator) {
      const v_i32_8 a{Int_32{-1}};
      const auto actual = a ^ a;
      Assert::IsTrue(allTrue(Zero<v_i32_8>() == actual), L"'^' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testLeftShiftOperator) {
      const v_i32_8 given{1, 2, 4, 8, 16, 32, 64, 128};
      const v_i32_8 shift{1, 2, 3, 4, 0, 1, 2, 3};
      const v_i32_8 expected{2, 8, 32, 128, 16, 64, 256, 1024};
      Assert::IsTrue(allTrue(expected == (given << shift)), L"'<<' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testRightShiftOperator) {
      const v_i32_8 given{16, 32, 64, 128, 256, 512, 1024, 2048};
      const v_i32_8 shift{1, 2, 3, 4, 0, 1, 2, 3};
      const v_i32_8 expected{8, 8, 8, 8, 256, 256, 256, 256};
      Assert::IsTrue(allTrue(expected == (given >> shift)), L"'>>' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testLessThanOperator) {
      const v_i32_8 a{1, 5, 3, 4, 1, 5, 3, 4};
      const v_i32_8 b{2, 5, 1, 9, 2, 5, 1, 9};
      const auto actual = a < b;
      Assert::AreEqual(Int_32{-1}, x1(actual), L"X1 mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, x2(actual), L"X2 mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, x3(actual), L"X3 mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{-1}, x4(actual), L"X4 mismatch", LINE_INFO());
    }

    TEST_METHOD(testGreaterThanOperator) {
      const v_i32_8 a{2, 5, 1, 9, 2, 5, 1, 9};
      const v_i32_8 b{1, 5, 3, 4, 1, 5, 3, 4};
      const auto actual = a > b;
      Assert::AreEqual(Int_32{-1}, x1(actual), L"X1 mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, x2(actual), L"X2 mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, x3(actual), L"X3 mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{-1}, x4(actual), L"X4 mismatch", LINE_INFO());
    }

    TEST_METHOD(testEqualsOperator) {
      const v_i32_8 a{1, 2, 3, 4, 5, 6, 7, 8};
      const v_i32_8 b{1, 9, 3, 9, 5, 9, 7, 9};
      const auto actual = a == b;
      Assert::AreEqual(Int_32{-1}, x1(actual), L"X1 mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, x2(actual), L"X2 mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{-1}, x3(actual), L"X3 mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, x4(actual), L"X4 mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{-1}, x5(actual), L"X5 mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, x6(actual), L"X6 mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{-1}, x7(actual), L"X7 mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, x8(actual), L"X8 mismatch", LINE_INFO());
    }

    TEST_METHOD(testNotEqualsOperator) {
      const v_i32_8 a{1, 2, 3, 4, 5, 6, 7, 8};
      const v_i32_8 b{1, 9, 3, 9, 5, 9, 7, 9};
      const auto actual = a != b;
      Assert::AreEqual(Int_32{0}, x1(actual), L"X1 mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{-1}, x2(actual), L"X2 mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{0}, x3(actual), L"X3 mismatch", LINE_INFO());
      Assert::AreEqual(Int_32{-1}, x4(actual), L"X4 mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundAssignOperators) {
      {
        v_i32_8 a{1, 2, 3, 4, 5, 6, 7, 8};
        const v_i32_8 b{10, 20, 30, 40, 50, 60, 70, 80};
        const auto expected = a + b;
        a += b;
        Assert::IsTrue(allTrue(expected == a), L"'+=' mismatch", LINE_INFO());
      }

      {
        v_i32_8 a{10, 20, 30, 40, 50, 60, 70, 80};
        const v_i32_8 b{1, 2, 3, 4, 5, 6, 7, 8};
        const auto expected = a - b;
        a -= b;
        Assert::IsTrue(allTrue(expected == a), L"'-=' mismatch", LINE_INFO());
      }

      {
        v_i32_8 a{2, 3, 4, 5, 6, 7, 8, 9};
        const v_i32_8 b{10, 20, 30, 40, 50, 60, 70, 80};
        const auto expected = a * b;
        a *= b;
        Assert::IsTrue(allTrue(expected == a), L"'*=' mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(testCompoundBitwiseAssignOperators) {
      {
        v_i32_8 a{Int_32{-1}};
        const v_i32_8 b{1, 0, 1, 0, 1, 0, 1, 0};
        const auto expected = a & b;
        a &= b;
        Assert::IsTrue(allTrue(expected == a), L"'&=' mismatch", LINE_INFO());
      }

      {
        v_i32_8 a{1, 0, 0, 0, 0, 0, 0, 0};
        const v_i32_8 b{0, 1, 0, 0, 0, 0, 0, 0};
        const auto expected = a | b;
        a |= b;
        Assert::IsTrue(allTrue(expected == a), L"'|=' mismatch", LINE_INFO());
      }

      {
        v_i32_8 a{1, 2, 3, 4, 5, 6, 7, 8};
        const v_i32_8 b{1, 2, 3, 4, 5, 6, 7, 8};
        const auto expected = a ^ b;
        a ^= b;
        Assert::IsTrue(allTrue(expected == a), L"'^=' mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(testCompoundShiftAssignOperators) {
      {
        v_i32_8 a{1, 2, 4, 8, 16, 32, 64, 128};
        const v_i32_8 b{1, 2, 3, 4, 0, 1, 2, 3};
        const auto expected = a << b;
        a <<= b;
        Assert::IsTrue(allTrue(expected == a), L"'<<=' mismatch", LINE_INFO());
      }

      {
        v_i32_8 a{16, 32, 64, 128, 256, 512, 1024, 2048};
        const v_i32_8 b{1, 2, 3, 4, 0, 1, 2, 3};
        const auto expected = a >> b;
        a >>= b;
        Assert::IsTrue(allTrue(expected == a), L"'>>=' mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(testRightShiftIsArithmetic) {
      const v_i32_8 given{Int_32{-2}};
      const v_i32_8 shift{Int_32{1}};
      const v_i32_8 expected{Int_32{-1}};
      Assert::IsTrue(allTrue(expected == (given >> shift)), L"arithmetic shift must sign-extend", LINE_INFO());
    }

    TEST_METHOD(testLessThanWithNegatives) {
      const v_i32_8 a{-5, 0, 5, Int_32(0x80000000), -1, 1, Int_32(0x7FFFFFFF), 0};
      const v_i32_8 b{-1, 0, -5, 0, 0, -1, Int_32(0x80000000), 0};
      const auto actual = a < b;
      Assert::AreEqual(Int_32{-1}, x1(actual), L"-5 < -1 true", LINE_INFO());
      Assert::AreEqual(Int_32{0}, x2(actual), L"0 < 0 false", LINE_INFO());
      Assert::AreEqual(Int_32{0}, x3(actual), L"5 < -5 false", LINE_INFO());
      Assert::AreEqual(Int_32{-1}, x4(actual), L"INT_MIN < 0 true", LINE_INFO());
      Assert::AreEqual(Int_32{-1}, x5(actual), L"-1 < 0 true", LINE_INFO());
      Assert::AreEqual(Int_32{0}, x6(actual), L"1 < -1 false", LINE_INFO());
      Assert::AreEqual(Int_32{0}, x7(actual), L"INT_MAX < INT_MIN false", LINE_INFO());
      Assert::AreEqual(Int_32{0}, x8(actual), L"0 < 0 false", LINE_INFO());
    }

    TEST_METHOD(testGreaterThanWithNegatives) {
      const v_i32_8 a{-1, 0, -5, 0, 0, -1, Int_32(0x80000000), 0};
      const v_i32_8 b{-5, 0, 5, Int_32(0x80000000), -1, 1, Int_32(0x7FFFFFFF), 0};
      const auto actual = a > b;
      Assert::AreEqual(Int_32{-1}, x1(actual), L"-1 > -5 true", LINE_INFO());
      Assert::AreEqual(Int_32{0}, x2(actual), L"0 > 0 false", LINE_INFO());
      Assert::AreEqual(Int_32{0}, x3(actual), L"-5 > 5 false", LINE_INFO());
      Assert::AreEqual(Int_32{-1}, x4(actual), L"0 > INT_MIN true", LINE_INFO());
      Assert::AreEqual(Int_32{-1}, x5(actual), L"0 > -1 true", LINE_INFO());
      Assert::AreEqual(Int_32{0}, x6(actual), L"-1 > 1 false", LINE_INFO());
      Assert::AreEqual(Int_32{0}, x7(actual), L"INT_MIN > INT_MAX false", LINE_INFO());
      Assert::AreEqual(Int_32{0}, x8(actual), L"0 > 0 false", LINE_INFO());
    }
  };
}
