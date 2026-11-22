#include "vectorization_tests.h"

namespace vectorization::test
{
    TEST_CLASS (v_i64_2_FunctionsTest)
    {
        public
        :
        TEST_METHOD(computesSqr)
        {
            const v_i64_2 given{Int_64{3LL}, Int_64{-4LL}};
            const v_i64_2 expected{Int_64{9LL}, Int_64{16LL}};
            const auto actual = sqr(given);
            Assert::IsTrue(allTrue(expected == actual), L"sqr value mismatch", LINE_INFO());
        }

        TEST_METHOD(computesComponentWiseMin)
        {
            const v_i64_2 a{Int_64{10LL}, Int_64{-5LL}};
            const v_i64_2 b{Int_64{-1LL}, Int_64{30LL}};
            const v_i64_2 expected{Int_64{-1LL}, Int_64{-5LL}};
            const auto actual = min(a, b);
            Assert::IsTrue(allTrue(expected == actual), L"min value mismatch", LINE_INFO());
        }

        TEST_METHOD(computesScalarMin)
        {
            const v_i64_2 given{Int_64{10LL}, Int_64{-5LL}};
            Assert::AreEqual(Int_64{-5LL}, min(given), L"scalar min mismatch", LINE_INFO());
        }

        TEST_METHOD(computesMinVector)
        {
            const v_i64_2 given{Int_64{10LL}, Int_64{-5LL}};
            const auto actual = minv(given);
            Assert::AreEqual(Int_64{-5LL}, x(actual), L"minv X mismatch", LINE_INFO());
            Assert::AreEqual(Int_64{-5LL}, y(actual), L"minv Y mismatch", LINE_INFO());
        }

        TEST_METHOD(computesComponentWiseMax)
        {
            const v_i64_2 a{Int_64{10LL}, Int_64{-5LL}};
            const v_i64_2 b{Int_64{-1LL}, Int_64{30LL}};
            const v_i64_2 expected{Int_64{10LL}, Int_64{30LL}};
            const auto actual = max(a, b);
            Assert::IsTrue(allTrue(expected == actual), L"max value mismatch", LINE_INFO());
        }

        TEST_METHOD(computesScalarMax)
        {
            const v_i64_2 given{Int_64{10LL}, Int_64{-5LL}};
            Assert::AreEqual(Int_64{10LL}, max(given), L"scalar max mismatch", LINE_INFO());
        }

        TEST_METHOD(computesMaxVector)
        {
            const v_i64_2 given{Int_64{10LL}, Int_64{-5LL}};
            const auto actual = maxv(given);
            Assert::AreEqual(Int_64{10LL}, x(actual), L"maxv X mismatch", LINE_INFO());
            Assert::AreEqual(Int_64{10LL}, y(actual), L"maxv Y mismatch", LINE_INFO());
        }

        TEST_METHOD(clampsValues)
        {
            const v_i64_2 given{Int_64{-10LL}, Int_64{30LL}};
            const v_i64_2 lo{Int_64{-5LL}, Int_64{-5LL}};
            const v_i64_2 hi{Int_64{10LL}, Int_64{10LL}};
            const v_i64_2 expected{Int_64{-5LL}, Int_64{10LL}};
            const auto actual = clamp(given, lo, hi);
            Assert::IsTrue(allTrue(expected == actual), L"clamp value mismatch", LINE_INFO());
        }

        TEST_METHOD(checksAnyTrue)
        {
            Assert::IsTrue(anyTrue(v_i64_2{Int_64{1LL}, Int_64{0LL}}), L"anyTrue should be true", LINE_INFO());
            Assert::IsTrue(anyTrue(v_i64_2{Int_64{0LL}, Int_64{1LL}}), L"anyTrue should be true", LINE_INFO());
            Assert::IsFalse(anyTrue(Zero<v_i64_2>()), L"anyTrue should be false", LINE_INFO());
        }

        TEST_METHOD(checksAllTrue)
        {
            Assert::IsTrue(allTrue(MaskAll<v_i64_2>()), L"allTrue should be true", LINE_INFO());
            Assert::IsFalse(
                allTrue(v_i64_2{MaskAll<v_i64_2::BoolType>(), Int_64{0LL}}), L"allTrue should be false", LINE_INFO()
            );
        }

        TEST_METHOD(checksAnyFalse)
        {
            Assert::IsTrue(
                anyFalse(v_i64_2{MaskAll<v_i64_2::BoolType>(), Int_64{0LL}}), L"anyFalse should be true", LINE_INFO()
            );
            Assert::IsFalse(anyFalse(MaskAll<v_i64_2>()), L"anyFalse should be false", LINE_INFO());
        }

        TEST_METHOD(checksAllFalse)
        {
            Assert::IsTrue(allFalse(Zero<v_i64_2>()), L"allFalse should be true", LINE_INFO());
            Assert::IsFalse(allFalse(v_i64_2{Int_64{0LL}, Int_64{1LL}}), L"allFalse should be false", LINE_INFO());
        }

        TEST_METHOD(computesLengthVector)
        {
            const v_i64_2 given{Int_64{10LL}, Int_64{-5LL}};
            const auto actual = lengthv(given);
            // Manhattan length: |10| + |-5|
            Assert::AreEqual(Int_64{15LL}, x(actual), L"lengthv X mismatch", LINE_INFO());
            Assert::AreEqual(Int_64{15LL}, y(actual), L"lengthv Y mismatch", LINE_INFO());
        }

        TEST_METHOD(computesScalarLength)
        {
            const v_i64_2 given{Int_64{10LL}, Int_64{-5LL}};
            Assert::AreEqual(Int_64{15LL}, length(given), L"length mismatch", LINE_INFO());
        }

        TEST_METHOD(computesDistance)
        {
            const v_i64_2 a{Int_64{10LL}, Int_64{-5LL}};
            const v_i64_2 b{Int_64{7LL}, Int_64{-10LL}};
            Assert::AreEqual(Int_64{8LL}, distance(a, b), L"distance mismatch", LINE_INFO());
        }

        TEST_METHOD(computesVectorDistance)
        {
            const v_i64_2 a{Int_64{1LL}, Int_64{-5LL}};
            const v_i64_2 b{Int_64{7LL}, Int_64{-10LL}};
            // |-6| + |5|
            Assert::IsTrue(allTrue(v_i64_2{Int_64{11LL}} == distancev(a, b)), L"distancev mismatch", LINE_INFO());
        }

        TEST_METHOD(broadcastsVectorMinAndMax)
        {
            const v_i64_2 given{Int_64{-5LL}, Int_64{10LL}};
            Assert::IsTrue(allTrue(v_i64_2{Int_64{-5LL}} == minv(given)), L"minv mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_i64_2{Int_64{10LL}} == maxv(given)), L"maxv mismatch", LINE_INFO());
        }

        TEST_METHOD(detectsNegativeValues)
        {
            Assert::AreEqual(Int_64{0b01}, isNegative(v_i64_2{Int_64{-1LL}, Int_64{0LL}}), L"X", LINE_INFO());
            Assert::AreEqual(Int_64{0b10}, isNegative(v_i64_2{Int_64{1LL}, Int_64{-3LL}}), L"Y", LINE_INFO());
            Assert::AreEqual(Int_64{0b00}, isNegative(v_i64_2{Int_64{0LL}, Int_64{5LL}}), L"none", LINE_INFO());
        }

        TEST_METHOD(computesAbsoluteValue)
        {
            const v_i64_2 given{Int_64{-3000000000LL}, Int_64{4LL}};
            const v_i64_2 expected{Int_64{3000000000LL}, Int_64{4LL}};
            Assert::IsTrue(allTrue(expected == abs(given)), L"abs value mismatch", LINE_INFO());
        }

        TEST_METHOD(computesHorizontalSum)
        {
            const v_i64_2 given{Int_64{-3000000000LL}, Int_64{5000000000LL}};
            Assert::AreEqual(Int_64{2000000000LL}, horizontalSum(given), L"horizontalSum mismatch", LINE_INFO());
            Assert::IsTrue(
                allTrue(v_i64_2{Int_64{2000000000LL}} == horizontalSumv(given)), L"horizontalSumv mismatch", LINE_INFO()
            );
        }

        TEST_METHOD(computesDotProduct)
        {
            const v_i64_2 a{Int_64{3LL}, Int_64{-4LL}};
            const v_i64_2 b{Int_64{5LL}, Int_64{6LL}};
            Assert::AreEqual(Int_64{-9LL}, dot(a, b), L"dot mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_i64_2{Int_64{-9LL}} == dotv(a, b)), L"dotv mismatch", LINE_INFO());
        }

        TEST_METHOD(computesMultiplyAddFamily)
        {
            const v_i64_2 a{Int_64{3LL}, Int_64{-4LL}};
            const v_i64_2 b{Int_64{5LL}, Int_64{6LL}};
            const v_i64_2 c{Int_64{10LL}, Int_64{20LL}};
            // a * b = {15, -24}
            Assert::IsTrue(
                allTrue(v_i64_2{Int_64{25LL}, Int_64{-4LL}} == multiplyAdd(a, b, c)), L"multiplyAdd", LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_i64_2{Int_64{5LL}, Int_64{-44LL}} == multiplySub(a, b, c)), L"multiplySub", LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_i64_2{Int_64{5LL}, Int_64{-4LL}} == multiplyAddSub(a, b, c)), L"multiplyAddSub", LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_i64_2{Int_64{25LL}, Int_64{-44LL}} == multiplySubAdd(a, b, c)), L"multiplySubAdd", LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_i64_2{Int_64{-5LL}, Int_64{44LL}} == negativeMultiplyAdd(a, b, c)), L"negativeMultiplyAdd",
                LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_i64_2{Int_64{-25LL}, Int_64{4LL}} == negativeMultiplySub(a, b, c)), L"negativeMultiplySub",
                LINE_INFO()
            );
        }

        TEST_METHOD(computesAddSubtract)
        {
            const v_i64_2 a{Int_64{10LL}, Int_64{20LL}};
            const v_i64_2 b{Int_64{1LL}, Int_64{2LL}};
            const v_i64_2 expected{Int_64{9LL}, Int_64{22LL}};
            Assert::IsTrue(allTrue(expected == addSubtract(a, b)), L"addSubtract mismatch", LINE_INFO());
        }

        TEST_METHOD(computesTruncatingModulo)
        {
            const v_i64_2 a{Int_64{-7LL}, Int_64{7LL}};
            const v_i64_2 b{Int_64{3LL}, Int_64{-3LL}};
            const v_i64_2 expected{Int_64{-1LL}, Int_64{1LL}};
            Assert::IsTrue(allTrue(expected == modulo(a, b)), L"modulo mismatch", LINE_INFO());
        }

        TEST_METHOD(findsArgMinAndArgMax)
        {
            const v_i64_2 given{Int_64{-5LL}, Int_64{10LL}};
            Assert::AreEqual(ASizeT{0}, argmin(given), L"argmin mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{1}, argmax(given), L"argmax mismatch", LINE_INFO());
            const v_i64_2 swapped{Int_64{10LL}, Int_64{-5LL}};
            Assert::AreEqual(ASizeT{1}, argmin(swapped), L"argmin Y mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{0}, argmax(swapped), L"argmax X mismatch", LINE_INFO());
        }

        TEST_METHOD(picksHighestIndexOfEqualExtrema)
        {
            const v_i64_2 given{Int_64{4LL}};
            Assert::AreEqual(ASizeT{1}, argmin(given), L"argmin tie", LINE_INFO());
            Assert::AreEqual(ASizeT{1}, argmax(given), L"argmax tie", LINE_INFO());
        }

        TEST_METHOD(testsTruthBitwise)
        {
            // Components with only some bits set are neither fully true nor false.
            const v_i64_2 partial{Int_64{1LL}};
            Assert::IsTrue(anyTrue(partial), L"anyTrue of partial bits", LINE_INFO());
            Assert::IsTrue(anyFalse(partial), L"anyFalse of partial bits", LINE_INFO());
            Assert::IsFalse(allTrue(partial), L"allTrue of partial bits", LINE_INFO());
            Assert::IsFalse(allFalse(partial), L"allFalse of partial bits", LINE_INFO());
        }
    };
}
