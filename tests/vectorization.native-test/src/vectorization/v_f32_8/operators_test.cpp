#include "vectorization_tests.h"

namespace vectorization::test {
  TEST_CLASS(v_f32_8_OperatorsTest) {
  public:
    TEST_METHOD(negation) {
      const v_f32_8 v(1.0f, -2.0f, 3.0f, -4.0f, 5.0f, -6.0f, 7.0f, -8.0f);
      const auto r = -v;
      Assert::AreEqual(-1.0f, x1(r), L"x1", LINE_INFO());
      Assert::AreEqual(2.0f, x2(r), L"x2", LINE_INFO());
      Assert::AreEqual(-5.0f, x5(r), L"x5", LINE_INFO());
      Assert::AreEqual(8.0f, x8(r), L"x8", LINE_INFO());
    }

    TEST_METHOD(addition) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_f32_8 b(10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f);
      const auto r = a + b;
      Assert::AreEqual(11.0f, x1(r), L"x1", LINE_INFO());
      Assert::AreEqual(88.0f, x8(r), L"x8", LINE_INFO());
    }

    TEST_METHOD(additionWithScalar) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = a + 10.0f;
      Assert::AreEqual(11.0f, x1(r), L"x1", LINE_INFO());
      Assert::AreEqual(18.0f, x8(r), L"x8", LINE_INFO());
    }

    TEST_METHOD(scalarPlusVector) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = 10.0f + a;
      Assert::AreEqual(11.0f, x1(r), L"x1", LINE_INFO());
      Assert::AreEqual(18.0f, x8(r), L"x8", LINE_INFO());
    }

    TEST_METHOD(subtraction) {
      const v_f32_8 a(10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f);
      const v_f32_8 b(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = a - b;
      Assert::AreEqual(9.0f, x1(r), L"x1", LINE_INFO());
      Assert::AreEqual(72.0f, x8(r), L"x8", LINE_INFO());
    }

    TEST_METHOD(vectorMinusScalar) {
      const v_f32_8 a(10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f);
      const auto r = a - 5.0f;
      Assert::AreEqual(5.0f, x1(r), L"x1", LINE_INFO());
      Assert::AreEqual(15.0f, x2(r), L"x2", LINE_INFO());
      Assert::AreEqual(75.0f, x8(r), L"x8", LINE_INFO());
    }

    TEST_METHOD(scalarMinusVector) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = 10.0f - a;
      Assert::AreEqual(9.0f, x1(r), L"x1", LINE_INFO());
      Assert::AreEqual(8.0f, x2(r), L"x2", LINE_INFO());
      Assert::AreEqual(2.0f, x8(r), L"x8", LINE_INFO());
    }

    TEST_METHOD(multiplication) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_f32_8 b(2.0f);
      const auto r = a * b;
      Assert::AreEqual(2.0f, x1(r), L"x1", LINE_INFO());
      Assert::AreEqual(16.0f, x8(r), L"x8", LINE_INFO());
    }

    TEST_METHOD(multiplicationWithScalar) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = a * 3.0f;
      Assert::AreEqual(3.0f, x1(r), L"x1", LINE_INFO());
      Assert::AreEqual(24.0f, x8(r), L"x8", LINE_INFO());
    }

    TEST_METHOD(scalarTimesVector) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = 2.0f * a;
      Assert::AreEqual(2.0f, x1(r), L"x1", LINE_INFO());
      Assert::AreEqual(16.0f, x8(r), L"x8", LINE_INFO());
    }

    TEST_METHOD(division) {
      const v_f32_8 a(2.0f, 4.0f, 6.0f, 8.0f, 10.0f, 12.0f, 14.0f, 16.0f);
      const auto r = a / v_f32_8(2.0f);
      Assert::AreEqual(1.0f, x1(r), L"x1", LINE_INFO());
      Assert::AreEqual(8.0f, x8(r), L"x8", LINE_INFO());
    }

    TEST_METHOD(vectorDividedByScalar) {
      const v_f32_8 a(2.0f, 4.0f, 6.0f, 8.0f, 10.0f, 12.0f, 14.0f, 16.0f);
      const auto r = a / 2.0f;
      Assert::AreEqual(1.0f, x1(r), L"x1", LINE_INFO());
      Assert::AreEqual(3.0f, x3(r), L"x3", LINE_INFO());
      Assert::AreEqual(8.0f, x8(r), L"x8", LINE_INFO());
    }

    TEST_METHOD(scalarDividedByVector) {
      const v_f32_8 a(1.0f, 2.0f, 4.0f, 5.0f, 8.0f, 10.0f, 20.0f, 25.0f);
      const auto r = 100.0f / a;
      Assert::AreEqual(100.0f, x1(r), L"x1", LINE_INFO());
      Assert::AreEqual(50.0f, x2(r), L"x2", LINE_INFO());
      Assert::AreEqual(25.0f, x3(r), L"x3", LINE_INFO());
      Assert::AreEqual(4.0f, x8(r), L"x8", LINE_INFO());
    }

    TEST_METHOD(moduloOperator) {
      const v_f32_8 a(5.0f, 7.0f, 10.0f, 3.5f, 9.0f, 11.0f, 6.5f, 15.0f);
      const v_f32_8 b(3.0f, 4.0f, 3.0f, 2.0f, 4.0f, 5.0f, 3.0f, 7.0f);
      const auto r = a % b;
      Assert::AreEqual(2.0f, x1(r), 1e-5f, L"5%3=2", LINE_INFO());
      Assert::AreEqual(3.0f, x2(r), 1e-5f, L"7%4=3", LINE_INFO());
      Assert::AreEqual(1.0f, x3(r), 1e-5f, L"10%3=1", LINE_INFO());
      Assert::AreEqual(1.5f, x4(r), 1e-5f, L"3.5%2=1.5", LINE_INFO());
      Assert::AreEqual(1.0f, x5(r), 1e-5f, L"9%4=1", LINE_INFO());
      Assert::AreEqual(1.0f, x6(r), 1e-5f, L"11%5=1", LINE_INFO());
      Assert::AreEqual(0.5f, x7(r), 1e-5f, L"6.5%3=0.5", LINE_INFO());
      Assert::AreEqual(1.0f, x8(r), 1e-5f, L"15%7=1", LINE_INFO());
    }

    TEST_METHOD(bitwiseNot) {
      const auto z = Zero<v_f32_8>();
      const auto r = ~z;
      // ~0 sets all bits, which is NaN for float
      Assert::IsTrue(std::isnan(x1(r)), L"~zero is NaN", LINE_INFO());
      Assert::IsTrue(std::isnan(x8(r)), L"~zero x8 is NaN", LINE_INFO());
    }

    TEST_METHOD(bitwiseNotOfMaskAll) {
      const auto all = MaskAll<v_f32_8>();
      const auto r = ~all;
      Assert::AreEqual(0.0f, x1(r), L"~all is zero x1", LINE_INFO());
      Assert::AreEqual(0.0f, x8(r), L"~all is zero x8", LINE_INFO());
    }

    TEST_METHOD(compoundAdd) {
      v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      a += v_f32_8(1.0f);
      Assert::AreEqual(2.0f, x1(a), L"x1", LINE_INFO());
      Assert::AreEqual(9.0f, x8(a), L"x8", LINE_INFO());
    }

    TEST_METHOD(compoundSubtract) {
      v_f32_8 a(10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f);
      a -= v_f32_8(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      Assert::AreEqual(9.0f, x1(a), L"x1", LINE_INFO());
      Assert::AreEqual(18.0f, x2(a), L"x2", LINE_INFO());
      Assert::AreEqual(72.0f, x8(a), L"x8", LINE_INFO());
    }

    TEST_METHOD(compoundMultiply) {
      v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      a *= v_f32_8(2.0f);
      Assert::AreEqual(2.0f, x1(a), L"x1", LINE_INFO());
      Assert::AreEqual(8.0f, x4(a), L"x4", LINE_INFO());
      Assert::AreEqual(16.0f, x8(a), L"x8", LINE_INFO());
    }

    TEST_METHOD(compoundDivide) {
      v_f32_8 a(2.0f, 4.0f, 6.0f, 8.0f, 10.0f, 12.0f, 14.0f, 16.0f);
      a /= v_f32_8(2.0f);
      Assert::AreEqual(1.0f, x1(a), L"x1", LINE_INFO());
      Assert::AreEqual(4.0f, x4(a), L"x4", LINE_INFO());
      Assert::AreEqual(8.0f, x8(a), L"x8", LINE_INFO());
    }

    TEST_METHOD(compoundModulo) {
      v_f32_8 a(5.0f, 7.0f, 10.0f, 9.0f, 11.0f, 13.0f, 15.0f, 17.0f);
      a %= v_f32_8(3.0f);
      Assert::AreEqual(2.0f, x1(a), 1e-5f, L"5%3=2", LINE_INFO());
      Assert::AreEqual(1.0f, x2(a), 1e-5f, L"7%3=1", LINE_INFO());
      Assert::AreEqual(1.0f, x3(a), 1e-5f, L"10%3=1", LINE_INFO());
      Assert::AreEqual(2.0f, x8(a), 1e-5f, L"17%3=2", LINE_INFO());
    }

    TEST_METHOD(compoundBitwiseAnd) {
      v_f32_8 a = MaskAll<v_f32_8>();
      a &= Zero<v_f32_8>();
      Assert::AreEqual(0.0f, x1(a), L"all & zero = zero x1", LINE_INFO());
      Assert::AreEqual(0.0f, x8(a), L"all & zero = zero x8", LINE_INFO());
    }

    TEST_METHOD(compoundBitwiseOr) {
      v_f32_8 a = Zero<v_f32_8>();
      a |= MaskAll<v_f32_8>();
      Assert::IsTrue(std::isnan(x1(a)), L"zero | all = NaN x1", LINE_INFO());
      Assert::IsTrue(std::isnan(x8(a)), L"zero | all = NaN x8", LINE_INFO());
    }

    TEST_METHOD(compoundBitwiseXor) {
      v_f32_8 a = MaskAll<v_f32_8>();
      a ^= MaskAll<v_f32_8>();
      Assert::AreEqual(0.0f, x1(a), L"all ^ all = zero x1", LINE_INFO());
      Assert::AreEqual(0.0f, x8(a), L"all ^ all = zero x8", LINE_INFO());
    }

    TEST_METHOD(shiftLeft) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_i32_8 shift(1);
      const auto r = a << shift;
      // Bit-shifting float interpreted as int, then back to float
      // Just verify it doesn't crash and produces a result
      (void)x1(r);
      (void)x8(r);
    }

    TEST_METHOD(shiftRight) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_i32_8 shift(1);
      const auto r = a >> shift;
      (void)x1(r);
      (void)x8(r);
    }

    TEST_METHOD(compoundShiftLeft) {
      v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_i32_8 shift(1);
      a <<= shift;
      (void)x1(a);
      (void)x8(a);
    }

    TEST_METHOD(compoundShiftRight) {
      v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_i32_8 shift(1);
      a >>= shift;
      (void)x1(a);
      (void)x8(a);
    }

    TEST_METHOD(equalityTrue) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_f32_8 b(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      Assert::IsTrue(allTrue(a == b), L"equal", LINE_INFO());
    }

    TEST_METHOD(equalityFalse) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_f32_8 b(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 9.0f);
      Assert::IsFalse(allTrue(a == b), L"not all equal", LINE_INFO());
    }

    TEST_METHOD(notEqualDetectsDifference) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_f32_8 b(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 9.0f);
      const auto r = a != b;
      Assert::AreEqual(0, x1(r), L"1!=1 false", LINE_INFO());
      Assert::AreEqual(0, x7(r), L"7!=7 false", LINE_INFO());
      Assert::AreNotEqual(0, x8(r), L"8!=9 true", LINE_INFO());
    }

    TEST_METHOD(notEqualAllSame) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      Assert::IsTrue(allFalse(a != a), L"self != self is all false", LINE_INFO());
    }

    TEST_METHOD(lessThan) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_f32_8 b(2.0f, 2.0f, 4.0f, 4.0f, 6.0f, 6.0f, 8.0f, 8.0f);
      const auto r = a < b;
      Assert::AreNotEqual(0, x1(r), L"1<2 true", LINE_INFO());
      Assert::AreEqual(0, x2(r), L"2<2 false", LINE_INFO());
      Assert::AreNotEqual(0, x5(r), L"5<6 true", LINE_INFO());
      Assert::AreEqual(0, x8(r), L"8<8 false", LINE_INFO());
    }

    TEST_METHOD(greaterThan) {
      const v_f32_8 a(2.0f, 2.0f, 4.0f, 4.0f, 6.0f, 6.0f, 8.0f, 8.0f);
      const v_f32_8 b(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = a > b;
      Assert::AreNotEqual(0, x1(r), L"2>1 true", LINE_INFO());
      Assert::AreEqual(0, x2(r), L"2>2 false", LINE_INFO());
      Assert::AreNotEqual(0, x3(r), L"4>3 true", LINE_INFO());
      Assert::AreEqual(0, x4(r), L"4>4 false", LINE_INFO());
      Assert::AreNotEqual(0, x5(r), L"6>5 true", LINE_INFO());
      Assert::AreEqual(0, x8(r), L"8>8 false", LINE_INFO());
    }

    TEST_METHOD(lessOrEqual) {
      const v_f32_8 a(1.0f, 2.0f, 5.0f, 4.0f, 5.0f, 6.0f, 7.0f, 9.0f);
      const v_f32_8 b(2.0f, 2.0f, 4.0f, 4.0f, 6.0f, 6.0f, 8.0f, 8.0f);
      const auto r = a <= b;
      Assert::AreNotEqual(0, x1(r), L"1<=2 true", LINE_INFO());
      Assert::AreNotEqual(0, x2(r), L"2<=2 true", LINE_INFO());
      Assert::AreEqual(0, x3(r), L"5<=4 false", LINE_INFO());
      Assert::AreNotEqual(0, x4(r), L"4<=4 true", LINE_INFO());
      Assert::AreNotEqual(0, x5(r), L"5<=6 true", LINE_INFO());
      Assert::AreNotEqual(0, x6(r), L"6<=6 true", LINE_INFO());
      Assert::AreNotEqual(0, x7(r), L"7<=8 true", LINE_INFO());
      Assert::AreEqual(0, x8(r), L"9<=8 false", LINE_INFO());
    }

    TEST_METHOD(greaterOrEqual) {
      const v_f32_8 a(1.0f, 2.0f, 5.0f, 4.0f, 5.0f, 6.0f, 7.0f, 9.0f);
      const v_f32_8 b(2.0f, 2.0f, 4.0f, 4.0f, 6.0f, 6.0f, 8.0f, 8.0f);
      const auto r = a >= b;
      Assert::AreEqual(0, x1(r), L"1>=2 false", LINE_INFO());
      Assert::AreNotEqual(0, x2(r), L"2>=2 true", LINE_INFO());
      Assert::AreNotEqual(0, x3(r), L"5>=4 true", LINE_INFO());
      Assert::AreNotEqual(0, x4(r), L"4>=4 true", LINE_INFO());
      Assert::AreEqual(0, x5(r), L"5>=6 false", LINE_INFO());
      Assert::AreNotEqual(0, x6(r), L"6>=6 true", LINE_INFO());
      Assert::AreEqual(0, x7(r), L"7>=8 false", LINE_INFO());
      Assert::AreNotEqual(0, x8(r), L"9>=8 true", LINE_INFO());
    }

    TEST_METHOD(bitwiseAnd) {
      const auto a = MaskAll<v_f32_8>();
      const auto b = Zero<v_f32_8>();
      const auto r = a & b;
      Assert::AreEqual(0.0f, x1(r), L"all & zero = zero", LINE_INFO());
    }

    TEST_METHOD(bitwiseOr) {
      const auto a = MaskAll<v_f32_8>();
      const auto b = Zero<v_f32_8>();
      const auto r = a | b;
      Assert::IsTrue(std::isnan(x1(r)), L"all | zero = all", LINE_INFO());
    }

    TEST_METHOD(bitwiseXor) {
      const auto a = MaskAll<v_f32_8>();
      const auto r = a ^ a;
      Assert::AreEqual(0.0f, x1(r), L"a ^ a = zero x1", LINE_INFO());
      Assert::AreEqual(0.0f, x8(r), L"a ^ a = zero x8", LINE_INFO());
    }

    TEST_METHOD(bitwiseXorWithZero) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const auto r = a ^ Zero<v_f32_8>();
      Assert::AreEqual(1.0f, x1(r), L"a ^ 0 = a x1", LINE_INFO());
      Assert::AreEqual(8.0f, x8(r), L"a ^ 0 = a x8", LINE_INFO());
    }

    TEST_METHOD(logicalNot) {
      const v_f32_8 v(0.0f, 1.0f, 0.0f, 2.0f, 0.0f, 3.0f, 0.0f, 4.0f);
      const auto r = !v;
      Assert::AreNotEqual(0, x1(r), L"!0 true", LINE_INFO());
      Assert::AreEqual(0, x2(r), L"!1 false", LINE_INFO());
      Assert::AreNotEqual(0, x5(r), L"!0 true", LINE_INFO());
      Assert::AreEqual(0, x8(r), L"!4 false", LINE_INFO());
    }

    TEST_METHOD(streamOutput) {
      const v_f32_8 v(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      std::ostringstream ss;
      ss << v;
      Assert::IsFalse(ss.str().empty(), L"stream not empty", LINE_INFO());
    }

    TEST_METHOD(moduloComputes) {
      const v_f32_8 a(5.5f, 6.5f, 7.5f, 8.5f, 9.5f, 10.5f, 11.5f, 12.5f);
      const v_f32_8 b(3.0f);
      const auto r = a % b;
      Assert::AreEqual(2.5f, x1(r), 1e-5f, L"5.5%3", LINE_INFO());
      Assert::AreEqual(0.5f, x2(r), 1e-5f, L"6.5%3", LINE_INFO());
      Assert::AreEqual(0.5f, x8(r), 1e-5f, L"12.5%3", LINE_INFO());
    }

    TEST_METHOD(shiftLeftPerLane) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_i32_8 shift(1);
      const auto r = a << shift;
      // Bit-shift left by 1 on float bit pattern (doubles the exponent contribution)
      // 1.0f = 0x3F800000, << 1 = 0x7F000000 = very large float
      Assert::AreNotEqual(0.0f, x1(r), L"non-zero result", LINE_INFO());
      Assert::AreNotEqual(0.0f, x8(r), L"non-zero result x8", LINE_INFO());
    }

    TEST_METHOD(shiftRightPerLane) {
      const v_f32_8 a(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f);
      const v_i32_8 shift(1);
      const auto r = a >> shift;
      Assert::AreNotEqual(0.0f, x1(r), L"non-zero result", LINE_INFO());
      Assert::AreNotEqual(0.0f, x8(r), L"non-zero result x8", LINE_INFO());
    }
  };
}
