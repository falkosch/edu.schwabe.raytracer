#include "vectorization_tests.h"

#include <cmath>

namespace vectorization::test {
  TEST_CLASS(v_f32_4_OperatorsTest) {
  public:
    static std::array<v_f32_4::ValueType, v_f32_4::SIZE> sample1() {
      return {
          Zero<v_f32_4::ValueType>(), One<v_f32_4::ValueType>(), Infinity<v_f32_4::ValueType>(),
          NotANumber<v_f32_4::ValueType>()
      };
    }

    static v_f32_4 sampleVector1() {
      const auto sampleComponents = sample1();
      return v_f32_4(
          sampleComponents.at(VectorIndices::X), sampleComponents.at(VectorIndices::Y),
          sampleComponents.at(VectorIndices::Z), sampleComponents.at(VectorIndices::W)
      );
    }

    static std::array<v_f32_4::ValueType, v_f32_4::SIZE> sample2() {
      return {
          NegativeZero<v_f32_4::ValueType>(), NegativeOne<v_f32_4::ValueType>(), NegativeInfinity<v_f32_4::ValueType>(),
          -NotANumber<v_f32_4::ValueType>()
      };
    }

    static v_f32_4 sampleVector2() {
      const auto sampleComponents = sample2();
      return v_f32_4(
          sampleComponents.at(VectorIndices::X), sampleComponents.at(VectorIndices::Y),
          sampleComponents.at(VectorIndices::Z), sampleComponents.at(VectorIndices::W)
      );
    }

    // "-"
    TEST_METHOD(testNegationOperator) {
      const auto expected =
          v_f32_4(-sample1().at(VectorIndices::X), -sample1().at(VectorIndices::Y), -sample1().at(VectorIndices::Z));
      const auto actual = -sampleVector1();

      Assert::AreEqual(x(expected), x(actual), L"'-' op value mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"'-' op value mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(actual), L"'-' op value mismatch", LINE_INFO());

      Assert::IsTrue(isNaN(w(actual)), L"'-' op value mismatch", LINE_INFO());
    }

    // "~"
    TEST_METHOD(testOnesComplementOperator) {
      const auto sample = MaskAll<v_f32_4>();
      const auto expected = Zero<v_f32_4>();
      const auto actual = ~sample;

      Assert::AreEqual(x(expected), x(actual), L"'~' op value mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"'~' op value mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(actual), L"'~' op value mismatch", LINE_INFO());
      Assert::AreEqual(w(expected), w(actual), L"'~' op value mismatch", LINE_INFO());
    }

    // "!"
    TEST_METHOD(testLogicalNegationOperator) {
      const auto expected = v_f32_4::VectorBoolType(MaskAll<v_f32_4::BoolType>(), 0, 0, 0);
      const auto actual = !sampleVector1();

      Assert::AreEqual(x(expected), x(actual), L"'!' op value mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"'!' op value mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(actual), L"'!' op value mismatch", LINE_INFO());
      Assert::AreEqual(w(expected), w(actual), L"'!' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testAddVectorOperator) {
      const auto expected = Zero<v_f32_4>();
      const auto actual = sampleVector1() + sampleVector2();

      Assert::AreEqual(x(expected), x(actual), L"'+' op value mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"'+' op value mismatch", LINE_INFO());
      Assert::IsTrue(isNaN(z(actual)), L"'+' op value mismatch", LINE_INFO());
      Assert::IsTrue(isNaN(w(actual)), L"'+' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testAddLeftHandValueOperator) {
      const auto expected =
          v_f32_4(One<v_f32_4::ValueType>(), Two<v_f32_4::ValueType>(), Infinity<v_f32_4::ValueType>());
      const auto actual = One<v_f32_4::ValueType>() + sampleVector1();

      Assert::AreEqual(x(expected), x(actual), L"'+' op value mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"'+' op value mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(actual), L"'+' op value mismatch", LINE_INFO());
      Assert::IsTrue(isNaN(w(actual)), L"'+' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testAddRightHandValueOperator) {
      const auto expected =
          v_f32_4(One<v_f32_4::ValueType>(), Two<v_f32_4::ValueType>(), Infinity<v_f32_4::ValueType>());
      const auto actual = sampleVector1() + One<v_f32_4::ValueType>();

      Assert::AreEqual(x(expected), x(actual), L"'+' op value mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"'+' op value mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(actual), L"'+' op value mismatch", LINE_INFO());
      Assert::IsTrue(isNaN(w(actual)), L"'+' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testSubtractVectorOperator) {
      const auto expected =
          v_f32_4(Zero<v_f32_4::ValueType>(), Two<v_f32_4::ValueType>(), Infinity<v_f32_4::ValueType>());
      const auto actual = sampleVector1() - sampleVector2();

      Assert::AreEqual(x(expected), x(actual), L"'-' op value mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"'-' op value mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(actual), L"'-' op value mismatch", LINE_INFO());
      Assert::IsTrue(isNaN(w(actual)), L"'-' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testSubtractLeftHandValueOperator) {
      const auto expected =
          v_f32_4(One<v_f32_4::ValueType>(), Zero<v_f32_4::ValueType>(), NegativeInfinity<v_f32_4::ValueType>());
      const auto actual = One<v_f32_4::ValueType>() - sampleVector1();

      Assert::AreEqual(x(expected), x(actual), L"'-' op value mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"'-' op value mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(actual), L"'-' op value mismatch", LINE_INFO());
      Assert::IsTrue(isNaN(w(actual)), L"'-' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testSubtractRightHandValueOperator) {
      const auto expected =
          v_f32_4(NegativeOne<v_f32_4::ValueType>(), Zero<v_f32_4::ValueType>(), Infinity<v_f32_4::ValueType>());
      const auto actual = sampleVector1() - One<v_f32_4::ValueType>();

      Assert::AreEqual(x(expected), x(actual), L"'-' op value mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"'-' op value mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(actual), L"'-' op value mismatch", LINE_INFO());
      Assert::IsTrue(isNaN(w(actual)), L"'-' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testMultiplyComponentWiseVectorOperator) {
      const auto expected = v_f32_4(sampleVector2());
      const auto actual = sampleVector1() * sampleVector2();

      Assert::AreEqual(x(expected), x(actual), L"'*' op value mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"'*' op value mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(actual), L"'*' op value mismatch", LINE_INFO());
      Assert::IsTrue(isNaN(w(actual)), L"'*' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testMultiplyLeftHandValueOperator) {
      const auto expected = v_f32_4(sampleVector2());
      const auto actual = NegativeOne<v_f32_4::ValueType>() * sampleVector1();

      Assert::AreEqual(x(expected), x(actual), L"'*' op value mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"'*' op value mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(actual), L"'*' op value mismatch", LINE_INFO());
      Assert::IsTrue(isNaN(w(actual)), L"'*' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testMultiplyRightHandValueOperator) {
      const auto expected = v_f32_4(sampleVector2());
      const auto actual = sampleVector1() * NegativeOne<v_f32_4::ValueType>();

      Assert::AreEqual(x(expected), x(actual), L"'*' op value mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"'*' op value mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(actual), L"'*' op value mismatch", LINE_INFO());
      Assert::IsTrue(isNaN(w(actual)), L"'*' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testDivideComponentWiseVectorOperator) {
      const auto expected = v_f32_4(sampleVector2());
      const auto actual = sampleVector1() / NegativeOne<v_f32_4>();

      Assert::AreEqual(x(expected), x(actual), L"'/' op value mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"'/' op value mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(actual), L"'/' op value mismatch", LINE_INFO());
      Assert::IsTrue(isNaN(w(actual)), L"'/' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testDivideLeftHandValueOperator) {
      const auto expected = v_f32_4(
          NegativeInfinity<v_f32_4::ValueType>(), sample2().at(VectorIndices::Y), NegativeZero<v_f32_4::ValueType>()
      );
      const auto actual = NegativeOne<v_f32_4>() / sampleVector1();

      Assert::AreEqual(x(expected), x(actual), L"'/' op value mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"'/' op value mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(actual), L"'/' op value mismatch", LINE_INFO());
      Assert::IsTrue(isNaN(w(actual)), L"'/' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testDivideRightHandValueOperator) {
      const auto expected = v_f32_4(sampleVector2());
      const auto actual = sampleVector1() / NegativeOne<v_f32_4>();

      Assert::AreEqual(x(expected), x(actual), L"'/' op value mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"'/' op value mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(actual), L"'/' op value mismatch", LINE_INFO());
      Assert::IsTrue(isNaN(w(actual)), L"'/' op value mismatch", LINE_INFO());
    }

    TEST_METHOD(testEqualsOperator) {
      auto actual = sampleVector1() == sampleVector2();
      auto expected = v_f32_4::VectorBoolType(MaskAll<v_f32_4::BoolType>(), 0, 0, 0);

      Assert::AreEqual(x(expected), x(actual), L"Equals value mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"Equals value mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(actual), L"Equals value mismatch", LINE_INFO());
      Assert::AreEqual(w(expected), w(actual), L"Equals value mismatch", LINE_INFO());

      const auto sampleVector3 = v_f32_4(sampleVector1());
      actual = sampleVector1() == sampleVector3;
      expected = v_f32_4::VectorBoolType(
          MaskAll<v_f32_4::BoolType>(), MaskAll<v_f32_4::BoolType>(), MaskAll<v_f32_4::BoolType>(), 0
      );

      Assert::AreEqual(x(expected), x(actual), L"Equals value mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"Equals value mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(actual), L"Equals value mismatch", LINE_INFO());
      // W holds NaN; /fp:fast makes NaN == NaN undefined, so verify NaN is preserved instead.
      Assert::IsTrue(std::isnan(w(sampleVector3)), L"W should still be NaN after copy", LINE_INFO());
    }

    TEST_METHOD(testNotEqualsOperator) {
      auto actual = sampleVector1() != sampleVector2();
      auto expected = v_f32_4::VectorBoolType(
          0, MaskAll<v_f32_4::BoolType>(), MaskAll<v_f32_4::BoolType>(), MaskAll<v_f32_4::BoolType>()
      );

      Assert::AreEqual(x(expected), x(actual), L"NotEquals value mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"NotEquals value mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(actual), L"NotEquals value mismatch", LINE_INFO());
      // W holds NaN vs -NaN; /fp:fast makes NaN != NaN undefined, so verify both are NaN instead.
      Assert::IsTrue(std::isnan(w(sampleVector1())), L"W should be NaN", LINE_INFO());
      Assert::IsTrue(std::isnan(w(sampleVector2())), L"W should be NaN", LINE_INFO());

      const auto sampleVector3 = v_f32_4(sampleVector1());
      actual = sampleVector1() != sampleVector3;

      expected = v_f32_4::VectorBoolType(0, 0, 0, MaskAll<v_f32_4::BoolType>());
      Assert::AreEqual(x(expected), x(actual), L"NotEquals value mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"NotEquals value mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(actual), L"NotEquals value mismatch", LINE_INFO());
      Assert::IsTrue(std::isnan(w(sampleVector3)), L"W should still be NaN after copy", LINE_INFO());
    }

    // "%"
    TEST_METHOD(testModuloOperator) {
      const v_f32_4 a{5.5f, 7.0f, 10.3f, 3.0f};
      const v_f32_4 b{2.0f, 3.0f, 4.0f, 1.5f};
      const auto actual = a % b;

      // modulo(a, b) = a - b * floor(a / b)
      Assert::AreEqual(std::fmod(5.5f, 2.0f), x(actual), L"'%' op X mismatch", LINE_INFO());
      Assert::AreEqual(std::fmod(7.0f, 3.0f), y(actual), L"'%' op Y mismatch", LINE_INFO());
      Assert::AreEqual(std::fmod(10.3f, 4.0f), z(actual), L"'%' op Z mismatch", LINE_INFO());
      Assert::AreEqual(std::fmod(3.0f, 1.5f), w(actual), L"'%' op W mismatch", LINE_INFO());
    }

    // "&"
    TEST_METHOD(testBitwiseAndOperator) {
      const auto a = MaskAll<v_f32_4>();
      const auto b = MaskX<v_f32_4>();
      const auto actual = a & b;

      Assert::AreEqual(x(b), x(actual), L"'&' op X mismatch", LINE_INFO());
      Assert::AreEqual(y(b), y(actual), L"'&' op Y mismatch", LINE_INFO());
      Assert::AreEqual(z(b), z(actual), L"'&' op Z mismatch", LINE_INFO());
      Assert::AreEqual(w(b), w(actual), L"'&' op W mismatch", LINE_INFO());
    }

    // "|"
    TEST_METHOD(testBitwiseOrOperator) {
      const auto a = MaskX<v_f32_4>();
      const auto b = MaskY<v_f32_4>();
      const auto expected = MaskXY<v_f32_4>();
      const auto actual = a | b;

      Assert::AreEqual(x(expected), x(actual), L"'|' op X mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"'|' op Y mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(actual), L"'|' op Z mismatch", LINE_INFO());
      Assert::AreEqual(w(expected), w(actual), L"'|' op W mismatch", LINE_INFO());
    }

    // "^"
    TEST_METHOD(testBitwiseXorOperator) {
      const auto a = MaskAll<v_f32_4>();
      const auto expected = Zero<v_f32_4>();
      const auto actual = a ^ a;

      Assert::AreEqual(x(expected), x(actual), L"'^' op X mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"'^' op Y mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(actual), L"'^' op Z mismatch", LINE_INFO());
      Assert::AreEqual(w(expected), w(actual), L"'^' op W mismatch", LINE_INFO());
    }

    // "<<"
    TEST_METHOD(testLeftShiftOperator) {
      const v_f32_4 given = One<v_f32_4>();
      // _mm_sll_epi32 reads the shift count from the low 64-bit element
      const v_f32_4::VectorBoolType shift{1, 0, 0, 0};
      const auto actual = given << shift;
      // Shifting the bit pattern of 1.0f (0x3F800000) left by 1 gives 0x7F000000
      const auto expected = v_f32_4(_mm_castsi128_ps(_mm_slli_epi32(_mm_castps_si128(given.components), 1)));

      Assert::AreEqual(x(expected), x(actual), L"'<<' op X mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"'<<' op Y mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(actual), L"'<<' op Z mismatch", LINE_INFO());
      Assert::AreEqual(w(expected), w(actual), L"'<<' op W mismatch", LINE_INFO());
    }

    // ">>"
    TEST_METHOD(testRightShiftOperator) {
      const v_f32_4 given = One<v_f32_4>();
      // _mm_srl_epi32 reads the shift count from the low 64-bit element
      const v_f32_4::VectorBoolType shift{1, 0, 0, 0};
      const auto actual = given >> shift;
      // Shifting the bit pattern of 1.0f (0x3F800000) right by 1 gives 0x1FC00000
      const auto expected = v_f32_4(_mm_castsi128_ps(_mm_srli_epi32(_mm_castps_si128(given.components), 1)));

      Assert::AreEqual(x(expected), x(actual), L"'>>' op X mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"'>>' op Y mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(actual), L"'>>' op Z mismatch", LINE_INFO());
      Assert::AreEqual(w(expected), w(actual), L"'>>' op W mismatch", LINE_INFO());
    }

    // "<"
    TEST_METHOD(testLessThanOperator) {
      const v_f32_4 a{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 b{2.0f, 2.0f, 1.0f, 5.0f};
      const auto actual = a < b;
      // X: 1<2 true, Y: 2<2 false, Z: 3<1 false, W: 4<5 true

      Assert::AreEqual(MaskAll<v_f32_4::BoolType>(), x(actual), L"'<' X mismatch", LINE_INFO());
      Assert::AreEqual(v_f32_4::BoolType{0}, y(actual), L"'<' Y mismatch", LINE_INFO());
      Assert::AreEqual(v_f32_4::BoolType{0}, z(actual), L"'<' Z mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_f32_4::BoolType>(), w(actual), L"'<' W mismatch", LINE_INFO());
    }

    // ">"
    TEST_METHOD(testGreaterThanOperator) {
      const v_f32_4 a{2.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 b{1.0f, 2.0f, 5.0f, 1.0f};
      const auto actual = a > b;
      // X: 2>1 true, Y: 2>2 false, Z: 3>5 false, W: 4>1 true

      Assert::AreEqual(MaskAll<v_f32_4::BoolType>(), x(actual), L"'>' X mismatch", LINE_INFO());
      Assert::AreEqual(v_f32_4::BoolType{0}, y(actual), L"'>' Y mismatch", LINE_INFO());
      Assert::AreEqual(v_f32_4::BoolType{0}, z(actual), L"'>' Z mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_f32_4::BoolType>(), w(actual), L"'>' W mismatch", LINE_INFO());
    }

    // "<="
    TEST_METHOD(testLessThanOrEqualOperator) {
      const v_f32_4 a{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 b{2.0f, 2.0f, 1.0f, 5.0f};
      const auto actual = a <= b;
      // X: 1<=2 true, Y: 2<=2 true, Z: 3<=1 false, W: 4<=5 true

      Assert::AreEqual(MaskAll<v_f32_4::BoolType>(), x(actual), L"'<=' X mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_f32_4::BoolType>(), y(actual), L"'<=' Y mismatch", LINE_INFO());
      Assert::AreEqual(v_f32_4::BoolType{0}, z(actual), L"'<=' Z mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_f32_4::BoolType>(), w(actual), L"'<=' W mismatch", LINE_INFO());
    }

    // ">="
    TEST_METHOD(testGreaterThanOrEqualOperator) {
      const v_f32_4 a{1.0f, 2.0f, 3.0f, 4.0f};
      const v_f32_4 b{2.0f, 2.0f, 1.0f, 5.0f};
      const auto actual = a >= b;
      // X: 1>=2 false, Y: 2>=2 true, Z: 3>=1 true, W: 4>=5 false

      Assert::AreEqual(v_f32_4::BoolType{0}, x(actual), L"'>=' X mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_f32_4::BoolType>(), y(actual), L"'>=' Y mismatch", LINE_INFO());
      Assert::AreEqual(MaskAll<v_f32_4::BoolType>(), z(actual), L"'>=' Z mismatch", LINE_INFO());
      Assert::AreEqual(v_f32_4::BoolType{0}, w(actual), L"'>=' W mismatch", LINE_INFO());
    }

    // "+="
    TEST_METHOD(testCompoundAddAssignOperator) {
      auto a = v_f32_4{2.0f, 3.0f, 4.0f, 5.0f};
      const v_f32_4 b{1.0f, 2.0f, 3.0f, 4.0f};
      const auto expected = a + b;
      a += b;

      Assert::AreEqual(x(expected), x(a), L"'+=' X mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(a), L"'+=' Y mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(a), L"'+=' Z mismatch", LINE_INFO());
      Assert::AreEqual(w(expected), w(a), L"'+=' W mismatch", LINE_INFO());
    }

    // "-="
    TEST_METHOD(testCompoundSubtractAssignOperator) {
      auto a = v_f32_4{10.0f, 20.0f, 30.0f, 40.0f};
      const v_f32_4 b{1.0f, 2.0f, 3.0f, 4.0f};
      const auto expected = a - b;
      a -= b;

      Assert::AreEqual(x(expected), x(a), L"'-=' X mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(a), L"'-=' Y mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(a), L"'-=' Z mismatch", LINE_INFO());
      Assert::AreEqual(w(expected), w(a), L"'-=' W mismatch", LINE_INFO());
    }

    // "*="
    TEST_METHOD(testCompoundMultiplyAssignOperator) {
      auto a = v_f32_4{2.0f, 3.0f, 4.0f, 5.0f};
      const v_f32_4 b{3.0f, 2.0f, 1.5f, 2.0f};
      const auto expected = a * b;
      a *= b;

      Assert::AreEqual(x(expected), x(a), L"'*=' X mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(a), L"'*=' Y mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(a), L"'*=' Z mismatch", LINE_INFO());
      Assert::AreEqual(w(expected), w(a), L"'*=' W mismatch", LINE_INFO());
    }

    // "/="
    TEST_METHOD(testCompoundDivideAssignOperator) {
      auto a = v_f32_4{10.0f, 20.0f, 30.0f, 40.0f};
      const v_f32_4 b{2.0f, 4.0f, 5.0f, 8.0f};
      const auto expected = a / b;
      a /= b;

      Assert::AreEqual(x(expected), x(a), L"'/=' X mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(a), L"'/=' Y mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(a), L"'/=' Z mismatch", LINE_INFO());
      Assert::AreEqual(w(expected), w(a), L"'/=' W mismatch", LINE_INFO());
    }

    // "%="
    TEST_METHOD(testCompoundModuloAssignOperator) {
      auto a = v_f32_4{5.5f, 7.0f, 10.3f, 3.0f};
      const v_f32_4 b{2.0f, 3.0f, 4.0f, 1.5f};
      const auto expected = a % b;
      a %= b;

      Assert::AreEqual(x(expected), x(a), L"'%%=' X mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(a), L"'%%=' Y mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(a), L"'%%=' Z mismatch", LINE_INFO());
      Assert::AreEqual(w(expected), w(a), L"'%%=' W mismatch", LINE_INFO());
    }

    // "&="
    TEST_METHOD(testCompoundBitwiseAndAssignOperator) {
      auto a = MaskAll<v_f32_4>();
      const auto b = MaskX<v_f32_4>();
      const auto expected = a & b;
      a &= b;

      Assert::AreEqual(x(expected), x(a), L"'&=' X mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(a), L"'&=' Y mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(a), L"'&=' Z mismatch", LINE_INFO());
      Assert::AreEqual(w(expected), w(a), L"'&=' W mismatch", LINE_INFO());
    }

    // "|="
    TEST_METHOD(testCompoundBitwiseOrAssignOperator) {
      auto a = MaskX<v_f32_4>();
      const auto b = MaskY<v_f32_4>();
      const auto expected = a | b;
      a |= b;

      Assert::AreEqual(x(expected), x(a), L"'|=' X mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(a), L"'|=' Y mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(a), L"'|=' Z mismatch", LINE_INFO());
      Assert::AreEqual(w(expected), w(a), L"'|=' W mismatch", LINE_INFO());
    }

    // "^="
    TEST_METHOD(testCompoundBitwiseXorAssignOperator) {
      auto a = MaskAll<v_f32_4>();
      const auto b = MaskAll<v_f32_4>();
      const auto expected = a ^ b;
      a ^= b;

      Assert::AreEqual(x(expected), x(a), L"'^=' X mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(a), L"'^=' Y mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(a), L"'^=' Z mismatch", LINE_INFO());
      Assert::AreEqual(w(expected), w(a), L"'^=' W mismatch", LINE_INFO());
    }

    // "<<="
    TEST_METHOD(testCompoundLeftShiftAssignOperator) {
      auto a = One<v_f32_4>();
      const v_f32_4::VectorBoolType b{1, 0, 0, 0};
      const auto expected = a << b;
      a <<= b;

      Assert::AreEqual(x(expected), x(a), L"'<<=' X mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(a), L"'<<=' Y mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(a), L"'<<=' Z mismatch", LINE_INFO());
      Assert::AreEqual(w(expected), w(a), L"'<<=' W mismatch", LINE_INFO());
    }

    // ">>="
    TEST_METHOD(testCompoundRightShiftAssignOperator) {
      auto a = One<v_f32_4>();
      const v_f32_4::VectorBoolType b{1, 0, 0, 0};
      const auto expected = a >> b;
      a >>= b;

      Assert::AreEqual(x(expected), x(a), L"'>>=' X mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(a), L"'>>=' Y mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(a), L"'>>=' Z mismatch", LINE_INFO());
      Assert::AreEqual(w(expected), w(a), L"'>>=' W mismatch", LINE_INFO());
    }
  };
}
