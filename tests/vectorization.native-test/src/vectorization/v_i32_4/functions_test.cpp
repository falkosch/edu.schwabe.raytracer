#include "vectorization_tests.h"

namespace vectorization::test
{
    TEST_CLASS (v_i32_4_FunctionsTest)
    {
        public
        :
        TEST_METHOD(zerosWComponent)
        {
            const v_i32_4 given{1, 2, 3, 4};
            const v_i32_4 expected{1, 2, 3, 0};
            const auto actual = zeroW(given);
            Assert::IsTrue(allTrue(expected == actual), L"zeroW value mismatch", LINE_INFO());
        }

        TEST_METHOD(computesAndnot)
        {
            const auto a = MaskAll<v_i32_4>();
            const auto b = MaskX<v_i32_4>();
            const auto actual = andnot(a, b);
            Assert::IsTrue(allTrue(Zero<v_i32_4>() == actual), L"andnot value mismatch", LINE_INFO());

            const auto actual2 = andnot(Zero<v_i32_4>(), MaskAll<v_i32_4>());
            Assert::IsTrue(allTrue(MaskAll<v_i32_4>() == actual2), L"andnot value mismatch", LINE_INFO());
        }

        TEST_METHOD(detectsNegativeValues)
        {
            const v_i32_4 given{-1, 0, 1, -2};
            const auto mask = isNegative(given);
            Assert::IsTrue((mask & 1) != 0, L"X should be negative", LINE_INFO());
            Assert::IsTrue((mask & 2) == 0, L"Y should not be negative", LINE_INFO());
            Assert::IsTrue((mask & 4) == 0, L"Z should not be negative", LINE_INFO());
            Assert::IsTrue((mask & 8) != 0, L"W should be negative", LINE_INFO());
            Assert::AreEqual(Int_32{0b1001}, mask, L"one bit per lane", LINE_INFO());
        }

        TEST_METHOD(setsOneW)
        {
            const v_i32_4 given{1, 2, 3, 4};
            const v_i32_4 expected{1, 2, 3, 1};
            Assert::IsTrue(allTrue(expected == oneW(given)), L"oneW value mismatch", LINE_INFO());
        }

        TEST_METHOD(computesVectorMinAndMax)
        {
            const v_i32_4 given{3, -1, 4, 2};
            Assert::IsTrue(allTrue(v_i32_4{-1} == minv(given)), L"minv mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_i32_4{4} == maxv(given)), L"maxv mismatch", LINE_INFO());
        }

        TEST_METHOD(computesVectorMin3AndMax3)
        {
            const v_i32_4 given{3, 1, 4, -9};
            Assert::IsTrue(allTrue(v_i32_4{1} == min3v(given)), L"min3v mismatch", LINE_INFO());
            const v_i32_4 high{3, 1, 4, 99};
            Assert::IsTrue(allTrue(v_i32_4{4} == max3v(high)), L"max3v mismatch", LINE_INFO());
        }

        TEST_METHOD(computesHorizontalSum)
        {
            const v_i32_4 given{1, -2, 3, 40};
            Assert::AreEqual(Int_32{42}, horizontalSum(given), L"horizontalSum mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_i32_4{42} == horizontalSumv(given)), L"horizontalSumv mismatch", LINE_INFO());
            Assert::AreEqual(Int_32{2}, horizontalSum3(given), L"horizontalSum3 mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_i32_4{2} == horizontalSum3v(given)), L"horizontalSum3v mismatch", LINE_INFO());
        }

        TEST_METHOD(computesDotProduct)
        {
            const v_i32_4 a{1, 2, 3, 4};
            const v_i32_4 b{5, -6, 7, 8};
            // 5 - 12 + 21 + 32 = 46, without w: 14
            Assert::AreEqual(Int_32{46}, dot(a, b), L"dot mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_i32_4{46} == dotv(a, b)), L"dotv mismatch", LINE_INFO());
            Assert::AreEqual(Int_32{14}, dot3(a, b), L"dot3 mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_i32_4{14} == dot3v(a, b)), L"dot3v mismatch", LINE_INFO());
        }

        TEST_METHOD(computesVectorDistance)
        {
            const v_i32_4 a{1, 2, 3, 4};
            const v_i32_4 b{4, -6, 9, 14};
            // |3| + |-8| + |6| + |10| = 27, without w: 17
            Assert::IsTrue(allTrue(v_i32_4{27} == distancev(a, b)), L"distancev mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_i32_4{17} == distance3v(a, b)), L"distance3v mismatch", LINE_INFO());
        }

        TEST_METHOD(computesMultiplyAddFamily)
        {
            const v_i32_4 a{1, 2, 3, 4};
            const v_i32_4 b{5, 6, -7, 8};
            const v_i32_4 c{10, 20, 30, 40};
            // a * b = {5, 12, -21, 32}
            Assert::IsTrue(allTrue(v_i32_4{15, 32, 9, 72} == multiplyAdd(a, b, c)), L"multiplyAdd", LINE_INFO());
            Assert::IsTrue(allTrue(v_i32_4{-5, -8, -51, -8} == multiplySub(a, b, c)), L"multiplySub", LINE_INFO());
            Assert::IsTrue(
                allTrue(v_i32_4{-5, 32, -51, 72} == multiplyAddSub(a, b, c)), L"multiplyAddSub", LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_i32_4{15, -8, 9, -8} == multiplySubAdd(a, b, c)), L"multiplySubAdd", LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_i32_4{5, 8, 51, 8} == negativeMultiplyAdd(a, b, c)), L"negativeMultiplyAdd", LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_i32_4{-15, -32, -9, -72} == negativeMultiplySub(a, b, c)), L"negativeMultiplySub",
                LINE_INFO()
            );
        }

        TEST_METHOD(computesAddSubtract)
        {
            const v_i32_4 a{10, 20, 30, 40};
            const v_i32_4 b{1, 2, 3, 4};
            const v_i32_4 expected{9, 22, 27, 44};
            Assert::IsTrue(allTrue(expected == addSubtract(a, b)), L"addSubtract mismatch", LINE_INFO());
        }

        TEST_METHOD(computesTruncatingModulo)
        {
            const v_i32_4 a{7, -7, 7, -7};
            const v_i32_4 b{3, 3, -3, -3};
            const v_i32_4 expected{1, -1, 1, -1};
            Assert::IsTrue(allTrue(expected == modulo(a, b)), L"modulo mismatch", LINE_INFO());
        }

        TEST_METHOD(findsArgMinAndArgMax)
        {
            Assert::AreEqual(ASizeT{1}, argmin(v_i32_4{3, -1, 4, 2}), L"argmin mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{3}, argmin(v_i32_4{3, 1, 4, -2}), L"argmin W mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{2}, argmax(v_i32_4{3, 1, 4, 2}), L"argmax mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{0}, argmax(v_i32_4{5, 1, 4, 2}), L"argmax X mismatch", LINE_INFO());
        }

        TEST_METHOD(picksHighestIndexOfEqualExtrema)
        {
            Assert::AreEqual(ASizeT{2}, argmin(v_i32_4{1, 5, 1, 7}), L"argmin tie", LINE_INFO());
            Assert::AreEqual(ASizeT{3}, argmax(v_i32_4{7, 5, 1, 7}), L"argmax tie", LINE_INFO());
            Assert::AreEqual(ASizeT{1}, argmin3(v_i32_4{1, 1, 5, 0}), L"argmin3 tie", LINE_INFO());
            Assert::AreEqual(ASizeT{2}, argmax3(v_i32_4{5, 1, 5, 9}), L"argmax3 tie", LINE_INFO());
        }

        TEST_METHOD(findsArgMin3AndArgMax3IgnoringW)
        {
            Assert::AreEqual(ASizeT{2}, argmin3(v_i32_4{3, 2, 1, -9}), L"argmin3 mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{0}, argmin3(v_i32_4{1, 2, 3, -9}), L"argmin3 X mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{1}, argmax3(v_i32_4{1, 3, 2, 99}), L"argmax3 mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{0}, argmax3(v_i32_4{4, 3, 2, 99}), L"argmax3 X mismatch", LINE_INFO());
        }

        TEST_METHOD(testsTruthBitwise)
        {
            // Components with only some bits set are neither fully true nor false.
            const v_i32_4 partial{1};
            Assert::IsTrue(anyTrue(partial), L"anyTrue of partial bits", LINE_INFO());
            Assert::IsTrue(anyFalse(partial), L"anyFalse of partial bits", LINE_INFO());
            Assert::IsFalse(allTrue(partial), L"allTrue of partial bits", LINE_INFO());
            Assert::IsFalse(allFalse(partial), L"allFalse of partial bits", LINE_INFO());
        }

        TEST_METHOD(computesAbsoluteValue)
        {
            const v_i32_4 given{-1, 2, -3, 4};
            const v_i32_4 expected{1, 2, 3, 4};
            const auto actual = abs(given);
            Assert::IsTrue(allTrue(expected == actual), L"abs value mismatch", LINE_INFO());
        }

        TEST_METHOD(computesSqr)
        {
            const v_i32_4 given{2, -3, 4, 0};
            const v_i32_4 expected{4, 9, 16, 0};
            const auto actual = sqr(given);
            Assert::IsTrue(allTrue(expected == actual), L"sqr value mismatch", LINE_INFO());
        }

        TEST_METHOD(computesComponentWiseMin)
        {
            const v_i32_4 a{1, 5, 3, 8};
            const v_i32_4 b{4, 2, 6, 1};
            const v_i32_4 expected{1, 2, 3, 1};
            const auto actual = min(a, b);
            Assert::IsTrue(allTrue(expected == actual), L"min value mismatch", LINE_INFO());
        }

        TEST_METHOD(computesScalarMin)
        {
            const v_i32_4 given{3, 1, 4, 2};
            Assert::AreEqual(Int_32{1}, min(given), L"scalar min mismatch", LINE_INFO());
        }

        TEST_METHOD(computesScalarMin3)
        {
            const v_i32_4 given{3, 1, 4, 0};
            Assert::AreEqual(Int_32{1}, min3(given), L"scalar min3 mismatch", LINE_INFO());
        }

        TEST_METHOD(computesComponentWiseMax)
        {
            const v_i32_4 a{1, 5, 3, 8};
            const v_i32_4 b{4, 2, 6, 1};
            const v_i32_4 expected{4, 5, 6, 8};
            const auto actual = max(a, b);
            Assert::IsTrue(allTrue(expected == actual), L"max value mismatch", LINE_INFO());
        }

        TEST_METHOD(computesScalarMax)
        {
            const v_i32_4 given{3, 1, 4, 2};
            Assert::AreEqual(Int_32{4}, max(given), L"scalar max mismatch", LINE_INFO());
        }

        TEST_METHOD(computesScalarMax3)
        {
            const v_i32_4 given{3, 1, 4, 99};
            Assert::AreEqual(Int_32{4}, max3(given), L"scalar max3 mismatch", LINE_INFO());
        }

        TEST_METHOD(clampsValues)
        {
            const v_i32_4 given{-5, 3, 15, 7};
            const v_i32_4 lo{0, 0, 0, 0};
            const v_i32_4 hi{10, 10, 10, 10};
            const v_i32_4 expected{0, 3, 10, 7};
            const auto actual = clamp(given, lo, hi);
            Assert::IsTrue(allTrue(expected == actual), L"clamp value mismatch", LINE_INFO());
        }

        TEST_METHOD(checksAnyTrue)
        {
            Assert::IsTrue(anyTrue(v_i32_4{1, 0, 0, 0}), L"anyTrue should be true", LINE_INFO());
            Assert::IsTrue(anyTrue(v_i32_4{0, 0, 0, 1}), L"anyTrue should be true", LINE_INFO());
            Assert::IsFalse(anyTrue(Zero<v_i32_4>()), L"anyTrue should be false", LINE_INFO());
        }

        TEST_METHOD(checksAllTrue)
        {
            Assert::IsTrue(allTrue(v_i32_4{Int_32{-1}}), L"allTrue should be true", LINE_INFO());
            Assert::IsFalse(allTrue(v_i32_4{-1, 0, -1, -1}), L"allTrue should be false", LINE_INFO());
        }

        TEST_METHOD(checksAnyFalse)
        {
            Assert::IsTrue(anyFalse(v_i32_4{-1, 0, -1, -1}), L"anyFalse should be true", LINE_INFO());
            Assert::IsFalse(anyFalse(MaskAll<v_i32_4>()), L"anyFalse should be false", LINE_INFO());
        }

        TEST_METHOD(checksAllFalse)
        {
            Assert::IsTrue(allFalse(Zero<v_i32_4>()), L"allFalse should be true", LINE_INFO());
            Assert::IsFalse(allFalse(v_i32_4{0, 1, 0, 0}), L"allFalse should be false", LINE_INFO());
        }

        TEST_METHOD(computesLengthv)
        {
            const v_i32_4 given{1, 2, 3, 4};
            const auto result = lengthv(given);
            // lengthv sums absolute values of all components: |1|+|2|+|3|+|4| = 10
            const v_i32_4 expected{10};
            Assert::IsTrue(allTrue(expected == result), L"lengthv mismatch", LINE_INFO());
        }

        TEST_METHOD(computesLength)
        {
            const v_i32_4 given{1, 2, 3, 4};
            Assert::AreEqual(Int_32{10}, length(given), L"length mismatch", LINE_INFO());
        }

        TEST_METHOD(computesLength3v)
        {
            const v_i32_4 given{1, 2, 3, 99};
            const auto result = length3v(given);
            // length3v sums absolute values of X,Y,Z only: |1|+|2|+|3| = 6
            const v_i32_4 expected{6};
            Assert::IsTrue(allTrue(expected == result), L"length3v mismatch", LINE_INFO());
        }

        TEST_METHOD(computesLength3)
        {
            const v_i32_4 given{1, 2, 3, 99};
            Assert::AreEqual(Int_32{6}, length3(given), L"length3 mismatch", LINE_INFO());
        }

        TEST_METHOD(computesDistance)
        {
            const v_i32_4 a{1, 2, 3, 4};
            const v_i32_4 b{4, 6, 9, 14};
            // distance = length(b - a) = |3| + |4| + |6| + |10| = 23
            Assert::AreEqual(Int_32{23}, distance(a, b), L"distance mismatch", LINE_INFO());
        }

        TEST_METHOD(computesDistance3)
        {
            const v_i32_4 a{1, 2, 3, 99};
            const v_i32_4 b{4, 6, 9, 99};
            // distance3 = length3(b - a) = |3| + |4| + |6| = 13
            Assert::AreEqual(Int_32{13}, distance3(a, b), L"distance3 mismatch", LINE_INFO());
        }

        TEST_METHOD(checksAnyTrue3)
        {
            Assert::IsTrue(anyTrue3(v_i32_4{0, 0, 1, 0}), L"should detect true in Z", LINE_INFO());
            Assert::IsFalse(anyTrue3(v_i32_4{0, 0, 0, 99}), L"W should be ignored", LINE_INFO());
            Assert::IsFalse(anyTrue3(Zero<v_i32_4>()), L"all zero should be false", LINE_INFO());
        }

        TEST_METHOD(checksAnyFalse3)
        {
            Assert::IsTrue(anyFalse3(v_i32_4{-1, 0, -1, -1}), L"should detect false in Y", LINE_INFO());
            Assert::IsFalse(anyFalse3(v_i32_4{-1, -1, -1, 0}), L"W should be ignored", LINE_INFO());
        }

        TEST_METHOD(checksAllTrue3)
        {
            Assert::IsTrue(allTrue3(v_i32_4{-1, -1, -1, 0}), L"W should be ignored", LINE_INFO());
            Assert::IsFalse(allTrue3(v_i32_4{-1, 0, -1, -1}), L"Y is zero", LINE_INFO());
        }

        TEST_METHOD(checksAllFalse3)
        {
            Assert::IsTrue(allFalse3(v_i32_4{0, 0, 0, 99}), L"W should be ignored", LINE_INFO());
            Assert::IsFalse(allFalse3(v_i32_4{0, 1, 0, 0}), L"Y is nonzero", LINE_INFO());
        }
    };
}
