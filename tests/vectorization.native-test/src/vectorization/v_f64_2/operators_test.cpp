#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f64_2_OperatorsTest) {
  public:
    TEST_METHOD(testNegationAndBitwiseOps) {
      const v_f64_2 a{1.0, -2.0};
      const auto neg = -a;
      Assert::AreEqual(-1.0, x(neg), L"negation X mismatch", LINE_INFO());
      Assert::AreEqual(2.0, y(neg), L"negation Y mismatch", LINE_INFO());

      // ~ of MaskAll should be Zero
      const auto maskAll = MaskAll<v_f64_2>();
      const auto complement = ~maskAll;
      Assert::AreEqual(0.0, x(complement), L"'~' X mismatch", LINE_INFO());
      Assert::AreEqual(0.0, y(complement), L"'~' Y mismatch", LINE_INFO());

      // ^ self should be zero
      const auto xorSelf = maskAll ^ maskAll;
      Assert::AreEqual(0.0, x(xorSelf), L"'^' X mismatch", LINE_INFO());
      Assert::AreEqual(0.0, y(xorSelf), L"'^' Y mismatch", LINE_INFO());
    }

    TEST_METHOD(testArithmeticOperators) {
      const v_f64_2 a{1.0, 2.0};
      const v_f64_2 b{3.0, 4.0};

      const auto sum = a + b;
      Assert::AreEqual(4.0, x(sum), L"'+' X mismatch", LINE_INFO());
      Assert::AreEqual(6.0, y(sum), L"'+' Y mismatch", LINE_INFO());

      const auto diff = b - a;
      Assert::AreEqual(2.0, x(diff), L"'-' X mismatch", LINE_INFO());
      Assert::AreEqual(2.0, y(diff), L"'-' Y mismatch", LINE_INFO());

      const auto prod = a * b;
      Assert::AreEqual(3.0, x(prod), L"'*' X mismatch", LINE_INFO());
      Assert::AreEqual(8.0, y(prod), L"'*' Y mismatch", LINE_INFO());

      const auto quot = v_f64_2{6.0, 8.0} / v_f64_2{2.0, 4.0};
      Assert::AreEqual(3.0, x(quot), L"'/' X mismatch", LINE_INFO());
      Assert::AreEqual(2.0, y(quot), L"'/' Y mismatch", LINE_INFO());

      // scalar overloads
      const auto addScalar = a + 10.0;
      Assert::AreEqual(11.0, x(addScalar), L"'+scalar' X mismatch", LINE_INFO());
      Assert::AreEqual(12.0, y(addScalar), L"'+scalar' Y mismatch", LINE_INFO());

      const auto mulScalar = 2.0 * a;
      Assert::AreEqual(2.0, x(mulScalar), L"'scalar*' X mismatch", LINE_INFO());
      Assert::AreEqual(4.0, y(mulScalar), L"'scalar*' Y mismatch", LINE_INFO());
    }

    TEST_METHOD(testComparisonOperators) {
      const v_f64_2 a{1.0, 3.0};
      const v_f64_2 b{2.0, 3.0};

      // a < b: X:1<2 true, Y:3<3 false
      const auto lt = a < b;
      Assert::IsTrue(x(lt) != 0, L"'<' X should be true", LINE_INFO());
      Assert::IsTrue(y(lt) == 0, L"'<' Y should be false", LINE_INFO());

      // a == b: X:1==2 false, Y:3==3 true
      const auto eq = a == b;
      Assert::IsTrue(x(eq) == 0, L"'==' X should be false", LINE_INFO());
      Assert::IsTrue(y(eq) != 0, L"'==' Y should be true", LINE_INFO());

      // a >= b: X:1>=2 false, Y:3>=3 true
      const auto ge = a >= b;
      Assert::IsTrue(x(ge) == 0, L"'>=' X should be false", LINE_INFO());
      Assert::IsTrue(y(ge) != 0, L"'>=' Y should be true", LINE_INFO());
    }

    TEST_METHOD(testCompoundAssignmentOperators) {
      auto a = v_f64_2{2.0, 3.0};
      const v_f64_2 b{1.0, 2.0};

      a += b;
      Assert::AreEqual(3.0, x(a), L"'+=' X mismatch", LINE_INFO());
      Assert::AreEqual(5.0, y(a), L"'+=' Y mismatch", LINE_INFO());

      a -= b;
      Assert::AreEqual(2.0, x(a), L"'-=' X mismatch", LINE_INFO());
      Assert::AreEqual(3.0, y(a), L"'-=' Y mismatch", LINE_INFO());

      a *= v_f64_2{2.0, 3.0};
      Assert::AreEqual(4.0, x(a), L"'*=' X mismatch", LINE_INFO());
      Assert::AreEqual(9.0, y(a), L"'*=' Y mismatch", LINE_INFO());

      a /= v_f64_2{2.0, 3.0};
      Assert::AreEqual(2.0, x(a), L"'/=' X mismatch", LINE_INFO());
      Assert::AreEqual(3.0, y(a), L"'/=' Y mismatch", LINE_INFO());
    }

    TEST_METHOD(testLeftShiftOperator) {
      const v_f64_2 given = One<v_f64_2>();
      const v_ui64_2 shift{0ULL, 1ULL};
      const auto actual = given << shift;
      // 1.0 = 0x3FF0000000000000, << 0 = same, << 1 = 0x7FE0000000000000
      Assert::AreEqual(1.0, x(actual), L"'<<' lane 0 (shift 0) should preserve value", LINE_INFO());
      Assert::AreNotEqual(1.0, y(actual), L"'<<' lane 1 (shift 1) should change value", LINE_INFO());
      Assert::AreNotEqual(0.0, y(actual), L"'<<' lane 1 should be non-zero", LINE_INFO());
    }

    TEST_METHOD(testLeftShiftScalarOperator) {
      const v_f64_2 given = One<v_f64_2>();
      const auto shifted = given << UInt_64{1};
      const auto unshifted = given << UInt_64{0};
      Assert::AreEqual(1.0, x(unshifted), L"'<<' by 0 preserves value", LINE_INFO());
      Assert::AreNotEqual(1.0, x(shifted), L"'<<' by 1 changes value", LINE_INFO());
    }

    TEST_METHOD(testLeftShiftCrossSignednessOperator) {
      const v_f64_2 given = One<v_f64_2>();
      const v_i64_2 shift{0LL, 1LL};
      const auto actual = given << shift;
      Assert::AreEqual(1.0, x(actual), L"'<<' cross-sign lane 0 mismatch", LINE_INFO());
      Assert::AreNotEqual(1.0, y(actual), L"'<<' cross-sign lane 1 mismatch", LINE_INFO());
    }

    TEST_METHOD(testRightShiftOperator) {
      const v_f64_2 given = One<v_f64_2>();
      const v_ui64_2 shift{0ULL, 1ULL};
      const auto actual = given >> shift;
      Assert::AreEqual(1.0, x(actual), L"'>>' lane 0 (shift 0) should preserve value", LINE_INFO());
      Assert::AreNotEqual(1.0, y(actual), L"'>>' lane 1 (shift 1) should change value", LINE_INFO());
    }

    TEST_METHOD(testRightShiftIsLogical) {
      const v_f64_2 given{-1.0, -2.0};
      const v_ui64_2 shift{1ULL, 1ULL};
      const auto actual = given >> shift;
      Assert::IsTrue(x(actual) > 0.0 || x(actual) == 0.0, L"'>>' on float must be logical (positive result)", LINE_INFO());
    }

    TEST_METHOD(testShiftByZeroPreservesValue) {
      const v_f64_2 given{1.0, -2.0};
      Assert::IsTrue(allTrue(given == (given << UInt_64{0})), L"'<<' by 0 preserves value", LINE_INFO());
      Assert::IsTrue(allTrue(given == (given >> UInt_64{0})), L"'>>' by 0 preserves value", LINE_INFO());
    }

    TEST_METHOD(testCompoundLeftShiftAssignOperator) {
      auto a = One<v_f64_2>();
      const v_ui64_2 shift{1ULL, 0ULL};
      const auto expected = a << shift;
      a <<= shift;
      Assert::AreEqual(x(expected), x(a), L"'<<=' X mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(a), L"'<<=' Y mismatch", LINE_INFO());
    }

    TEST_METHOD(testCompoundRightShiftAssignOperator) {
      auto a = One<v_f64_2>();
      const v_ui64_2 shift{1ULL, 0ULL};
      const auto expected = a >> shift;
      a >>= shift;
      Assert::AreEqual(x(expected), x(a), L"'>>=' X mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(a), L"'>>=' Y mismatch", LINE_INFO());
    }

    TEST_METHOD(testModuloOperator) {
      const v_f64_2 a{5.5, 7.0};
      const v_f64_2 b{2.0, 3.0};
      const auto actual = a % b;

      Assert::AreEqual(std::fmod(5.5, 2.0), x(actual), 1e-10, L"'%%' X mismatch", LINE_INFO());
      Assert::AreEqual(std::fmod(7.0, 3.0), y(actual), 1e-10, L"'%%' Y mismatch", LINE_INFO());
    }

    TEST_METHOD(testOstream) {
      const v_f64_2 v{1.0, 2.0};
      std::ostringstream oss;
      oss << v;
      Assert::IsFalse(oss.str().empty(), L"ostream output should not be empty", LINE_INFO());
    }
  };
}
