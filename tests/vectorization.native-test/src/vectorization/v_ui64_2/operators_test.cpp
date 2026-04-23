#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_ui64_2_OperatorsTest) {
  public:
    TEST_METHOD(testOnesComplementOperator) {
      const auto actual = ~MaskAll<v_ui64_2>();
      Assert::IsTrue(allTrue(Zero<v_ui64_2>() == actual), L"'~' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testLogicalNegationOperator) {
      const v_ui64_2 given{UInt_64{0}, UInt_64{1}};
      const auto actual = !given;
      Assert::AreEqual(MaskAll<v_ui64_2::BoolType>(), x(actual), L"'!' op value mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{0}, y(actual), L"'!' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testAddVectorOperator) {
      const v_ui64_2 a{UInt_64{1}, UInt_64{2}};
      const v_ui64_2 b{UInt_64{10}, UInt_64{20}};
      const v_ui64_2 expected{UInt_64{11}, UInt_64{22}};
      Assert::IsTrue(allTrue(expected == (a + b)), L"'+' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testSubtractVectorOperator) {
      const v_ui64_2 a{UInt_64{10}, UInt_64{20}};
      const v_ui64_2 b{UInt_64{1}, UInt_64{2}};
      const v_ui64_2 expected{UInt_64{9}, UInt_64{18}};
      Assert::IsTrue(allTrue(expected == (a - b)), L"'-' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testMultiplyVectorOperator) {
      const v_ui64_2 a{UInt_64{3}, UInt_64{4}};
      const v_ui64_2 b{UInt_64{10}, UInt_64{20}};
      const v_ui64_2 expected{UInt_64{30}, UInt_64{80}};
      Assert::IsTrue(allTrue(expected == (a * b)), L"'*' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testDivideVectorOperator) {
      const v_ui64_2 a{UInt_64{20}, UInt_64{30}};
      const v_ui64_2 b{UInt_64{5}, UInt_64{10}};
      const v_ui64_2 expected{UInt_64{4}, UInt_64{3}};
      Assert::IsTrue(allTrue(expected == (a / b)), L"'/' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testModuloVectorOperator) {
      const v_ui64_2 a{UInt_64{10}, UInt_64{21}};
      const v_ui64_2 b{UInt_64{3}, UInt_64{5}};
      const v_ui64_2 expected{UInt_64{1}, UInt_64{1}};
      Assert::IsTrue(allTrue(expected == (a % b)), L"'%' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testBitwiseAndOperator) {
      const auto a = MaskAll<v_ui64_2>();
      const auto b = MaskX<v_ui64_2>();
      const auto actual = a & b;
      Assert::IsTrue(allTrue(b == actual), L"'&' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testBitwiseOrOperator) {
      const auto a = MaskX<v_ui64_2>();
      const auto b = MaskY<v_ui64_2>();
      const auto expected = MaskXY<v_ui64_2>();
      const auto actual = a | b;
      Assert::IsTrue(allTrue(expected == actual), L"'|' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testBitwiseXorOperator) {
      const auto a = MaskAll<v_ui64_2>();
      const auto actual = a ^ a;
      Assert::IsTrue(allTrue(Zero<v_ui64_2>() == actual), L"'^' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testLeftShiftOperator) {
      const v_ui64_2 given{UInt_64{1}, UInt_64{2}};
      const v_ui64_2 shift{UInt_64{1}};
      const v_ui64_2 expected{UInt_64{2}, UInt_64{4}};
      Assert::IsTrue(allTrue(expected == (given << shift)), L"'<<' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testRightShiftOperator) {
      const v_ui64_2 given{UInt_64{4}, UInt_64{8}};
      const v_ui64_2 shift{UInt_64{1}};
      const v_ui64_2 expected{UInt_64{2}, UInt_64{4}};
      Assert::IsTrue(allTrue(expected == (given >> shift)), L"'>>' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testEqualsOperator) {
      const v_ui64_2 a{UInt_64{1}, UInt_64{2}};
      const v_ui64_2 b{UInt_64{1}, UInt_64{9}};
      const auto actual = a == b;

      Assert::AreEqual(MaskAll<v_ui64_2::BoolType>(), x(actual), L"Equals value mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{0}, y(actual), L"Equals value mismatch", LINE_INFO());
    }

    TEST_METHOD(testNotEqualsOperator) {
      const v_ui64_2 a{UInt_64{1}, UInt_64{2}};
      const v_ui64_2 b{UInt_64{1}, UInt_64{9}};
      const auto actual = a != b;

      Assert::AreEqual(UInt_64{0}, x(actual), L"NotEquals value mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_ui64_2::BoolType>(), y(actual), L"NotEquals value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundAddAssignOperator) {
      v_ui64_2 actual{UInt_64{1}, UInt_64{2}};
      actual += v_ui64_2{UInt_64{10}, UInt_64{20}};
      const v_ui64_2 expected{UInt_64{11}, UInt_64{22}};
      Assert::IsTrue(allTrue(expected == actual), L"'+=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testLessThanOperator) {
      const v_ui64_2 a{UInt_64{10}, UInt_64{20}};
      const v_ui64_2 b{UInt_64{20}, UInt_64{20}};
      const auto actual = a < b;
      Assert::AreEqual(static_cast<UInt_64>(-1), x(actual), L"'<' op X mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{0}, y(actual), L"'<' op Y mismatch", LINE_INFO());
    }

    TEST_METHOD(testGreaterThanOperator) {
      const v_ui64_2 a{UInt_64{10}, UInt_64{20}};
      const v_ui64_2 b{UInt_64{20}, UInt_64{20}};
      const auto actual = a > b;
      Assert::AreEqual(UInt_64{0}, x(actual), L"'>' op X mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{0}, y(actual), L"'>' op Y mismatch", LINE_INFO());
    }

    TEST_METHOD(testLessThanOrEqualOperator) {
      const v_ui64_2 a{UInt_64{10}, UInt_64{20}};
      const v_ui64_2 b{UInt_64{20}, UInt_64{20}};
      const auto actual = a <= b;
      Assert::AreEqual(static_cast<UInt_64>(-1), x(actual), L"'<=' op X mismatch", LINE_INFO());
      Assert::AreEqual(static_cast<UInt_64>(-1), y(actual), L"'<=' op Y mismatch", LINE_INFO());
    }

    TEST_METHOD(testGreaterThanOrEqualOperator) {
      const v_ui64_2 a{UInt_64{10}, UInt_64{20}};
      const v_ui64_2 b{UInt_64{20}, UInt_64{20}};
      const auto actual = a >= b;
      Assert::AreEqual(UInt_64{0}, x(actual), L"'>=' op X mismatch", LINE_INFO());
      Assert::AreEqual(static_cast<UInt_64>(-1), y(actual), L"'>=' op Y mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundSubtractAssignOperator) {
      const v_ui64_2 b{UInt_64{3}, UInt_64{7}};
      v_ui64_2 actual{UInt_64{100}, UInt_64{200}};
      const v_ui64_2 expected = actual - b;
      actual -= b;
      Assert::IsTrue(allTrue(expected == actual), L"'-=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundMultiplyAssignOperator) {
      const v_ui64_2 b{UInt_64{3}, UInt_64{7}};
      v_ui64_2 actual{UInt_64{100}, UInt_64{200}};
      const v_ui64_2 expected = actual * b;
      actual *= b;
      Assert::IsTrue(allTrue(expected == actual), L"'*=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundDivideAssignOperator) {
      const v_ui64_2 b{UInt_64{3}, UInt_64{7}};
      v_ui64_2 actual{UInt_64{100}, UInt_64{200}};
      const v_ui64_2 expected = actual / b;
      actual /= b;
      Assert::IsTrue(allTrue(expected == actual), L"'/=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundModuloAssignOperator) {
      const v_ui64_2 b{UInt_64{3}, UInt_64{7}};
      v_ui64_2 actual{UInt_64{100}, UInt_64{200}};
      const v_ui64_2 expected = actual % b;
      actual %= b;
      Assert::IsTrue(allTrue(expected == actual), L"'%=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundBitwiseAndAssignOperator) {
      const auto b = MaskX<v_ui64_2>();
      auto actual = MaskAll<v_ui64_2>();
      const auto expected = actual & b;
      actual &= b;
      Assert::IsTrue(allTrue(expected == actual), L"'&=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundBitwiseOrAssignOperator) {
      const auto b = MaskY<v_ui64_2>();
      auto actual = MaskX<v_ui64_2>();
      const auto expected = actual | b;
      actual |= b;
      Assert::IsTrue(allTrue(expected == actual), L"'|=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundBitwiseXorAssignOperator) {
      const auto b = MaskAll<v_ui64_2>();
      auto actual = MaskAll<v_ui64_2>();
      const auto expected = actual ^ b;
      actual ^= b;
      Assert::IsTrue(allTrue(expected == actual), L"'^=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundLeftShiftAssignOperator) {
      const v_ui64_2 b{UInt_64{2}};
      v_ui64_2 actual{UInt_64{1}, UInt_64{3}};
      const v_ui64_2 expected = actual << b;
      actual <<= b;
      Assert::IsTrue(allTrue(expected == actual), L"'<<=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundRightShiftAssignOperator) {
      const v_ui64_2 b{UInt_64{1}};
      v_ui64_2 actual{UInt_64{8}, UInt_64{16}};
      const v_ui64_2 expected = actual >> b;
      actual >>= b;
      Assert::IsTrue(allTrue(expected == actual), L"'>>=' op value mismatch", LINE_INFO());
    }
  };
}
