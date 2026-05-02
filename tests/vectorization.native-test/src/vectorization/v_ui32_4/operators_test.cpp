#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_ui32_4_OperatorsTest) {
  public:
    TEST_METHOD(testOnesComplementOperator) {
      const auto actual = ~MaskAll<v_ui32_4>();
      Assert::IsTrue(allTrue(Zero<v_ui32_4>() == actual), L"'~' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testLogicalNegationOperator) {
      const v_ui32_4 given{0u, 1u, 0u, 42u};
      const auto actual = !given;
      Assert::AreEqual(MaskAll<v_ui32_4::BoolType>(), x(actual), L"'!' op value mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, y(actual), L"'!' op value mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_ui32_4::BoolType>(), z(actual), L"'!' op value mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, w(actual), L"'!' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testAddVectorOperator) {
      const v_ui32_4 a{1u, 2u, 3u, 4u};
      const v_ui32_4 b{10u, 20u, 30u, 40u};
      const v_ui32_4 expected{11u, 22u, 33u, 44u};
      Assert::IsTrue(allTrue(expected == (a + b)), L"'+' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testSubtractVectorOperator) {
      const v_ui32_4 a{10u, 20u, 30u, 40u};
      const v_ui32_4 b{1u, 2u, 3u, 4u};
      const v_ui32_4 expected{9u, 18u, 27u, 36u};
      Assert::IsTrue(allTrue(expected == (a - b)), L"'-' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testMultiplyVectorOperator) {
      const v_ui32_4 a{2u, 3u, 4u, 5u};
      const v_ui32_4 b{10u, 20u, 30u, 40u};
      const v_ui32_4 expected{20u, 60u, 120u, 200u};
      Assert::IsTrue(allTrue(expected == (a * b)), L"'*' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testDivideVectorOperator) {
      const v_ui32_4 a{10u, 20u, 30u, 40u};
      const v_ui32_4 b{2u, 5u, 10u, 8u};
      const v_ui32_4 expected{5u, 4u, 3u, 5u};
      Assert::IsTrue(allTrue(expected == (a / b)), L"'/' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testModuloVectorOperator) {
      const v_ui32_4 a{10u, 21u, 33u, 47u};
      const v_ui32_4 b{3u, 5u, 10u, 8u};
      const v_ui32_4 expected{1u, 1u, 3u, 7u};
      Assert::IsTrue(allTrue(expected == (a % b)), L"'%' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testBitwiseAndOperator) {
      const auto a = MaskAll<v_ui32_4>();
      const auto b = MaskX<v_ui32_4>();
      const auto actual = a & b;
      Assert::IsTrue(allTrue(b == actual), L"'&' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testBitwiseOrOperator) {
      const auto a = MaskX<v_ui32_4>();
      const auto b = MaskY<v_ui32_4>();
      const auto expected = MaskXY<v_ui32_4>();
      const auto actual = a | b;
      Assert::IsTrue(allTrue(expected == actual), L"'|' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testBitwiseXorOperator) {
      const auto a = MaskAll<v_ui32_4>();
      const auto actual = a ^ a;
      Assert::IsTrue(allTrue(Zero<v_ui32_4>() == actual), L"'^' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testLeftShiftOperator) {
      const v_ui32_4 given{1u, 2u, 4u, 8u};
      // _mm_sll_epi32 reads the shift count from the low 64-bit element
      const v_ui32_4 shift{1u, 0u, 0u, 0u};
      const v_ui32_4 expected{2u, 4u, 8u, 16u};
      Assert::IsTrue(allTrue(expected == (given << shift)), L"'<<' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testRightShiftOperator) {
      const v_ui32_4 given{2u, 4u, 8u, 16u};
      // _mm_srl_epi32 reads the shift count from the low 64-bit element
      const v_ui32_4 shift{1u, 0u, 0u, 0u};
      const v_ui32_4 expected{1u, 2u, 4u, 8u};
      Assert::IsTrue(allTrue(expected == (given >> shift)), L"'>>' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testEqualsOperator) {
      const v_ui32_4 a{1u, 2u, 3u, 4u};
      const v_ui32_4 b{1u, 9u, 3u, 9u};
      const auto actual = a == b;

      Assert::AreEqual(MaskAll<v_ui32_4::BoolType>(), x(actual), L"Equals value mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, y(actual), L"Equals value mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_ui32_4::BoolType>(), z(actual), L"Equals value mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, w(actual), L"Equals value mismatch", LINE_INFO());
    }

    TEST_METHOD(testNotEqualsOperator) {
      const v_ui32_4 a{1u, 2u, 3u, 4u};
      const v_ui32_4 b{1u, 9u, 3u, 9u};
      const auto actual = a != b;

      Assert::AreEqual(UInt_32{0}, x(actual), L"NotEquals value mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_ui32_4::BoolType>(), y(actual), L"NotEquals value mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, z(actual), L"NotEquals value mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_ui32_4::BoolType>(), w(actual), L"NotEquals value mismatch", LINE_INFO());
    }

    TEST_METHOD(testLessThanOperator) {
      const v_ui32_4 a{1u, 5u, 3u, 4u};
      const v_ui32_4 b{2u, 5u, 1u, 9u};
      const auto actual = a < b;

      Assert::AreEqual(MaskAll<v_ui32_4::BoolType>(), x(actual), L"'<' value mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, y(actual), L"'<' value mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, z(actual), L"'<' value mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_ui32_4::BoolType>(), w(actual), L"'<' value mismatch", LINE_INFO());
    }

    TEST_METHOD(testGreaterThanOperator) {
      const v_ui32_4 a{2u, 5u, 1u, 9u};
      const v_ui32_4 b{1u, 5u, 3u, 4u};
      const auto actual = a > b;

      Assert::AreEqual(MaskAll<v_ui32_4::BoolType>(), x(actual), L"'>' value mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, y(actual), L"'>' value mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, z(actual), L"'>' value mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_ui32_4::BoolType>(), w(actual), L"'>' value mismatch", LINE_INFO());
    }

    TEST_METHOD(testLessThanWithHighBitValues) {
      const v_ui32_4 a{0x80000000u, 1u, 0xFFFFFFFFu, 0u};
      const v_ui32_4 b{1u, 0x80000000u, 0u, 0xFFFFFFFFu};
      const auto actual = a < b;
      Assert::AreEqual(UInt_32{0}, x(actual), L"0x80000000 < 1 must be false", LINE_INFO());
      Assert::AreEqual(MaskAll<v_ui32_4::BoolType>(), y(actual), L"1 < 0x80000000 must be true", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, z(actual), L"0xFFFFFFFF < 0 must be false", LINE_INFO());
      Assert::AreEqual(MaskAll<v_ui32_4::BoolType>(), w(actual), L"0 < 0xFFFFFFFF must be true", LINE_INFO());
    }

    TEST_METHOD(testGreaterThanWithHighBitValues) {
      const v_ui32_4 a{0x80000000u, 1u, 0xFFFFFFFFu, 0u};
      const v_ui32_4 b{1u, 0x80000000u, 0u, 0xFFFFFFFFu};
      const auto actual = a > b;
      Assert::AreEqual(MaskAll<v_ui32_4::BoolType>(), x(actual), L"0x80000000 > 1 must be true", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, y(actual), L"1 > 0x80000000 must be false", LINE_INFO());
      Assert::AreEqual(MaskAll<v_ui32_4::BoolType>(), z(actual), L"0xFFFFFFFF > 0 must be true", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, w(actual), L"0 > 0xFFFFFFFF must be false", LINE_INFO());
    }

    TEST_METHOD(testLessThanOrEqualWithHighBitValues) {
      const v_ui32_4 a{0x80000000u, 0x80000000u, 0xFFFFFFFFu, 0u};
      const v_ui32_4 b{0x80000000u, 0x80000001u, 0xFFFFFFFEu, 0u};
      const auto result = a <= b;
      Assert::AreEqual(MaskAll<v_ui32_4::BoolType>(), x(result), L"equal values", LINE_INFO());
      Assert::AreEqual(MaskAll<v_ui32_4::BoolType>(), y(result), L"0x80000000 <= 0x80000001", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, z(result), L"0xFFFFFFFF <= 0xFFFFFFFE false", LINE_INFO());
      Assert::AreEqual(MaskAll<v_ui32_4::BoolType>(), w(result), L"0 <= 0", LINE_INFO());
    }

    TEST_METHOD(testGreaterThanOrEqualWithHighBitValues) {
      const v_ui32_4 a{0x80000000u, 0x80000001u, 0xFFFFFFFEu, 0u};
      const v_ui32_4 b{0x80000000u, 0x80000000u, 0xFFFFFFFFu, 0u};
      const auto result = a >= b;
      Assert::AreEqual(MaskAll<v_ui32_4::BoolType>(), x(result), L"equal values", LINE_INFO());
      Assert::AreEqual(MaskAll<v_ui32_4::BoolType>(), y(result), L"0x80000001 >= 0x80000000", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, z(result), L"0xFFFFFFFE >= 0xFFFFFFFF false", LINE_INFO());
      Assert::AreEqual(MaskAll<v_ui32_4::BoolType>(), w(result), L"0 >= 0", LINE_INFO());
    }

    TEST_METHOD(testCompoundAddAssignOperator) {
      v_ui32_4 actual{1u, 2u, 3u, 4u};
      actual += v_ui32_4{10u, 20u, 30u, 40u};
      const v_ui32_4 expected{11u, 22u, 33u, 44u};
      Assert::IsTrue(allTrue(expected == actual), L"'+=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testLessThanOrEqualOperator) {
      const v_ui32_4 a{1, 2, 3, 4};
      const v_ui32_4 b{2, 2, 2, 5};
      const auto result = a <= b;
      Assert::IsTrue(allTrue(result == v_ui32_4{UInt_32(-1), UInt_32(-1), 0, UInt_32(-1)}), L"<= mismatch", LINE_INFO());
    }

    TEST_METHOD(testGreaterThanOrEqualOperator) {
      const v_ui32_4 a{1, 2, 3, 4};
      const v_ui32_4 b{2, 2, 2, 5};
      const auto result = a >= b;
      Assert::IsTrue(allTrue(result == v_ui32_4{0, UInt_32(-1), UInt_32(-1), 0}), L">= mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundSubtractAssignOperator) {
      v_ui32_4 a{10u, 20u, 30u, 40u};
      const v_ui32_4 b{2u, 3u, 5u, 4u};
      const auto expected = a - b;
      a -= b;
      Assert::IsTrue(allTrue(expected == a), L"'-=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundMultiplyAssignOperator) {
      v_ui32_4 a{10u, 20u, 30u, 40u};
      const v_ui32_4 b{2u, 3u, 5u, 4u};
      const auto expected = a * b;
      a *= b;
      Assert::IsTrue(allTrue(expected == a), L"'*=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundDivideAssignOperator) {
      v_ui32_4 a{10u, 20u, 30u, 40u};
      const v_ui32_4 b{2u, 3u, 5u, 4u};
      const auto expected = a / b;
      a /= b;
      Assert::IsTrue(allTrue(expected == a), L"'/=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundModuloAssignOperator) {
      v_ui32_4 a{10u, 20u, 30u, 40u};
      const v_ui32_4 b{2u, 3u, 5u, 4u};
      const auto expected = a % b;
      a %= b;
      Assert::IsTrue(allTrue(expected == a), L"'%=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundBitwiseAndAssignOperator) {
      v_ui32_4 a{0xFF00FF00u, 0x0F0F0F0Fu, 0xAAAAAAAAu, 0x12345678u};
      const v_ui32_4 b{0x0F0F0F0Fu, 0xF0F0F0F0u, 0x55555555u, 0x87654321u};
      const auto expected = a & b;
      a &= b;
      Assert::IsTrue(allTrue(expected == a), L"'&=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundBitwiseOrAssignOperator) {
      v_ui32_4 a{0xFF00FF00u, 0x0F0F0F0Fu, 0xAAAAAAAAu, 0x12345678u};
      const v_ui32_4 b{0x0F0F0F0Fu, 0xF0F0F0F0u, 0x55555555u, 0x87654321u};
      const auto expected = a | b;
      a |= b;
      Assert::IsTrue(allTrue(expected == a), L"'|=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundBitwiseXorAssignOperator) {
      v_ui32_4 a{0xFF00FF00u, 0x0F0F0F0Fu, 0xAAAAAAAAu, 0x12345678u};
      const v_ui32_4 b{0x0F0F0F0Fu, 0xF0F0F0F0u, 0x55555555u, 0x87654321u};
      const auto expected = a ^ b;
      a ^= b;
      Assert::IsTrue(allTrue(expected == a), L"'^=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundLeftShiftAssignOperator) {
      v_ui32_4 a{16u, 32u, 64u, 128u};
      const v_ui32_4 b{v_ui32_4(1)};
      const auto expected = a << b;
      a <<= b;
      Assert::IsTrue(allTrue(expected == a), L"'<<=' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundRightShiftAssignOperator) {
      v_ui32_4 a{16u, 32u, 64u, 128u};
      const v_ui32_4 b{v_ui32_4(1)};
      const auto expected = a >> b;
      a >>= b;
      Assert::IsTrue(allTrue(expected == a), L"'>>=' op value mismatch", LINE_INFO());
    }
  };
}
