#include "vectorization_tests.h"

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
      Assert::AreEqual(w(expected), w(actual), L"Equals value mismatch", LINE_INFO());
    }

    TEST_METHOD(testNotEqualsOperator) {
      auto actual = sampleVector1() != sampleVector2();
      auto expected = v_f32_4::VectorBoolType(
          0, MaskAll<v_f32_4::BoolType>(), MaskAll<v_f32_4::BoolType>(), MaskAll<v_f32_4::BoolType>()
      );

      Assert::AreEqual(x(expected), x(actual), L"NotEquals value mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"NotEquals value mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(actual), L"NotEquals value mismatch", LINE_INFO());
      Assert::AreEqual(w(expected), w(actual), L"NotEquals value mismatch", LINE_INFO());

      const auto sampleVector3 = v_f32_4(sampleVector1());
      actual = sampleVector1() != sampleVector3;

      expected = v_f32_4::VectorBoolType(0, 0, 0, MaskAll<v_f32_4::BoolType>());
      Assert::AreEqual(x(expected), x(actual), L"Equals value mismatch", LINE_INFO());
      Assert::AreEqual(y(expected), y(actual), L"Equals value mismatch", LINE_INFO());
      Assert::AreEqual(z(expected), z(actual), L"Equals value mismatch", LINE_INFO());
      Assert::AreEqual(w(expected), w(actual), L"Equals value mismatch", LINE_INFO());
    }
  };
}
