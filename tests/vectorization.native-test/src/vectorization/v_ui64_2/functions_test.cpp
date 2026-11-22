#include "vectorization_tests.h"

namespace vectorization::test
{
    TEST_CLASS (v_ui64_2_FunctionsTest)
    {
        public
        :
        TEST_METHOD(computesSqr)
        {
            const v_ui64_2 given{UInt_64{3}, UInt_64{5}};
            const v_ui64_2 expected{UInt_64{9}, UInt_64{25}};
            const auto actual = sqr(given);
            Assert::IsTrue(allTrue(expected == actual), L"sqr value mismatch", LINE_INFO());
        }

        TEST_METHOD(computesComponentWiseMin)
        {
            const v_ui64_2 a{UInt_64{1}, UInt_64{5}};
            const v_ui64_2 b{UInt_64{4}, UInt_64{2}};
            const v_ui64_2 expected{UInt_64{1}, UInt_64{2}};
            const auto actual = min(a, b);
            Assert::IsTrue(allTrue(expected == actual), L"min value mismatch", LINE_INFO());
        }

        TEST_METHOD(computesScalarMin)
        {
            const v_ui64_2 given{UInt_64{3}, UInt_64{1}};
            Assert::AreEqual(UInt_64{1}, min(given), L"scalar min mismatch", LINE_INFO());
        }

        TEST_METHOD(computesMinVector)
        {
            const v_ui64_2 given{UInt_64{3}, UInt_64{1}};
            const auto actual = minv(given);
            Assert::AreEqual(UInt_64{1}, x(actual), L"minv X mismatch", LINE_INFO());
            Assert::AreEqual(UInt_64{1}, y(actual), L"minv Y mismatch", LINE_INFO());
        }

        TEST_METHOD(computesComponentWiseMax)
        {
            const v_ui64_2 a{UInt_64{1}, UInt_64{5}};
            const v_ui64_2 b{UInt_64{4}, UInt_64{2}};
            const v_ui64_2 expected{UInt_64{4}, UInt_64{5}};
            const auto actual = max(a, b);
            Assert::IsTrue(allTrue(expected == actual), L"max value mismatch", LINE_INFO());
        }

        TEST_METHOD(computesScalarMax)
        {
            const v_ui64_2 given{UInt_64{3}, UInt_64{7}};
            Assert::AreEqual(UInt_64{7}, max(given), L"scalar max mismatch", LINE_INFO());
        }

        TEST_METHOD(computesMaxVector)
        {
            const v_ui64_2 given{UInt_64{3}, UInt_64{7}};
            const auto actual = maxv(given);
            Assert::AreEqual(UInt_64{7}, x(actual), L"maxv X mismatch", LINE_INFO());
            Assert::AreEqual(UInt_64{7}, y(actual), L"maxv Y mismatch", LINE_INFO());
        }

        TEST_METHOD(clampsValues)
        {
            const v_ui64_2 given{UInt_64{0}, UInt_64{15}};
            const v_ui64_2 lo{UInt_64{1}, UInt_64{1}};
            const v_ui64_2 hi{UInt_64{10}, UInt_64{10}};
            const v_ui64_2 expected{UInt_64{1}, UInt_64{10}};
            const auto actual = clamp(given, lo, hi);
            Assert::IsTrue(allTrue(expected == actual), L"clamp value mismatch", LINE_INFO());
        }

        TEST_METHOD(checksAnyTrue)
        {
            Assert::IsTrue(anyTrue(v_ui64_2{UInt_64{1}, UInt_64{0}}), L"anyTrue should be true", LINE_INFO());
            Assert::IsTrue(anyTrue(v_ui64_2{UInt_64{0}, UInt_64{1}}), L"anyTrue should be true", LINE_INFO());
            Assert::IsFalse(anyTrue(Zero<v_ui64_2>()), L"anyTrue should be false", LINE_INFO());
        }

        TEST_METHOD(checksAllTrue)
        {
            Assert::IsTrue(allTrue(MaskAll<v_ui64_2>()), L"allTrue should be true", LINE_INFO());
            Assert::IsFalse(allTrue(v_ui64_2{MaskAll<v_ui64_2::BoolType>(), UInt_64{0}}), L"allTrue should be false",
                            LINE_INFO());
        }

        TEST_METHOD(checksAnyFalse)
        {
            Assert::IsTrue(anyFalse(v_ui64_2{MaskAll<v_ui64_2::BoolType>(), UInt_64{0}}), L"anyFalse should be true",
                           LINE_INFO());
            Assert::IsFalse(anyFalse(MaskAll<v_ui64_2>()), L"anyFalse should be false", LINE_INFO());
        }

        TEST_METHOD(checksAllFalse)
        {
            Assert::IsTrue(allFalse(Zero<v_ui64_2>()), L"allFalse should be true", LINE_INFO());
            Assert::IsFalse(allFalse(v_ui64_2{UInt_64{0}, UInt_64{1}}), L"allFalse should be false", LINE_INFO());
        }

        TEST_METHOD(computesLengthVector)
        {
            const v_ui64_2 given{UInt_64{3}, UInt_64{7}};
            const auto actual = lengthv(given);
            Assert::AreEqual(UInt_64{10}, x(actual), L"lengthv X mismatch", LINE_INFO());
            Assert::AreEqual(UInt_64{10}, y(actual), L"lengthv Y mismatch", LINE_INFO());
        }

        TEST_METHOD(computesScalarLength)
        {
            const v_ui64_2 given{UInt_64{3}, UInt_64{7}};
            Assert::AreEqual(UInt_64{10}, length(given), L"length mismatch", LINE_INFO());
        }

        TEST_METHOD(computesDistance)
        {
            const v_ui64_2 a{UInt_64{10}, UInt_64{20}};
            const v_ui64_2 b{UInt_64{7}, UInt_64{15}};
            Assert::AreEqual(UInt_64{8}, distance(a, b), L"distance mismatch", LINE_INFO());
        }

        TEST_METHOD(computesDistanceWithoutWrapAround)
        {
            // Components of b above those of a must not wrap around.
            const v_ui64_2 a{UInt_64{10}, UInt_64{15}};
            const v_ui64_2 b{UInt_64{7}, UInt_64{20}};
            Assert::AreEqual(UInt_64{8}, distance(a, b), L"distance mismatch", LINE_INFO());
            Assert::AreEqual(UInt_64{8}, distance(b, a), L"distance is symmetric", LINE_INFO());
            Assert::IsTrue(allTrue(v_ui64_2{UInt_64{8}} == distancev(a, b)), L"distancev mismatch", LINE_INFO());
        }

        TEST_METHOD(broadcastsVectorMinAndMax)
        {
            const v_ui64_2 given{UInt_64{1}, UInt_64{7}};
            Assert::IsTrue(allTrue(v_ui64_2{UInt_64{1}} == minv(given)), L"minv mismatch", LINE_INFO());
            const v_ui64_2 swapped{UInt_64{7}, UInt_64{1}};
            Assert::IsTrue(allTrue(v_ui64_2{UInt_64{7}} == maxv(swapped)), L"maxv mismatch", LINE_INFO());
        }

        TEST_METHOD(computesHorizontalSum)
        {
            const v_ui64_2 given{UInt_64{3000000000}, UInt_64{5000000000}};
            Assert::AreEqual(UInt_64{8000000000}, horizontalSum(given), L"horizontalSum mismatch", LINE_INFO());
            Assert::IsTrue(
                allTrue(v_ui64_2{UInt_64{8000000000}} == horizontalSumv(given)), L"horizontalSumv mismatch", LINE_INFO()
            );
        }

        TEST_METHOD(computesDotProduct)
        {
            const v_ui64_2 a{UInt_64{3}, UInt_64{4}};
            const v_ui64_2 b{UInt_64{5}, UInt_64{6}};
            Assert::AreEqual(UInt_64{39}, dot(a, b), L"dot mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_ui64_2{UInt_64{39}} == dotv(a, b)), L"dotv mismatch", LINE_INFO());
        }

        TEST_METHOD(computesMultiplyAddFamily)
        {
            const v_ui64_2 a{UInt_64{3}, UInt_64{4}};
            const v_ui64_2 b{UInt_64{50}, UInt_64{60}};
            const v_ui64_2 c{UInt_64{10}, UInt_64{20}};
            // a * b = {150, 240}
            Assert::IsTrue(
                allTrue(v_ui64_2{UInt_64{160}, UInt_64{260}} == multiplyAdd(a, b, c)), L"multiplyAdd", LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_ui64_2{UInt_64{140}, UInt_64{220}} == multiplySub(a, b, c)), L"multiplySub", LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_ui64_2{UInt_64{140}, UInt_64{260}} == multiplyAddSub(a, b, c)), L"multiplyAddSub",
                LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_ui64_2{UInt_64{160}, UInt_64{220}} == multiplySubAdd(a, b, c)), L"multiplySubAdd",
                LINE_INFO()
            );
            const v_ui64_2 large{UInt_64{1000}};
            Assert::IsTrue(
                allTrue(v_ui64_2{UInt_64{850}, UInt_64{760}} == negativeMultiplyAdd(a, b, large)),
                L"negativeMultiplyAdd", LINE_INFO()
            );
            // -(a * b) - c wraps around
            Assert::IsTrue(
                allTrue(v_ui64_2{UInt_64(-160), UInt_64(-260)} == negativeMultiplySub(a, b, c)),
                L"negativeMultiplySub", LINE_INFO()
            );
        }

        TEST_METHOD(computesAddSubtract)
        {
            const v_ui64_2 a{UInt_64{10}, UInt_64{20}};
            const v_ui64_2 b{UInt_64{1}, UInt_64{2}};
            const v_ui64_2 expected{UInt_64{9}, UInt_64{22}};
            Assert::IsTrue(allTrue(expected == addSubtract(a, b)), L"addSubtract mismatch", LINE_INFO());
        }

        TEST_METHOD(computesModulo)
        {
            const v_ui64_2 a{UInt_64{7}, UInt_64{10000000001}};
            const v_ui64_2 b{UInt_64{3}, UInt_64{10000000000}};
            const v_ui64_2 expected{UInt_64{1}, UInt_64{1}};
            Assert::IsTrue(allTrue(expected == modulo(a, b)), L"modulo mismatch", LINE_INFO());
        }

        TEST_METHOD(findsArgMinAndArgMax)
        {
            // above INT64_MAX: compared unsigned
            const v_ui64_2 given{UInt_64{5}, UInt_64(-1)};
            Assert::AreEqual(ASizeT{0}, argmin(given), L"argmin mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{1}, argmax(given), L"argmax mismatch", LINE_INFO());
            const v_ui64_2 swapped{UInt_64(-1), UInt_64{5}};
            Assert::AreEqual(ASizeT{1}, argmin(swapped), L"argmin Y mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{0}, argmax(swapped), L"argmax X mismatch", LINE_INFO());
        }

        TEST_METHOD(picksHighestIndexOfEqualExtrema)
        {
            const v_ui64_2 given{UInt_64{4}};
            Assert::AreEqual(ASizeT{1}, argmin(given), L"argmin tie", LINE_INFO());
            Assert::AreEqual(ASizeT{1}, argmax(given), L"argmax tie", LINE_INFO());
        }

        TEST_METHOD(testsTruthBitwise)
        {
            // Components with only some bits set are neither fully true nor false.
            const v_ui64_2 partial{UInt_64{1}};
            Assert::IsTrue(anyTrue(partial), L"anyTrue of partial bits", LINE_INFO());
            Assert::IsTrue(anyFalse(partial), L"anyFalse of partial bits", LINE_INFO());
            Assert::IsFalse(allTrue(partial), L"allTrue of partial bits", LINE_INFO());
            Assert::IsFalse(allFalse(partial), L"allFalse of partial bits", LINE_INFO());
        }
    };
}
