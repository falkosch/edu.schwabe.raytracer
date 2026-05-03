#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f64_4_OperatorsTest) {
  public:
    TEST_METHOD(testNegationAndBitwiseOps) {
      const v_f64_4 a{1.0, -2.0, 3.0, -4.0};
      const auto neg = -a;
      Assert::AreEqual(-1.0, x(neg), L"negation X mismatch", LINE_INFO());
      Assert::AreEqual(2.0, y(neg), L"negation Y mismatch", LINE_INFO());
      Assert::AreEqual(-3.0, z(neg), L"negation Z mismatch", LINE_INFO());
      Assert::AreEqual(4.0, w(neg), L"negation W mismatch", LINE_INFO());

      // ~ of MaskAll should be Zero
      const auto maskAll = MaskAll<v_f64_4>();
      const auto complement = ~maskAll;
      Assert::AreEqual(0.0, x(complement), L"'~' X mismatch", LINE_INFO());
      Assert::AreEqual(0.0, y(complement), L"'~' Y mismatch", LINE_INFO());
      Assert::AreEqual(0.0, z(complement), L"'~' Z mismatch", LINE_INFO());
      Assert::AreEqual(0.0, w(complement), L"'~' W mismatch", LINE_INFO());

      // ^ self should be zero
      const auto xorSelf = maskAll ^ maskAll;
      Assert::AreEqual(0.0, x(xorSelf), L"'^' X mismatch", LINE_INFO());
      Assert::AreEqual(0.0, w(xorSelf), L"'^' W mismatch", LINE_INFO());
    }

    TEST_METHOD(testArithmeticOperators) {
      const v_f64_4 a{1.0, 2.0, 3.0, 4.0};
      const v_f64_4 b{5.0, 6.0, 7.0, 8.0};

      const auto sum = a + b;
      Assert::AreEqual(6.0, x(sum), L"'+' X mismatch", LINE_INFO());
      Assert::AreEqual(8.0, y(sum), L"'+' Y mismatch", LINE_INFO());
      Assert::AreEqual(10.0, z(sum), L"'+' Z mismatch", LINE_INFO());
      Assert::AreEqual(12.0, w(sum), L"'+' W mismatch", LINE_INFO());

      const auto diff = b - a;
      Assert::AreEqual(4.0, x(diff), L"'-' X mismatch", LINE_INFO());
      Assert::AreEqual(4.0, w(diff), L"'-' W mismatch", LINE_INFO());

      const auto prod = a * b;
      Assert::AreEqual(5.0, x(prod), L"'*' X mismatch", LINE_INFO());
      Assert::AreEqual(32.0, w(prod), L"'*' W mismatch", LINE_INFO());

      const auto quot = v_f64_4{6.0, 8.0, 10.0, 12.0} / v_f64_4{2.0, 4.0, 5.0, 6.0};
      Assert::AreEqual(3.0, x(quot), L"'/' X mismatch", LINE_INFO());
      Assert::AreEqual(2.0, y(quot), L"'/' Y mismatch", LINE_INFO());
      Assert::AreEqual(2.0, z(quot), L"'/' Z mismatch", LINE_INFO());
      Assert::AreEqual(2.0, w(quot), L"'/' W mismatch", LINE_INFO());
    }

    TEST_METHOD(testScalarOverloads) {
      const v_f64_4 a{1.0, 2.0, 3.0, 4.0};

      const auto addScalar = a + 10.0;
      Assert::AreEqual(11.0, x(addScalar), L"'+scalar' X mismatch", LINE_INFO());
      Assert::AreEqual(14.0, w(addScalar), L"'+scalar' W mismatch", LINE_INFO());

      const auto mulScalar = 2.0 * a;
      Assert::AreEqual(2.0, x(mulScalar), L"'scalar*' X mismatch", LINE_INFO());
      Assert::AreEqual(8.0, w(mulScalar), L"'scalar*' W mismatch", LINE_INFO());

      const auto divScalar = a / 2.0;
      Assert::AreEqual(0.5, x(divScalar), L"'/scalar' X mismatch", LINE_INFO());
      Assert::AreEqual(2.0, w(divScalar), L"'/scalar' W mismatch", LINE_INFO());
    }

    TEST_METHOD(testComparisonOperators) {
      const v_f64_4 a{1.0, 3.0, 5.0, 7.0};
      const v_f64_4 b{2.0, 3.0, 4.0, 8.0};

      // a < b: X:true, Y:false, Z:false, W:true
      const auto lt = a < b;
      Assert::IsTrue(x(lt) != 0, L"'<' X should be true", LINE_INFO());
      Assert::IsTrue(y(lt) == 0, L"'<' Y should be false", LINE_INFO());
      Assert::IsTrue(z(lt) == 0, L"'<' Z should be false", LINE_INFO());
      Assert::IsTrue(w(lt) != 0, L"'<' W should be true", LINE_INFO());

      // a == b: only Y is equal
      const auto eq = a == b;
      Assert::IsTrue(x(eq) == 0, L"'==' X should be false", LINE_INFO());
      Assert::IsTrue(y(eq) != 0, L"'==' Y should be true", LINE_INFO());
      Assert::IsTrue(z(eq) == 0, L"'==' Z should be false", LINE_INFO());
      Assert::IsTrue(w(eq) == 0, L"'==' W should be false", LINE_INFO());
    }

    TEST_METHOD(testCompoundAssignmentOperators) {
      auto a = v_f64_4{1.0, 2.0, 3.0, 4.0};
      const v_f64_4 b{1.0, 1.0, 1.0, 1.0};

      a += b;
      Assert::AreEqual(2.0, x(a), L"'+=' X mismatch", LINE_INFO());
      Assert::AreEqual(5.0, w(a), L"'+=' W mismatch", LINE_INFO());

      a -= b;
      Assert::AreEqual(1.0, x(a), L"'-=' X mismatch", LINE_INFO());
      Assert::AreEqual(4.0, w(a), L"'-=' W mismatch", LINE_INFO());

      a *= v_f64_4{2.0, 3.0, 4.0, 5.0};
      Assert::AreEqual(2.0, x(a), L"'*=' X mismatch", LINE_INFO());
      Assert::AreEqual(20.0, w(a), L"'*=' W mismatch", LINE_INFO());
    }

    TEST_METHOD(testModuloOperator) {
      const v_f64_4 a{5.5, 7.0, 10.3, 3.0};
      const v_f64_4 b{2.0, 3.0, 4.0, 1.5};
      const auto actual = a % b;

      Assert::AreEqual(std::fmod(5.5, 2.0), x(actual), 1e-10, L"'%%' X mismatch", LINE_INFO());
      Assert::AreEqual(std::fmod(7.0, 3.0), y(actual), 1e-10, L"'%%' Y mismatch", LINE_INFO());
      Assert::AreEqual(std::fmod(10.3, 4.0), z(actual), 1e-10, L"'%%' Z mismatch", LINE_INFO());
      Assert::AreEqual(std::fmod(3.0, 1.5), w(actual), 1e-10, L"'%%' W mismatch", LINE_INFO());
    }
  };
}
