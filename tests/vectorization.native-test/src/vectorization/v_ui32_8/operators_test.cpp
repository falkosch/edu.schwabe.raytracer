#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_ui32_8_OperatorsTest) {
  public:
    TEST_METHOD(testOnesComplementOperator) {
      const v_ui32_8 allOnes{UInt_32{0xFFFFFFFFu}};
      const auto actual = ~allOnes;
      Assert::IsTrue(allTrue(Zero<v_ui32_8>() == actual), L"'~' op value mismatch", LINE_INFO());

      const auto actual2 = ~Zero<v_ui32_8>();
      Assert::IsTrue(allTrue(allOnes == actual2), L"'~' zero mismatch", LINE_INFO());
    }

    TEST_METHOD(testLogicalNegationOperator) {
      const v_ui32_8 given{0u, 1u, 0u, 42u, 0u, 99u, 0u, 0xFFFFFFFFu};
      const auto actual = !given;
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x1(actual), L"X1 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, x2(actual), L"X2 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x3(actual), L"X3 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, x4(actual), L"X4 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x5(actual), L"X5 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, x6(actual), L"X6 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x7(actual), L"X7 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, x8(actual), L"X8 mismatch", LINE_INFO());
    }

    TEST_METHOD(testAddVectorOperator) {
      const v_ui32_8 a{1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u};
      const v_ui32_8 b{10u, 20u, 30u, 40u, 50u, 60u, 70u, 80u};
      const v_ui32_8 expected{11u, 22u, 33u, 44u, 55u, 66u, 77u, 88u};
      Assert::IsTrue(allTrue(expected == (a + b)), L"'+' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testSubtractVectorOperator) {
      const v_ui32_8 a{10u, 20u, 30u, 40u, 50u, 60u, 70u, 80u};
      const v_ui32_8 b{1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u};
      const v_ui32_8 expected{9u, 18u, 27u, 36u, 45u, 54u, 63u, 72u};
      Assert::IsTrue(allTrue(expected == (a - b)), L"'-' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testMultiplyVectorOperator) {
      const v_ui32_8 a{2u, 3u, 4u, 5u, 6u, 7u, 8u, 9u};
      const v_ui32_8 b{10u, 20u, 30u, 40u, 50u, 60u, 70u, 80u};
      const v_ui32_8 expected{20u, 60u, 120u, 200u, 300u, 420u, 560u, 720u};
      Assert::IsTrue(allTrue(expected == (a * b)), L"'*' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testBitwiseAndOperator) {
      const v_ui32_8 allOnes{UInt_32{0xFFFFFFFFu}};
      const v_ui32_8 pattern{0xFF00FF00u, 0x0F0F0F0Fu, 0xAAAAAAAAu, 0x55555555u,
                             0x12345678u, 0x87654321u, 0xDEADBEEFu, 0xCAFEBABEu};
      const auto actual = allOnes & pattern;
      Assert::IsTrue(allTrue(pattern == actual), L"'&' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testBitwiseOrOperator) {
      const v_ui32_8 a{0xFF000000u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};
      const v_ui32_8 b{0u, 0u, 0u, 0u, 0u, 0u, 0u, 0x000000FFu};
      const v_ui32_8 expected{0xFF000000u, 0u, 0u, 0u, 0u, 0u, 0u, 0x000000FFu};
      Assert::IsTrue(allTrue(expected == (a | b)), L"'|' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testBitwiseXorOperator) {
      const v_ui32_8 a{UInt_32{0xFFFFFFFFu}};
      const auto actual = a ^ a;
      Assert::IsTrue(allTrue(Zero<v_ui32_8>() == actual), L"'^' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testLeftShiftOperator) {
      const v_ui32_8 given{1u, 2u, 4u, 8u, 16u, 32u, 64u, 128u};
      const v_ui32_8 shift{1u, 2u, 3u, 4u, 0u, 1u, 2u, 3u};
      const v_ui32_8 expected{2u, 8u, 32u, 128u, 16u, 64u, 256u, 1024u};
      Assert::IsTrue(allTrue(expected == (given << shift)), L"'<<' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testLeftShiftEdgeCases) {
      const v_ui32_8 given{1u, 0xFFFFFFFFu, 0x80000000u, 1u, 1u, 1u, 1u, 1u};
      const v_ui32_8 shift{0u, 0u, 0u, 31u, 0u, 0u, 0u, 0u};
      Assert::AreEqual(UInt_32{1}, x1(given << shift), L"shift by 0", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x2(given << shift), L"shift max by 0", LINE_INFO());
      Assert::AreEqual(UInt_32{0x80000000u}, x3(given << shift), L"shift high-bit by 0", LINE_INFO());
      Assert::AreEqual(UInt_32{0x80000000u}, x4(given << shift), L"shift 1 by 31", LINE_INFO());
    }

    TEST_METHOD(testRightShiftOperator) {
      const v_ui32_8 given{16u, 32u, 64u, 128u, 256u, 512u, 1024u, 2048u};
      const v_ui32_8 shift{1u, 2u, 3u, 4u, 0u, 1u, 2u, 3u};
      const v_ui32_8 expected{8u, 8u, 8u, 8u, 256u, 256u, 256u, 256u};
      Assert::IsTrue(allTrue(expected == (given >> shift)), L"'>>' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testRightShiftIsLogical) {
      const v_ui32_8 given{UInt_32{0x80000000u}};
      const v_ui32_8 shift{UInt_32{1u}};
      const v_ui32_8 expected{UInt_32{0x40000000u}};
      Assert::IsTrue(allTrue(expected == (given >> shift)), L"logical shift must not sign-extend", LINE_INFO());
    }

    TEST_METHOD(testLessThanOperator) {
      const v_ui32_8 a{1u, 5u, 3u, 4u, 1u, 5u, 3u, 4u};
      const v_ui32_8 b{2u, 5u, 1u, 9u, 2u, 5u, 1u, 9u};
      const auto actual = a < b;
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x1(actual), L"1 < 2", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, x2(actual), L"5 < 5", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, x3(actual), L"3 < 1", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x4(actual), L"4 < 9", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x5(actual), L"1 < 2", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, x6(actual), L"5 < 5", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, x7(actual), L"3 < 1", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x8(actual), L"4 < 9", LINE_INFO());
    }

    TEST_METHOD(testLessThanWithHighBitValues) {
      const v_ui32_8 a{0x80000000u, 1u, 0xFFFFFFFFu, 0u, 0x80000000u, 0x7FFFFFFFu, 0xFFFFFFFEu, 0x80000001u};
      const v_ui32_8 b{1u, 0x80000000u, 0u, 0xFFFFFFFFu, 0x80000001u, 0x80000000u, 0xFFFFFFFFu, 0x80000000u};
      const auto actual = a < b;
      Assert::AreEqual(UInt_32{0}, x1(actual), L"0x80000000 < 1 must be false", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x2(actual), L"1 < 0x80000000 must be true", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, x3(actual), L"0xFFFFFFFF < 0 must be false", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x4(actual), L"0 < 0xFFFFFFFF must be true", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x5(actual), L"0x80000000 < 0x80000001 must be true", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x6(actual), L"0x7FFFFFFF < 0x80000000 must be true", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x7(actual), L"0xFFFFFFFE < 0xFFFFFFFF must be true", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, x8(actual), L"0x80000001 < 0x80000000 must be false", LINE_INFO());
    }

    TEST_METHOD(testGreaterThanOperator) {
      const v_ui32_8 a{2u, 5u, 1u, 9u, 2u, 5u, 1u, 9u};
      const v_ui32_8 b{1u, 5u, 3u, 4u, 1u, 5u, 3u, 4u};
      const auto actual = a > b;
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x1(actual), L"2 > 1", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, x2(actual), L"5 > 5", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, x3(actual), L"1 > 3", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x4(actual), L"9 > 4", LINE_INFO());
    }

    TEST_METHOD(testGreaterThanWithHighBitValues) {
      const v_ui32_8 a{0x80000000u, 1u, 0xFFFFFFFFu, 0u, 0x80000001u, 0x80000000u, 0xFFFFFFFFu, 0x80000000u};
      const v_ui32_8 b{1u, 0x80000000u, 0u, 0xFFFFFFFFu, 0x80000000u, 0x7FFFFFFFu, 0xFFFFFFFEu, 0x80000001u};
      const auto actual = a > b;
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x1(actual), L"0x80000000 > 1 must be true", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, x2(actual), L"1 > 0x80000000 must be false", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x3(actual), L"0xFFFFFFFF > 0 must be true", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, x4(actual), L"0 > 0xFFFFFFFF must be false", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x5(actual), L"0x80000001 > 0x80000000 must be true", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x6(actual), L"0x80000000 > 0x7FFFFFFF must be true", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x7(actual), L"0xFFFFFFFF > 0xFFFFFFFE must be true", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, x8(actual), L"0x80000000 > 0x80000001 must be false", LINE_INFO());
    }

    TEST_METHOD(testLessThanOrEqualOperator) {
      const v_ui32_8 a{1u, 2u, 3u, 4u, 0u, 0xFFFFFFFFu, 0x80000000u, 5u};
      const v_ui32_8 b{2u, 2u, 2u, 5u, 0u, 0xFFFFFFFFu, 0x80000001u, 4u};
      const auto result = a <= b;
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x1(result), L"1<=2 true", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x2(result), L"2<=2 true", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, x3(result), L"3<=2 false", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x4(result), L"4<=5 true", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x5(result), L"0<=0 true", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x6(result), L"max<=max true", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x7(result), L"0x80000000<=0x80000001 true", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, x8(result), L"5<=4 false", LINE_INFO());
    }

    TEST_METHOD(testGreaterThanOrEqualOperator) {
      const v_ui32_8 a{1u, 2u, 3u, 4u, 0u, 0xFFFFFFFFu, 0x80000001u, 5u};
      const v_ui32_8 b{2u, 2u, 2u, 5u, 0u, 0xFFFFFFFFu, 0x80000000u, 4u};
      const auto result = a >= b;
      Assert::AreEqual(UInt_32{0}, x1(result), L"1>=2 false", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x2(result), L"2>=2 true", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x3(result), L"3>=2 true", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, x4(result), L"4>=5 false", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x5(result), L"0>=0 true", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x6(result), L"max>=max true", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x7(result), L"0x80000001>=0x80000000 true", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x8(result), L"5>=4 true", LINE_INFO());
    }

    TEST_METHOD(testEqualsOperator) {
      const v_ui32_8 a{1u, 2u, 0x80000000u, 0xFFFFFFFFu, 5u, 6u, 7u, 8u};
      const v_ui32_8 b{1u, 9u, 0x80000000u, 0xFFFFFFFEu, 5u, 9u, 7u, 9u};
      const auto actual = a == b;
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x1(actual), L"X1 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, x2(actual), L"X2 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x3(actual), L"X3 high-bit equal", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, x4(actual), L"X4 high-bit differ", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x5(actual), L"X5 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, x6(actual), L"X6 mismatch", LINE_INFO());
    }

    TEST_METHOD(testNotEqualsOperator) {
      const v_ui32_8 a{1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u};
      const v_ui32_8 b{1u, 9u, 3u, 9u, 5u, 9u, 7u, 9u};
      const auto actual = a != b;
      Assert::AreEqual(UInt_32{0}, x1(actual), L"X1 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x2(actual), L"X2 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{0}, x3(actual), L"X3 mismatch", LINE_INFO());
      Assert::AreEqual(UInt_32{0xFFFFFFFFu}, x4(actual), L"X4 mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundArithmeticAssignOperators) {
      {
        v_ui32_8 a{1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u};
        const v_ui32_8 b{10u, 20u, 30u, 40u, 50u, 60u, 70u, 80u};
        const auto expected = a + b;
        a += b;
        Assert::IsTrue(allTrue(expected == a), L"'+=' mismatch", LINE_INFO());
      }

      {
        v_ui32_8 a{10u, 20u, 30u, 40u, 50u, 60u, 70u, 80u};
        const v_ui32_8 b{1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u};
        const auto expected = a - b;
        a -= b;
        Assert::IsTrue(allTrue(expected == a), L"'-=' mismatch", LINE_INFO());
      }

      {
        v_ui32_8 a{2u, 3u, 4u, 5u, 6u, 7u, 8u, 9u};
        const v_ui32_8 b{10u, 20u, 30u, 40u, 50u, 60u, 70u, 80u};
        const auto expected = a * b;
        a *= b;
        Assert::IsTrue(allTrue(expected == a), L"'*=' mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(testCompoundBitwiseAssignOperators) {
      {
        v_ui32_8 a{UInt_32{0xFFFFFFFFu}};
        const v_ui32_8 b{1u, 0u, 1u, 0u, 1u, 0u, 1u, 0u};
        const auto expected = a & b;
        a &= b;
        Assert::IsTrue(allTrue(expected == a), L"'&=' mismatch", LINE_INFO());
      }

      {
        v_ui32_8 a{1u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};
        const v_ui32_8 b{0u, 1u, 0u, 0u, 0u, 0u, 0u, 0u};
        const auto expected = a | b;
        a |= b;
        Assert::IsTrue(allTrue(expected == a), L"'|=' mismatch", LINE_INFO());
      }

      {
        v_ui32_8 a{1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u};
        const auto expected = a ^ a;
        a ^= a;
        Assert::IsTrue(allTrue(expected == a), L"'^=' mismatch", LINE_INFO());
      }
    }

    TEST_METHOD(testCompoundShiftAssignOperators) {
      {
        v_ui32_8 a{1u, 2u, 4u, 8u, 16u, 32u, 64u, 128u};
        const v_ui32_8 b{1u, 2u, 3u, 4u, 0u, 1u, 2u, 3u};
        const auto expected = a << b;
        a <<= b;
        Assert::IsTrue(allTrue(expected == a), L"'<<=' mismatch", LINE_INFO());
      }

      {
        v_ui32_8 a{16u, 32u, 64u, 128u, 256u, 512u, 1024u, 2048u};
        const v_ui32_8 b{1u, 2u, 3u, 4u, 0u, 1u, 2u, 3u};
        const auto expected = a >> b;
        a >>= b;
        Assert::IsTrue(allTrue(expected == a), L"'>>=' mismatch", LINE_INFO());
      }
    }
  };
}
