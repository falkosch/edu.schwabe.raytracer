#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_i64_2_OperatorsTest) {
  public:
    TEST_METHOD(testNegationOperator) {
      const v_i64_2 given{Int_64{10LL}, Int_64{-5LL}};
      const v_i64_2 expected{Int_64{-10LL}, Int_64{5LL}};
      const auto actual = -given;
      Assert::IsTrue(allTrue(expected == actual), L"'-' unary op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testNegationOfZero) {
      const auto actual = -Zero<v_i64_2>();
      Assert::IsTrue(allTrue(Zero<v_i64_2>() == actual), L"negation of zero mismatch", LINE_INFO());
    }

    TEST_METHOD(testOnesComplementOperator) {
      const auto actual = ~MaskAll<v_i64_2>();
      Assert::IsTrue(allTrue(Zero<v_i64_2>() == actual), L"'~' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testLogicalNegationOperator) {
      const v_i64_2 given{Int_64{0LL}, Int_64{1LL}};
      const auto actual = !given;
      Assert::AreEqual(MaskAll<v_i64_2::BoolType>(), x(actual), L"'!' op value mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{0LL}, y(actual), L"'!' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testAddVectorOperator) {
      const v_i64_2 a{Int_64{10LL}, Int_64{-5LL}};
      const v_i64_2 b{Int_64{30LL}, Int_64{15LL}};
      const v_i64_2 expected{Int_64{40LL}, Int_64{10LL}};
      Assert::IsTrue(allTrue(expected == (a + b)), L"'+' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testSubtractVectorOperator) {
      const v_i64_2 a{Int_64{10LL}, Int_64{-5LL}};
      const v_i64_2 b{Int_64{30LL}, Int_64{15LL}};
      const v_i64_2 expected{Int_64{-20LL}, Int_64{-20LL}};
      Assert::IsTrue(allTrue(expected == (a - b)), L"'-' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testMultiplyVectorOperator) {
      const v_i64_2 a{Int_64{10LL}, Int_64{-5LL}};
      const v_i64_2 b{Int_64{3LL}, Int_64{4LL}};
      const v_i64_2 expected{Int_64{30LL}, Int_64{-20LL}};
      Assert::IsTrue(allTrue(expected == (a * b)), L"'*' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testDivideVectorOperator) {
      const v_i64_2 a{Int_64{30LL}, Int_64{-20LL}};
      const v_i64_2 b{Int_64{10LL}, Int_64{5LL}};
      const v_i64_2 expected{Int_64{3LL}, Int_64{-4LL}};
      Assert::IsTrue(allTrue(expected == (a / b)), L"'/' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testModuloVectorOperator) {
      const v_i64_2 a{Int_64{10LL}, Int_64{-7LL}};
      const v_i64_2 b{Int_64{3LL}, Int_64{5LL}};
      const v_i64_2 expected{Int_64{1LL}, Int_64{-2LL}};
      Assert::IsTrue(allTrue(expected == (a % b)), L"'%' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testBitwiseAndOperator) {
      const auto a = MaskAll<v_i64_2>();
      const auto b = MaskX<v_i64_2>();
      const auto actual = a & b;
      Assert::IsTrue(allTrue(b == actual), L"'&' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testBitwiseOrOperator) {
      const auto a = MaskX<v_i64_2>();
      const auto b = MaskY<v_i64_2>();
      const auto expected = MaskXY<v_i64_2>();
      const auto actual = a | b;
      Assert::IsTrue(allTrue(expected == actual), L"'|' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testBitwiseXorOperator) {
      const auto a = MaskAll<v_i64_2>();
      const auto actual = a ^ a;
      Assert::IsTrue(allTrue(Zero<v_i64_2>() == actual), L"'^' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testLeftShiftOperator) {
      const v_i64_2 given{Int_64{1LL}, Int_64{2LL}};
      const v_i64_2 shift{Int_64{1LL}};
      const v_i64_2 expected{Int_64{2LL}, Int_64{4LL}};
      Assert::IsTrue(allTrue(expected == (given << shift)), L"'<<' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testRightShiftOperator) {
      const v_i64_2 given{Int_64{8LL}, Int_64{16LL}};
      const v_i64_2 shift{Int_64{1LL}};
      const v_i64_2 expected{Int_64{4LL}, Int_64{8LL}};
      Assert::IsTrue(allTrue(expected == (given >> shift)), L"'>>' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testRightShiftArithmeticPreservesSign) {
      const v_i64_2 given{Int_64{-8LL}, Int_64{-16LL}};
      const v_i64_2 shift{Int_64{1LL}};
      const v_i64_2 expected{Int_64{-4LL}, Int_64{-8LL}};
      Assert::IsTrue(allTrue(expected == (given >> shift)), L"'>>' arithmetic shift sign mismatch", LINE_INFO());
    }

    TEST_METHOD(testSignedGreaterThan) {
      const v_i64_2 a{Int_64{10LL}, Int_64{-5LL}};
      const v_i64_2 b{Int_64{-1LL}, Int_64{-5LL}};
      const auto actual = a > b;
      Assert::AreEqual(MaskAll<v_i64_2::BoolType>(), x(actual), L"'>' op X mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{0LL}, y(actual), L"'>' op Y mismatch", LINE_INFO());
    }

    TEST_METHOD(testSignedLessThan) {
      const v_i64_2 a{Int_64{-5LL}, Int_64{10LL}};
      const v_i64_2 b{Int_64{10LL}, Int_64{10LL}};
      const auto actual = a < b;
      Assert::AreEqual(MaskAll<v_i64_2::BoolType>(), x(actual), L"'<' op X mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{0LL}, y(actual), L"'<' op Y mismatch", LINE_INFO());
    }

    TEST_METHOD(testSignedLessThanOrEqual) {
      const v_i64_2 a{Int_64{-5LL}, Int_64{10LL}};
      const v_i64_2 b{Int_64{10LL}, Int_64{10LL}};
      const auto actual = a <= b;
      Assert::AreEqual(MaskAll<v_i64_2::BoolType>(), x(actual), L"'<=' op X mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_i64_2::BoolType>(), y(actual), L"'<=' op Y mismatch", LINE_INFO());
    }

    TEST_METHOD(testSignedGreaterThanOrEqual) {
      const v_i64_2 a{Int_64{-5LL}, Int_64{10LL}};
      const v_i64_2 b{Int_64{10LL}, Int_64{10LL}};
      const auto actual = a >= b;
      Assert::AreEqual(Int_64{0LL}, x(actual), L"'>=' op X mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_i64_2::BoolType>(), y(actual), L"'>=' op Y mismatch", LINE_INFO());
    }

    TEST_METHOD(testEqualsOperator) {
      const v_i64_2 a{Int_64{10LL}, Int_64{-5LL}};
      const v_i64_2 b{Int_64{10LL}, Int_64{30LL}};
      const auto actual = a == b;

      Assert::AreEqual(MaskAll<v_i64_2::BoolType>(), x(actual), L"Equals value mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{0LL}, y(actual), L"Equals value mismatch", LINE_INFO());
    }

    TEST_METHOD(testNotEqualsOperator) {
      const v_i64_2 a{Int_64{10LL}, Int_64{-5LL}};
      const v_i64_2 b{Int_64{10LL}, Int_64{30LL}};
      const auto actual = a != b;

      Assert::AreEqual(Int_64{0LL}, x(actual), L"NotEquals value mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_i64_2::BoolType>(), y(actual), L"NotEquals value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundAddAssignOperator) {
      v_i64_2 actual{Int_64{10LL}, Int_64{-5LL}};
      actual += v_i64_2{Int_64{30LL}, Int_64{15LL}};
      const v_i64_2 expected{Int_64{40LL}, Int_64{10LL}};
      Assert::IsTrue(allTrue(expected == actual), L"'+=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundSubtractAssignOperator) {
      const v_i64_2 b{Int_64{3LL}, Int_64{7LL}};
      v_i64_2 actual{Int_64{10LL}, Int_64{-5LL}};
      const v_i64_2 expected = actual - b;
      actual -= b;
      Assert::IsTrue(allTrue(expected == actual), L"'-=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundMultiplyAssignOperator) {
      const v_i64_2 b{Int_64{3LL}, Int_64{-2LL}};
      v_i64_2 actual{Int_64{10LL}, Int_64{-5LL}};
      const v_i64_2 expected = actual * b;
      actual *= b;
      Assert::IsTrue(allTrue(expected == actual), L"'*=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundDivideAssignOperator) {
      const v_i64_2 b{Int_64{2LL}, Int_64{5LL}};
      v_i64_2 actual{Int_64{30LL}, Int_64{-10LL}};
      const v_i64_2 expected = actual / b;
      actual /= b;
      Assert::IsTrue(allTrue(expected == actual), L"'/=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundModuloAssignOperator) {
      const v_i64_2 b{Int_64{3LL}, Int_64{7LL}};
      v_i64_2 actual{Int_64{10LL}, Int_64{-5LL}};
      const v_i64_2 expected = actual % b;
      actual %= b;
      Assert::IsTrue(allTrue(expected == actual), L"'%=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundBitwiseAndAssignOperator) {
      const auto b = MaskX<v_i64_2>();
      auto actual = MaskAll<v_i64_2>();
      const auto expected = actual & b;
      actual &= b;
      Assert::IsTrue(allTrue(expected == actual), L"'&=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundBitwiseOrAssignOperator) {
      const auto b = MaskY<v_i64_2>();
      auto actual = MaskX<v_i64_2>();
      const auto expected = actual | b;
      actual |= b;
      Assert::IsTrue(allTrue(expected == actual), L"'|=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundBitwiseXorAssignOperator) {
      const auto b = MaskAll<v_i64_2>();
      auto actual = MaskAll<v_i64_2>();
      const auto expected = actual ^ b;
      actual ^= b;
      Assert::IsTrue(allTrue(expected == actual), L"'^=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundLeftShiftAssignOperator) {
      const v_i64_2 b{Int_64{2LL}};
      v_i64_2 actual{Int_64{1LL}, Int_64{3LL}};
      const v_i64_2 expected = actual << b;
      actual <<= b;
      Assert::IsTrue(allTrue(expected == actual), L"'<<=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundRightShiftAssignOperator) {
      const v_i64_2 b{Int_64{1LL}};
      v_i64_2 actual{Int_64{8LL}, Int_64{16LL}};
      const v_i64_2 expected = actual >> b;
      actual >>= b;
      Assert::IsTrue(allTrue(expected == actual), L"'>>=' op value mismatch", LINE_INFO());
    }
  };
}
