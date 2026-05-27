#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_ui64_4_OperatorsTest) {
  public:
    TEST_METHOD(testOnesComplementOperator) {
      const v_ui64_4 given{UInt_64{0}};
      const auto actual = ~given;
      Assert::AreEqual(std::numeric_limits<UInt_64>::max(), x(actual), L"'~' op X mismatch", LINE_INFO());
      Assert::AreEqual(std::numeric_limits<UInt_64>::max(), y(actual), L"'~' op Y mismatch", LINE_INFO());
      Assert::AreEqual(std::numeric_limits<UInt_64>::max(), z(actual), L"'~' op Z mismatch", LINE_INFO());
      Assert::AreEqual(std::numeric_limits<UInt_64>::max(), w(actual), L"'~' op W mismatch", LINE_INFO());
    }

    TEST_METHOD(testLogicalNegationOperator) {
      const v_ui64_4 given{0ULL, 10ULL, 0ULL, 1ULL};
      const auto actual = !given;
      Assert::AreEqual(std::numeric_limits<UInt_64>::max(), x(actual), L"'!' op X mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{0}, y(actual), L"'!' op Y mismatch", LINE_INFO());
      Assert::AreEqual(std::numeric_limits<UInt_64>::max(), z(actual), L"'!' op Z mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{0}, w(actual), L"'!' op W mismatch", LINE_INFO());
    }

    TEST_METHOD(testAddVectorOperator) {
      const v_ui64_4 a{10ULL, 20ULL, 30ULL, 40ULL};
      const v_ui64_4 b{1ULL, 2ULL, 3ULL, 4ULL};
      const v_ui64_4 expected{11ULL, 22ULL, 33ULL, 44ULL};
      Assert::IsTrue(allTrue(expected == (a + b)), L"'+' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testSubtractVectorOperator) {
      const v_ui64_4 a{10ULL, 20ULL, 30ULL, 40ULL};
      const v_ui64_4 b{1ULL, 2ULL, 3ULL, 4ULL};
      const v_ui64_4 expected{9ULL, 18ULL, 27ULL, 36ULL};
      Assert::IsTrue(allTrue(expected == (a - b)), L"'-' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testBitwiseAndOperator) {
      const v_ui64_4 a{std::numeric_limits<UInt_64>::max()};
      const v_ui64_4 b{10ULL, 20ULL, 30ULL, 40ULL};
      const auto actual = a & b;
      Assert::IsTrue(allTrue(b == actual), L"'&' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testBitwiseOrOperator) {
      const v_ui64_4 a{10ULL, 0ULL, 0ULL, 0ULL};
      const v_ui64_4 b{0ULL, 20ULL, 0ULL, 0ULL};
      const v_ui64_4 expected{10ULL, 20ULL, 0ULL, 0ULL};
      const auto actual = a | b;
      Assert::IsTrue(allTrue(expected == actual), L"'|' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testBitwiseXorOperator) {
      const v_ui64_4 a{10ULL, 20ULL, 30ULL, 40ULL};
      const auto actual = a ^ a;
      Assert::IsTrue(allTrue(Zero<v_ui64_4>() == actual), L"'^' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testLeftShiftOperator) {
      const v_ui64_4 given{1ULL, 2ULL, 4ULL, 8ULL};
      const v_ui64_4 shift{0ULL, 1ULL, 2ULL, 3ULL};
      const v_ui64_4 expected{1ULL, 4ULL, 16ULL, 64ULL};
      Assert::IsTrue(allTrue(expected == (given << shift)), L"'<<' op per-lane mismatch", LINE_INFO());
    }

    TEST_METHOD(testLeftShiftScalarOperator) {
      const v_ui64_4 given{1ULL, 2ULL, 4ULL, 8ULL};
      const v_ui64_4 expected{2ULL, 4ULL, 8ULL, 16ULL};
      Assert::IsTrue(allTrue(expected == (given << Int_64{1})), L"'<<' signed scalar mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(expected == (given << UInt_64{1})), L"'<<' unsigned scalar mismatch", LINE_INFO());
    }

    TEST_METHOD(testLeftShiftCrossSignednessOperator) {
      const v_ui64_4 given{1ULL, 2ULL, 4ULL, 8ULL};
      const v_i64_4 shift{0LL, 1LL, 2LL, 3LL};
      const v_ui64_4 expected{1ULL, 4ULL, 16ULL, 64ULL};
      Assert::IsTrue(allTrue(expected == (given << shift)), L"'<<' cross-sign mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundLeftShiftAssignOperator) {
      v_ui64_4 actual{1ULL, 2ULL, 4ULL, 8ULL};
      const v_ui64_4 shift{0ULL, 1ULL, 2ULL, 3ULL};
      const auto expected = actual << shift;
      actual <<= shift;
      Assert::IsTrue(allTrue(expected == actual), L"'<<=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testLogicalRightShiftOperator) {
      const v_ui64_4 a{16ULL, 32ULL, 64ULL, 128ULL};
      const v_ui64_4 b{1ULL, 2ULL, 3ULL, 4ULL};
      const v_ui64_4 expected{8ULL, 8ULL, 8ULL, 8ULL};
      Assert::IsTrue(allTrue(expected == (a >> b)), L"'>>' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testLogicalRightShiftHighBit) {
      const v_ui64_4 a{std::numeric_limits<UInt_64>::max()};
      const v_ui64_4 b{UInt_64{1}};
      const v_ui64_4 expected{std::numeric_limits<UInt_64>::max() >> 1};
      Assert::IsTrue(allTrue(expected == (a >> b)), L"'>>' high bit should not sign-extend", LINE_INFO());
    }

    TEST_METHOD(testRightShiftScalarOperator) {
      const v_ui64_4 given{16ULL, 32ULL, 64ULL, 128ULL};
      const v_ui64_4 expected{8ULL, 16ULL, 32ULL, 64ULL};
      Assert::IsTrue(allTrue(expected == (given >> Int_64{1})), L"'>>' signed scalar mismatch", LINE_INFO());
      Assert::IsTrue(allTrue(expected == (given >> UInt_64{1})), L"'>>' unsigned scalar mismatch", LINE_INFO());
    }

    TEST_METHOD(testRightShiftCrossSignednessOperator) {
      const v_ui64_4 given{16ULL, 32ULL, 64ULL, 128ULL};
      const v_i64_4 shift{1LL, 2LL, 3LL, 4LL};
      const v_ui64_4 expected{8ULL, 8ULL, 8ULL, 8ULL};
      Assert::IsTrue(allTrue(expected == (given >> shift)), L"'>>' cross-sign mismatch", LINE_INFO());
    }

    TEST_METHOD(testEqualsOperator) {
      const v_ui64_4 a{10ULL, 20ULL, 30ULL, 40ULL};
      const v_ui64_4 b{10ULL, 99ULL, 30ULL, 99ULL};
      const auto actual = a == b;

      Assert::AreEqual(std::numeric_limits<UInt_64>::max(), x(actual), L"Equals X mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{0}, y(actual), L"Equals Y mismatch", LINE_INFO());
      Assert::AreEqual(std::numeric_limits<UInt_64>::max(), z(actual), L"Equals Z mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{0}, w(actual), L"Equals W mismatch", LINE_INFO());
    }

    TEST_METHOD(testNotEqualsOperator) {
      const v_ui64_4 a{10ULL, 20ULL, 30ULL, 40ULL};
      const v_ui64_4 b{10ULL, 99ULL, 30ULL, 99ULL};
      const auto actual = a != b;

      Assert::AreEqual(UInt_64{0}, x(actual), L"NotEquals X mismatch", LINE_INFO());
      Assert::AreEqual(std::numeric_limits<UInt_64>::max(), y(actual), L"NotEquals Y mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{0}, z(actual), L"NotEquals Z mismatch", LINE_INFO());
      Assert::AreEqual(std::numeric_limits<UInt_64>::max(), w(actual), L"NotEquals W mismatch", LINE_INFO());
    }

    TEST_METHOD(testLessThanOperator) {
      const v_ui64_4 a{10ULL, 50ULL, 30ULL, 40ULL};
      const v_ui64_4 b{20ULL, 50ULL, 10ULL, 90ULL};
      const auto actual = a < b;

      Assert::AreEqual(std::numeric_limits<UInt_64>::max(), x(actual), L"'<' X mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{0}, y(actual), L"'<' Y mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{0}, z(actual), L"'<' Z mismatch", LINE_INFO());
      Assert::AreEqual(std::numeric_limits<UInt_64>::max(), w(actual), L"'<' W mismatch", LINE_INFO());
    }

    TEST_METHOD(testLessThanUnsignedSemantics) {
      const v_ui64_4 a{0ULL, 1ULL, std::numeric_limits<UInt_64>::max(), 0ULL};
      const v_ui64_4 b{1ULL, 0ULL, 0ULL, std::numeric_limits<UInt_64>::max()};
      const auto actual = a < b;

      Assert::AreEqual(std::numeric_limits<UInt_64>::max(), x(actual), L"0 < 1 should be true", LINE_INFO());
      Assert::AreEqual(UInt_64{0}, y(actual), L"1 < 0 should be false", LINE_INFO());
      Assert::AreEqual(UInt_64{0}, z(actual), L"MAX < 0 should be false (unsigned)", LINE_INFO());
      Assert::AreEqual(std::numeric_limits<UInt_64>::max(), w(actual), L"0 < MAX should be true (unsigned)", LINE_INFO());
    }

    TEST_METHOD(testGreaterThanOperator) {
      const v_ui64_4 a{20ULL, 50ULL, 10ULL, 90ULL};
      const v_ui64_4 b{10ULL, 50ULL, 30ULL, 40ULL};
      const auto actual = a > b;

      Assert::AreEqual(std::numeric_limits<UInt_64>::max(), x(actual), L"'>' X mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{0}, y(actual), L"'>' Y mismatch", LINE_INFO());
      Assert::AreEqual(UInt_64{0}, z(actual), L"'>' Z mismatch", LINE_INFO());
      Assert::AreEqual(std::numeric_limits<UInt_64>::max(), w(actual), L"'>' W mismatch", LINE_INFO());
    }

    TEST_METHOD(testGreaterThanUnsignedSemantics) {
      const v_ui64_4 a{std::numeric_limits<UInt_64>::max(), 0ULL, 1ULL, 0ULL};
      const v_ui64_4 b{0ULL, std::numeric_limits<UInt_64>::max(), 0ULL, 1ULL};
      const auto actual = a > b;

      Assert::AreEqual(std::numeric_limits<UInt_64>::max(), x(actual), L"MAX > 0 should be true (unsigned)", LINE_INFO());
      Assert::AreEqual(UInt_64{0}, y(actual), L"0 > MAX should be false (unsigned)", LINE_INFO());
      Assert::AreEqual(std::numeric_limits<UInt_64>::max(), z(actual), L"1 > 0 should be true", LINE_INFO());
      Assert::AreEqual(UInt_64{0}, w(actual), L"0 > 1 should be false", LINE_INFO());
    }

    TEST_METHOD(testCompoundAddAssignOperator) {
      v_ui64_4 actual{10ULL, 20ULL, 30ULL, 40ULL};
      actual += v_ui64_4{1ULL, 2ULL, 3ULL, 4ULL};
      const v_ui64_4 expected{11ULL, 22ULL, 33ULL, 44ULL};
      Assert::IsTrue(allTrue(expected == actual), L"'+=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundSubtractAssignOperator) {
      v_ui64_4 actual{10ULL, 20ULL, 30ULL, 40ULL};
      actual -= v_ui64_4{1ULL, 2ULL, 3ULL, 4ULL};
      const v_ui64_4 expected{9ULL, 18ULL, 27ULL, 36ULL};
      Assert::IsTrue(allTrue(expected == actual), L"'-=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundBitwiseAndAssignOperator) {
      v_ui64_4 actual{std::numeric_limits<UInt_64>::max()};
      const v_ui64_4 b{10ULL, 20ULL, 30ULL, 40ULL};
      actual &= b;
      Assert::IsTrue(allTrue(b == actual), L"'&=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundBitwiseOrAssignOperator) {
      v_ui64_4 actual{10ULL, 20ULL, 30ULL, 40ULL};
      const v_ui64_4 b{1ULL, 2ULL, 3ULL, 4ULL};
      const auto expected = actual | b;
      actual |= b;
      Assert::IsTrue(allTrue(expected == actual), L"'|=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundBitwiseXorAssignOperator) {
      v_ui64_4 actual{10ULL, 20ULL, 30ULL, 40ULL};
      const v_ui64_4 b{1ULL, 2ULL, 3ULL, 4ULL};
      const auto expected = actual ^ b;
      actual ^= b;
      Assert::IsTrue(allTrue(expected == actual), L"'^=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundRightShiftAssignOperator) {
      v_ui64_4 actual{16ULL, 32ULL, 64ULL, 128ULL};
      const v_ui64_4 b{1ULL, 2ULL, 3ULL, 4ULL};
      const auto expected = actual >> b;
      actual >>= b;
      Assert::IsTrue(allTrue(expected == actual), L"'>>=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testOstreamOutput) {
      const v_ui64_4 given{10ULL, 20ULL, 30ULL, 40ULL};
      std::ostringstream stream;
      stream << given;
      const auto result = stream.str();
      Assert::IsFalse(result.empty(), L"ostream output should not be empty", LINE_INFO());
    }
  };
}
