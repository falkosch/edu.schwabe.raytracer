#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_i64_4_OperatorsTest) {
  public:
    TEST_METHOD(testNegationOperator) {
      const v_i64_4 given{10LL, -20LL, 30LL, -40LL};
      const v_i64_4 expected{-10LL, 20LL, -30LL, 40LL};
      const auto actual = -given;
      Assert::IsTrue(allTrue(expected == actual), L"'-' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testOnesComplementOperator) {
      const auto actual = ~NegativeOne<v_i64_4>();
      Assert::IsTrue(allTrue(Zero<v_i64_4>() == actual), L"'~' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testLogicalNegationOperator) {
      const v_i64_4 given{0LL, 10LL, 0LL, -1LL};
      const auto actual = !given;
      Assert::AreEqual(MaskAll<v_i64_4::BoolType>(), x(actual), L"'!' op X mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{0}, y(actual), L"'!' op Y mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_i64_4::BoolType>(), z(actual), L"'!' op Z mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{0}, w(actual), L"'!' op W mismatch", LINE_INFO());
    }

    TEST_METHOD(testAddVectorOperator) {
      const v_i64_4 a{10LL, 20LL, 30LL, 40LL};
      const v_i64_4 b{1LL, 2LL, 3LL, 4LL};
      const v_i64_4 expected{11LL, 22LL, 33LL, 44LL};
      Assert::IsTrue(allTrue(expected == (a + b)), L"'+' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testSubtractVectorOperator) {
      const v_i64_4 a{10LL, 20LL, 30LL, 40LL};
      const v_i64_4 b{1LL, 2LL, 3LL, 4LL};
      const v_i64_4 expected{9LL, 18LL, 27LL, 36LL};
      Assert::IsTrue(allTrue(expected == (a - b)), L"'-' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testBitwiseAndOperator) {
      const v_i64_4 a{Int_64{-1}};
      const v_i64_4 b{10LL, 20LL, 30LL, 40LL};
      const auto actual = a & b;
      Assert::IsTrue(allTrue(b == actual), L"'&' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testBitwiseOrOperator) {
      const v_i64_4 a{10LL, 0LL, 0LL, 0LL};
      const v_i64_4 b{0LL, 20LL, 0LL, 0LL};
      const v_i64_4 expected{10LL, 20LL, 0LL, 0LL};
      const auto actual = a | b;
      Assert::IsTrue(allTrue(expected == actual), L"'|' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testBitwiseXorOperator) {
      const v_i64_4 a{10LL, 20LL, 30LL, 40LL};
      const auto actual = a ^ a;
      Assert::IsTrue(allTrue(Zero<v_i64_4>() == actual), L"'^' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testEqualsOperator) {
      const v_i64_4 a{10LL, 20LL, 30LL, 40LL};
      const v_i64_4 b{10LL, 99LL, 30LL, 99LL};
      const auto actual = a == b;

      Assert::AreEqual(MaskAll<v_i64_4::BoolType>(), x(actual), L"Equals X mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{0}, y(actual), L"Equals Y mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_i64_4::BoolType>(), z(actual), L"Equals Z mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{0}, w(actual), L"Equals W mismatch", LINE_INFO());
    }

    TEST_METHOD(testNotEqualsOperator) {
      const v_i64_4 a{10LL, 20LL, 30LL, 40LL};
      const v_i64_4 b{10LL, 99LL, 30LL, 99LL};
      const auto actual = a != b;

      Assert::AreEqual(Int_64{0}, x(actual), L"NotEquals X mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_i64_4::BoolType>(), y(actual), L"NotEquals Y mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{0}, z(actual), L"NotEquals Z mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_i64_4::BoolType>(), w(actual), L"NotEquals W mismatch", LINE_INFO());
    }

    TEST_METHOD(testLessThanOperator) {
      const v_i64_4 a{10LL, 50LL, 30LL, 40LL};
      const v_i64_4 b{20LL, 50LL, 10LL, 90LL};
      const auto actual = a < b;

      Assert::AreEqual(MaskAll<v_i64_4::BoolType>(), x(actual), L"'<' X mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{0}, y(actual), L"'<' Y mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{0}, z(actual), L"'<' Z mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_i64_4::BoolType>(), w(actual), L"'<' W mismatch", LINE_INFO());
    }

    TEST_METHOD(testGreaterThanOperator) {
      const v_i64_4 a{20LL, 50LL, 10LL, 90LL};
      const v_i64_4 b{10LL, 50LL, 30LL, 40LL};
      const auto actual = a > b;

      Assert::AreEqual(MaskAll<v_i64_4::BoolType>(), x(actual), L"'>' X mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{0}, y(actual), L"'>' Y mismatch", LINE_INFO());
      Assert::AreEqual(Int_64{0}, z(actual), L"'>' Z mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_i64_4::BoolType>(), w(actual), L"'>' W mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundAddAssignOperator) {
      v_i64_4 actual{10LL, 20LL, 30LL, 40LL};
      actual += v_i64_4{1LL, 2LL, 3LL, 4LL};
      const v_i64_4 expected{11LL, 22LL, 33LL, 44LL};
      Assert::IsTrue(allTrue(expected == actual), L"'+=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundSubtractAssignOperator) {
      v_i64_4 actual{10LL, 20LL, 30LL, 40LL};
      actual -= v_i64_4{1LL, 2LL, 3LL, 4LL};
      const v_i64_4 expected{9LL, 18LL, 27LL, 36LL};
      Assert::IsTrue(allTrue(expected == actual), L"'-=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundBitwiseAndAssignOperator) {
      v_i64_4 actual{Int_64{-1}};
      const v_i64_4 b{10LL, 20LL, 30LL, 40LL};
      actual &= b;
      Assert::IsTrue(allTrue(b == actual), L"'&=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundBitwiseOrAssignOperator) {
      v_i64_4 actual{10LL, 20LL, 30LL, 40LL};
      const v_i64_4 b{1LL, 2LL, 3LL, 4LL};
      const auto expected = actual | b;
      actual |= b;
      Assert::IsTrue(allTrue(expected == actual), L"'|=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundBitwiseXorAssignOperator) {
      v_i64_4 actual{10LL, 20LL, 30LL, 40LL};
      const v_i64_4 b{1LL, 2LL, 3LL, 4LL};
      const auto expected = actual ^ b;
      actual ^= b;
      Assert::IsTrue(allTrue(expected == actual), L"'^=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testOstreamOutput) {
      const v_i64_4 given{10LL, 20LL, 30LL, 40LL};
      std::ostringstream stream;
      stream << given;
      const auto result = stream.str();
      Assert::IsFalse(result.empty(), L"ostream output should not be empty", LINE_INFO());
    }
  };
}
