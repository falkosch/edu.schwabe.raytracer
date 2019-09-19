#include "../../Iterate.h"
#include "../../StandardSample.h"

#include <CppUnitTest.h>
#include <vectorization.h>

#include <array>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vectorization
{
    namespace test
    {

        TEST_CLASS(v_f32_4_OperatorsTest)
        {
        public:

            static const std::array<v_f32_4::ValueType, v_f32_4::SIZE> samplePositiveValues() {
                return {
                    Zero<v_f32_4::ValueType>(),
                    One<v_f32_4::ValueType>(),
                    Infinity<v_f32_4::ValueType>(),
                    NotANumber<v_f32_4::ValueType>()
                };
            }

            static const std::array<v_f32_4::ValueType, v_f32_4::SIZE> sampleNegativeValues() {
                return {
                    NegativeZero<v_f32_4::ValueType>(),
                    NegativeOne<v_f32_4::ValueType>(),
                    NegativeInfinity<v_f32_4::ValueType>(),
                    -NotANumber<v_f32_4::ValueType>()
                };
            }

            static const v_f32_4 samplePositiveVector() {
                return StandardSample::ofVectorTypeFromArrayType<v_f32_4>(samplePositiveValues());
            }

            static const v_f32_4 sampleNegativeVector() {
                return StandardSample::ofVectorTypeFromArrayType<v_f32_4>(sampleNegativeValues());
            }

            // "-"
            TEST_METHOD(unaryMinusReturnsNegative)
            {
                Iterate::toExclusiveEnd<VectorSizes::Z>([](auto i, auto message) {
                    Assert::AreEqual(sampleNegativeVector()[i], (-samplePositiveVector())[i], message.c_str());
                });

                // NaN values keep NaN
                Assert::IsTrue(isNaN(w(-samplePositiveVector())), L"'-' op value mismatch", LINE_INFO());
            }

            // "~"
            TEST_METHOD(testOnesComplementOperator)
            {
                auto sample = MaskAll<v_f32_4>();
                auto expected = Zero<v_f32_4>();
                auto actual = ~sample;

                Assert::AreEqual(x(expected), x(actual), L"'~' op value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"'~' op value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual), L"'~' op value mismatch", LINE_INFO());
                Assert::AreEqual(w(expected), w(actual), L"'~' op value mismatch", LINE_INFO());
            }

            // "!"
            TEST_METHOD(testLogicalNegationOperator)
            {
                auto expected = v_f32_4::VectorBoolType(
                    MaskAll<v_f32_4::BoolType>(),
                    0,
                    0,
                    0
                );
                auto actual = !samplePositiveVector();

                Assert::AreEqual(x(expected), x(actual), L"'!' op value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"'!' op value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual), L"'!' op value mismatch", LINE_INFO());
                Assert::AreEqual(w(expected), w(actual), L"'!' op value mismatch", LINE_INFO());
            }

            TEST_METHOD(testAddVectorOperator)
            {
                auto expected = Zero<v_f32_4>();
                auto actual = samplePositiveVector() + sampleNegativeVector();

                Assert::AreEqual(x(expected), x(actual), L"'+' op value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"'+' op value mismatch", LINE_INFO());
                Assert::IsTrue(isNaN(z(actual)), L"'+' op value mismatch", LINE_INFO());
                Assert::IsTrue(isNaN(w(actual)), L"'+' op value mismatch", LINE_INFO());
            }

            TEST_METHOD(testAddLeftHandValueOperator)
            {
                auto expected = v_f32_4(
                    One<v_f32_4::ValueType>(),
                    Two<v_f32_4::ValueType>(),
                    Infinity<v_f32_4::ValueType>()
                );
                auto actual = One<v_f32_4::ValueType>() + samplePositiveVector();

                Assert::AreEqual(x(expected), x(actual), L"'+' op value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"'+' op value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual), L"'+' op value mismatch", LINE_INFO());
                Assert::IsTrue(isNaN(w(actual)), L"'+' op value mismatch", LINE_INFO());
            }

            TEST_METHOD(testAddRightHandValueOperator)
            {
                auto expected = v_f32_4(
                    One<v_f32_4::ValueType>(),
                    Two<v_f32_4::ValueType>(),
                    Infinity<v_f32_4::ValueType>()
                );
                auto actual = samplePositiveVector() + One<v_f32_4::ValueType>();

                Assert::AreEqual(x(expected), x(actual), L"'+' op value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"'+' op value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual), L"'+' op value mismatch", LINE_INFO());
                Assert::IsTrue(isNaN(w(actual)), L"'+' op value mismatch", LINE_INFO());
            }

            TEST_METHOD(testSubtractVectorOperator)
            {
                auto expected = v_f32_4(
                    Zero<v_f32_4::ValueType>(),
                    Two<v_f32_4::ValueType>(),
                    Infinity<v_f32_4::ValueType>()
                );
                auto actual = samplePositiveVector() - sampleNegativeVector();

                Assert::AreEqual(x(expected), x(actual), L"'-' op value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"'-' op value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual), L"'-' op value mismatch", LINE_INFO());
                Assert::IsTrue(isNaN(w(actual)), L"'-' op value mismatch", LINE_INFO());
            }

            TEST_METHOD(testSubtractLeftHandValueOperator)
            {
                auto expected = v_f32_4(
                    One<v_f32_4::ValueType>(),
                    Zero<v_f32_4::ValueType>(),
                    NegativeInfinity<v_f32_4::ValueType>()
                );
                auto actual = One<v_f32_4::ValueType>() - samplePositiveVector();

                Assert::AreEqual(x(expected), x(actual), L"'-' op value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"'-' op value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual), L"'-' op value mismatch", LINE_INFO());
                Assert::IsTrue(isNaN(w(actual)), L"'-' op value mismatch", LINE_INFO());
            }

            TEST_METHOD(testSubtractRightHandValueOperator)
            {
                auto expected = v_f32_4(
                    NegativeOne<v_f32_4::ValueType>(),
                    Zero<v_f32_4::ValueType>(),
                    Infinity<v_f32_4::ValueType>()
                );
                auto actual = samplePositiveVector() - One<v_f32_4::ValueType>();

                Assert::AreEqual(x(expected), x(actual), L"'-' op value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"'-' op value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual), L"'-' op value mismatch", LINE_INFO());
                Assert::IsTrue(isNaN(w(actual)), L"'-' op value mismatch", LINE_INFO());
            }

            TEST_METHOD(testMultiplyComponentWiseVectorOperator)
            {
                auto expected = v_f32_4(sampleNegativeVector());
                auto actual = samplePositiveVector() * sampleNegativeVector();

                Assert::AreEqual(x(expected), x(actual), L"'*' op value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"'*' op value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual), L"'*' op value mismatch", LINE_INFO());
                Assert::IsTrue(isNaN(w(actual)), L"'*' op value mismatch", LINE_INFO());
            }

            TEST_METHOD(testMultiplyLeftHandValueOperator)
            {
                auto expected = v_f32_4(sampleNegativeVector());
                auto actual = NegativeOne<v_f32_4::ValueType>() * samplePositiveVector();

                Assert::AreEqual(x(expected), x(actual), L"'*' op value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"'*' op value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual), L"'*' op value mismatch", LINE_INFO());
                Assert::IsTrue(isNaN(w(actual)), L"'*' op value mismatch", LINE_INFO());
            }

            TEST_METHOD(testMultiplyRightHandValueOperator)
            {
                auto expected = v_f32_4(sampleNegativeVector());
                auto actual = samplePositiveVector() * NegativeOne<v_f32_4::ValueType>();

                Assert::AreEqual(x(expected), x(actual), L"'*' op value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"'*' op value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual), L"'*' op value mismatch", LINE_INFO());
                Assert::IsTrue(isNaN(w(actual)), L"'*' op value mismatch", LINE_INFO());
            }

            TEST_METHOD(testDivideComponentWiseVectorOperator)
            {
                auto expected = v_f32_4(sampleNegativeVector());
                auto actual = samplePositiveVector() / NegativeOne<v_f32_4>();

                Assert::AreEqual(x(expected), x(actual), L"'/' op value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"'/' op value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual), L"'/' op value mismatch", LINE_INFO());
                Assert::IsTrue(isNaN(w(actual)), L"'/' op value mismatch", LINE_INFO());
            }

            TEST_METHOD(testDivideLeftHandValueOperator)
            {
                auto expected = v_f32_4(
                    NegativeInfinity<v_f32_4::ValueType>(),
                    sampleNegativeValues().at(VectorIndices::Y),
                    NegativeZero<v_f32_4::ValueType>()
                );
                auto actual = NegativeOne<v_f32_4>() / samplePositiveVector();

                Assert::AreEqual(x(expected), x(actual), L"'/' op value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"'/' op value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual), L"'/' op value mismatch", LINE_INFO());
                Assert::IsTrue(isNaN(w(actual)), L"'/' op value mismatch", LINE_INFO());
            }

            TEST_METHOD(testDivideRightHandValueOperator)
            {
                auto expected = v_f32_4(sampleNegativeVector());
                auto actual = samplePositiveVector() / NegativeOne<v_f32_4>();

                Assert::AreEqual(x(expected), x(actual), L"'/' op value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"'/' op value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual), L"'/' op value mismatch", LINE_INFO());
                Assert::IsTrue(isNaN(w(actual)), L"'/' op value mismatch", LINE_INFO());
            }



            TEST_METHOD(testEqualsOperator)
            {
                auto actual = samplePositiveVector() == sampleNegativeVector();
                auto expected = v_f32_4::VectorBoolType(MaskAll<v_f32_4::BoolType>(), 0, 0, 0);

                Assert::AreEqual(x(expected), x(actual), L"Equals value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"Equals value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual), L"Equals value mismatch", LINE_INFO());
                Assert::AreEqual(w(expected), w(actual), L"Equals value mismatch", LINE_INFO());

                auto sampleVector3 = v_f32_4(samplePositiveVector());
                actual = samplePositiveVector() == sampleVector3;
                expected = v_f32_4::VectorBoolType(MaskAll<v_f32_4::BoolType>(), MaskAll<v_f32_4::BoolType>(), MaskAll<v_f32_4::BoolType>(), 0);

                Assert::AreEqual(x(expected), x(actual), L"Equals value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"Equals value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual), L"Equals value mismatch", LINE_INFO());
                Assert::AreEqual(w(expected), w(actual), L"Equals value mismatch", LINE_INFO());
            }

            TEST_METHOD(testNotEqualsOperator)
            {
                auto actual = samplePositiveVector() != sampleNegativeVector();
                auto expected = v_f32_4::VectorBoolType(0, MaskAll<v_f32_4::BoolType>(), MaskAll<v_f32_4::BoolType>(), MaskAll<v_f32_4::BoolType>());

                Assert::AreEqual(x(expected), x(actual), L"NotEquals value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"NotEquals value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual), L"NotEquals value mismatch", LINE_INFO());
                Assert::AreEqual(w(expected), w(actual), L"NotEquals value mismatch", LINE_INFO());

                auto sampleVector3 = v_f32_4(samplePositiveVector());
                actual = samplePositiveVector() != sampleVector3;

                expected = v_f32_4::VectorBoolType(0, 0, 0, MaskAll<v_f32_4::BoolType>());
                Assert::AreEqual(x(expected), x(actual), L"Equals value mismatch", LINE_INFO());
                Assert::AreEqual(y(expected), y(actual), L"Equals value mismatch", LINE_INFO());
                Assert::AreEqual(z(expected), z(actual), L"Equals value mismatch", LINE_INFO());
                Assert::AreEqual(w(expected), w(actual), L"Equals value mismatch", LINE_INFO());
            }

        };

    }
}
