#include "vectorization_tests.h"

namespace vectorization::test
{
    TEST_CLASS (v_i32_8_FunctionsTest)
    {
        public
        :
        TEST_METHOD(computesAndnot)
        {
            const v_i32_8 allOnes{Int_32{-1}};
            const v_i32_8 mask{Int_32{-1}, 0, Int_32{-1}, 0, Int_32{-1}, 0, Int_32{-1}, 0};

            const auto actual = andnot(allOnes, mask);
            Assert::IsTrue(allTrue(Zero<v_i32_8>() == actual), L"andnot(~0, x) should be 0", LINE_INFO());

            const auto actual2 = andnot(Zero<v_i32_8>(), allOnes);
            Assert::IsTrue(allTrue(allOnes == actual2), L"andnot(0, ~0) should be ~0", LINE_INFO());
        }

        TEST_METHOD(detectsNegativeValues)
        {
            const v_i32_8 given{-1, 0, 1, -2, -3, 0, 0, -4};
            const auto mask = isNegative(given);
            Assert::IsTrue((mask & 1) != 0, L"X1 should be negative", LINE_INFO());
            Assert::IsTrue((mask & 2) == 0, L"X2 should not be negative", LINE_INFO());
            Assert::IsTrue((mask & 4) == 0, L"X3 should not be negative", LINE_INFO());
            Assert::IsTrue((mask & 8) != 0, L"X4 should be negative", LINE_INFO());
            Assert::IsTrue((mask & 16) != 0, L"X5 should be negative", LINE_INFO());
            Assert::IsTrue((mask & 32) == 0, L"X6 should not be negative", LINE_INFO());
            Assert::IsTrue((mask & 64) == 0, L"X7 should not be negative", LINE_INFO());
            Assert::IsTrue((mask & 128) != 0, L"X8 should be negative", LINE_INFO());
        }

        TEST_METHOD(computesAbsoluteValue)
        {
            const v_i32_8 given{-1, 2, -3, 4, -5, 6, -7, 8};
            const v_i32_8 expected{1, 2, 3, 4, 5, 6, 7, 8};
            const auto actual = abs(given);
            Assert::IsTrue(allTrue(expected == actual), L"abs value mismatch", LINE_INFO());
        }

        TEST_METHOD(computesSqr)
        {
            const v_i32_8 given{2, -3, 4, 0, 1, -1, 5, -2};
            const v_i32_8 expected{4, 9, 16, 0, 1, 1, 25, 4};
            const auto actual = sqr(given);
            Assert::IsTrue(allTrue(expected == actual), L"sqr value mismatch", LINE_INFO());
        }

        TEST_METHOD(computesComponentWiseMin)
        {
            const v_i32_8 a{1, 5, 3, 8, 2, 9, 4, 7};
            const v_i32_8 b{4, 2, 6, 1, 8, 3, 7, 5};
            const v_i32_8 expected{1, 2, 3, 1, 2, 3, 4, 5};
            const auto actual = min(a, b);
            Assert::IsTrue(allTrue(expected == actual), L"min value mismatch", LINE_INFO());
        }

        TEST_METHOD(computesComponentWiseMax)
        {
            const v_i32_8 a{1, 5, 3, 8, 2, 9, 4, 7};
            const v_i32_8 b{4, 2, 6, 1, 8, 3, 7, 5};
            const v_i32_8 expected{4, 5, 6, 8, 8, 9, 7, 7};
            const auto actual = max(a, b);
            Assert::IsTrue(allTrue(expected == actual), L"max value mismatch", LINE_INFO());
        }

        TEST_METHOD(clampsValues)
        {
            const v_i32_8 given{-5, 3, 15, 7, -1, 20, 0, 10};
            const v_i32_8 lo{Int_32{0}};
            const v_i32_8 hi{Int_32{10}};
            const v_i32_8 expected{0, 3, 10, 7, 0, 10, 0, 10};
            const auto actual = clamp(given, lo, hi);
            Assert::IsTrue(allTrue(expected == actual), L"clamp value mismatch", LINE_INFO());
        }

        TEST_METHOD(checksAnyTrue)
        {
            Assert::IsTrue(anyTrue(v_i32_8{1, 0, 0, 0, 0, 0, 0, 0}), L"anyTrue first lane", LINE_INFO());
            Assert::IsTrue(anyTrue(v_i32_8{0, 0, 0, 0, 0, 0, 0, 1}), L"anyTrue last lane", LINE_INFO());
            Assert::IsFalse(anyTrue(Zero<v_i32_8>()), L"anyTrue all zero", LINE_INFO());
        }

        TEST_METHOD(checksAllTrue)
        {
            Assert::IsTrue(allTrue(v_i32_8{Int_32{-1}}), L"allTrue should be true", LINE_INFO());
            Assert::IsFalse(allTrue(v_i32_8{-1, -1, -1, -1, -1, 0, -1, -1}), L"allTrue with one zero", LINE_INFO());
        }

        TEST_METHOD(checksAnyFalse)
        {
            Assert::IsTrue(anyFalse(v_i32_8{-1, -1, -1, 0, -1, -1, -1, -1}), L"anyFalse with one zero", LINE_INFO());
            Assert::IsFalse(anyFalse(v_i32_8{Int_32{-1}}), L"anyFalse all ones", LINE_INFO());
        }

        TEST_METHOD(checksAllFalse)
        {
            Assert::IsTrue(allFalse(Zero<v_i32_8>()), L"allFalse all zero", LINE_INFO());
            Assert::IsFalse(allFalse(v_i32_8{0, 1, 0, 0, 0, 0, 0, 0}), L"allFalse with one nonzero", LINE_INFO());
        }

        TEST_METHOD(testsTruthBitwise)
        {
            // Components with only some bits set are neither fully true nor false.
            const v_i32_8 partial{1};
            Assert::IsTrue(anyTrue(partial), L"anyTrue of partial bits", LINE_INFO());
            Assert::IsTrue(anyFalse(partial), L"anyFalse of partial bits", LINE_INFO());
            Assert::IsFalse(allTrue(partial), L"allTrue of partial bits", LINE_INFO());
            Assert::IsFalse(allFalse(partial), L"allFalse of partial bits", LINE_INFO());
        }

        TEST_METHOD(computesVectorMinAndMax)
        {
            // extrema in the upper half, so that both halves take part
            const v_i32_8 given{3, 1, 4, 2, 8, -6, 5, 7};
            Assert::IsTrue(allTrue(v_i32_8{-6} == minv(given)), L"minv mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_i32_8{8} == maxv(given)), L"maxv mismatch", LINE_INFO());
            Assert::AreEqual(Int_32{-6}, min(given), L"min mismatch", LINE_INFO());
            Assert::AreEqual(Int_32{8}, max(given), L"max mismatch", LINE_INFO());
            const v_i32_8 lower{-3, 1, 4, 2, 8, 6, 5, 9};
            Assert::AreEqual(Int_32{-3}, min(lower), L"min in lower half", LINE_INFO());
            Assert::AreEqual(Int_32{9}, max(lower), L"max in upper half", LINE_INFO());
        }

        TEST_METHOD(computesHorizontalSum)
        {
            const v_i32_8 given{1, 2, 3, 4, 5, 6, 7, -100};
            Assert::AreEqual(Int_32{-72}, horizontalSum(given), L"horizontalSum mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_i32_8{-72} == horizontalSumv(given)), L"horizontalSumv mismatch", LINE_INFO());
        }

        TEST_METHOD(computesDotProduct)
        {
            const v_i32_8 a{1, 2, 3, 4, 5, 6, 7, 8};
            const v_i32_8 b{1, -1, 1, -1, 1, -1, 1, -10};
            // 1 - 2 + 3 - 4 + 5 - 6 + 7 - 80 = -76
            Assert::AreEqual(Int_32{-76}, dot(a, b), L"dot mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_i32_8{-76} == dotv(a, b)), L"dotv mismatch", LINE_INFO());
        }

        TEST_METHOD(computesManhattanLengthAndDistance)
        {
            const v_i32_8 given{1, -2, 3, -4, 5, -6, 7, -8};
            Assert::AreEqual(Int_32{36}, length(given), L"length mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_i32_8{36} == lengthv(given)), L"lengthv mismatch", LINE_INFO());
            const v_i32_8 a{1, 2, 3, 4, 5, 6, 7, 8};
            const v_i32_8 b{2, 0, 3, 8, 5, 6, 7, -2};
            // 1 + 2 + 0 + 4 + 0 + 0 + 0 + 10 = 17
            Assert::AreEqual(Int_32{17}, distance(a, b), L"distance mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_i32_8{17} == distancev(a, b)), L"distancev mismatch", LINE_INFO());
        }

        TEST_METHOD(computesMultiplyAddFamily)
        {
            const v_i32_8 a{1, 2, 3, 4, 5, 6, 7, 8};
            const v_i32_8 b{2, 2, 2, 2, -2, -2, -2, -2};
            const v_i32_8 c{1, 1, 1, 1, 1, 1, 1, 1};
            // a * b = {2, 4, 6, 8, -10, -12, -14, -16}
            Assert::IsTrue(
                allTrue(v_i32_8{3, 5, 7, 9, -9, -11, -13, -15} == multiplyAdd(a, b, c)), L"multiplyAdd", LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_i32_8{1, 3, 5, 7, -11, -13, -15, -17} == multiplySub(a, b, c)), L"multiplySub", LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_i32_8{1, 5, 5, 9, -11, -11, -15, -15} == multiplyAddSub(a, b, c)), L"multiplyAddSub",
                LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_i32_8{3, 3, 7, 7, -9, -13, -13, -17} == multiplySubAdd(a, b, c)), L"multiplySubAdd",
                LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_i32_8{-1, -3, -5, -7, 11, 13, 15, 17} == negativeMultiplyAdd(a, b, c)),
                L"negativeMultiplyAdd", LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_i32_8{-3, -5, -7, -9, 9, 11, 13, 15} == negativeMultiplySub(a, b, c)),
                L"negativeMultiplySub", LINE_INFO()
            );
        }

        TEST_METHOD(computesAddSubtract)
        {
            const v_i32_8 a{10, 20, 30, 40, 50, 60, 70, 80};
            const v_i32_8 b{1, 2, 3, 4, 5, 6, 7, 8};
            const v_i32_8 expected{9, 22, 27, 44, 45, 66, 63, 88};
            Assert::IsTrue(allTrue(expected == addSubtract(a, b)), L"addSubtract mismatch", LINE_INFO());
        }

        TEST_METHOD(computesTruncatingModulo)
        {
            const v_i32_8 a{7, -7, 7, -7, 9, 10, 11, 12};
            const v_i32_8 b{3, 3, -3, -3, 5, 5, 5, 5};
            const v_i32_8 expected{1, -1, 1, -1, 4, 0, 1, 2};
            Assert::IsTrue(allTrue(expected == modulo(a, b)), L"modulo mismatch", LINE_INFO());
        }

        TEST_METHOD(findsArgMinAndArgMax)
        {
            Assert::AreEqual(ASizeT{5}, argmin(v_i32_8{3, 1, 4, 2, 8, -6, 5, 7}), L"argmin mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{4}, argmax(v_i32_8{3, 1, 4, 2, 8, -6, 5, 7}), L"argmax mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{0}, argmin(v_i32_8{-9, 1, 4, 2, 8, 6, 5, 7}), L"argmin X1 mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{7}, argmax(v_i32_8{3, 1, 4, 2, 8, 6, 5, 9}), L"argmax X8 mismatch", LINE_INFO());
        }

        TEST_METHOD(picksHighestIndexOfEqualExtrema)
        {
            Assert::AreEqual(ASizeT{6}, argmin(v_i32_8{1, 5, 1, 7, 2, 3, 1, 4}), L"argmin tie", LINE_INFO());
            Assert::AreEqual(ASizeT{3}, argmax(v_i32_8{7, 5, 1, 7, 2, 3, 1, 4}), L"argmax tie", LINE_INFO());
        }
    };
}
