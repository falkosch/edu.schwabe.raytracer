#include "vectorization_tests.h"

namespace vectorization::test
{
    TEST_CLASS (v_ui32_8_FunctionsTest)
    {
        public
        :
        TEST_METHOD(computesAndnot)
        {
            const v_ui32_8 allOnes{UInt_32{0xFFFFFFFFu}};

            const auto actual = andnot(allOnes, allOnes);
            Assert::IsTrue(allTrue(Zero<v_ui32_8>() == actual), L"andnot(~0, ~0) should be 0", LINE_INFO());

            const auto actual2 = andnot(Zero<v_ui32_8>(), allOnes);
            Assert::IsTrue(allTrue(allOnes == actual2), L"andnot(0, ~0) should be ~0", LINE_INFO());
        }

        TEST_METHOD(computesSqr)
        {
            const v_ui32_8 given{2u, 3u, 4u, 0u, 1u, 5u, 10u, 7u};
            const v_ui32_8 expected{4u, 9u, 16u, 0u, 1u, 25u, 100u, 49u};
            const auto actual = sqr(given);
            Assert::IsTrue(allTrue(expected == actual), L"sqr value mismatch", LINE_INFO());
        }

        TEST_METHOD(computesComponentWiseMin)
        {
            const v_ui32_8 a{1u, 5u, 3u, 8u, 2u, 9u, 4u, 7u};
            const v_ui32_8 b{4u, 2u, 6u, 1u, 8u, 3u, 7u, 5u};
            const v_ui32_8 expected{1u, 2u, 3u, 1u, 2u, 3u, 4u, 5u};
            const auto actual = min(a, b);
            Assert::IsTrue(allTrue(expected == actual), L"min value mismatch", LINE_INFO());
        }

        TEST_METHOD(computesComponentWiseMax)
        {
            const v_ui32_8 a{1u, 5u, 3u, 8u, 2u, 9u, 4u, 7u};
            const v_ui32_8 b{4u, 2u, 6u, 1u, 8u, 3u, 7u, 5u};
            const v_ui32_8 expected{4u, 5u, 6u, 8u, 8u, 9u, 7u, 7u};
            const auto actual = max(a, b);
            Assert::IsTrue(allTrue(expected == actual), L"max value mismatch", LINE_INFO());
        }

        TEST_METHOD(clampsValues)
        {
            const v_ui32_8 given{0u, 3u, 15u, 7u, 0u, 20u, 5u, 10u};
            const v_ui32_8 lo{UInt_32{1}};
            const v_ui32_8 hi{UInt_32{10}};
            const v_ui32_8 expected{1u, 3u, 10u, 7u, 1u, 10u, 5u, 10u};
            const auto actual = clamp(given, lo, hi);
            Assert::IsTrue(allTrue(expected == actual), L"clamp value mismatch", LINE_INFO());
        }

        TEST_METHOD(checksAnyTrue)
        {
            Assert::IsTrue(anyTrue(v_ui32_8{1u, 0u, 0u, 0u, 0u, 0u, 0u, 0u}), L"anyTrue first lane", LINE_INFO());
            Assert::IsTrue(anyTrue(v_ui32_8{0u, 0u, 0u, 0u, 0u, 0u, 0u, 1u}), L"anyTrue last lane", LINE_INFO());
            Assert::IsFalse(anyTrue(Zero<v_ui32_8>()), L"anyTrue all zero", LINE_INFO());
        }

        TEST_METHOD(checksAllTrue)
        {
            Assert::IsTrue(allTrue(v_ui32_8{UInt_32{0xFFFFFFFFu}}), L"allTrue should be true", LINE_INFO());
            Assert::IsFalse(
                allTrue(v_ui32_8{
                    0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0u, 0xFFFFFFFFu, 0xFFFFFFFFu
                }),
                L"allTrue with one zero", LINE_INFO()
            );
        }

        TEST_METHOD(checksAnyFalse)
        {
            Assert::IsTrue(
                anyFalse(v_ui32_8{
                    0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0u, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu
                }),
                L"anyFalse with one zero", LINE_INFO()
            );
            Assert::IsFalse(anyFalse(v_ui32_8{UInt_32{0xFFFFFFFFu}}), L"anyFalse all ones", LINE_INFO());
        }

        TEST_METHOD(checksAllFalse)
        {
            Assert::IsTrue(allFalse(Zero<v_ui32_8>()), L"allFalse all zero", LINE_INFO());
            Assert::IsFalse(allFalse(v_ui32_8{0u, 1u, 0u, 0u, 0u, 0u, 0u, 0u}), L"allFalse with one nonzero",
                            LINE_INFO());
        }

        TEST_METHOD(testsTruthBitwise)
        {
            // Components with only some bits set are neither fully true nor false.
            const v_ui32_8 partial{1u};
            Assert::IsTrue(anyTrue(partial), L"anyTrue of partial bits", LINE_INFO());
            Assert::IsTrue(anyFalse(partial), L"anyFalse of partial bits", LINE_INFO());
            Assert::IsFalse(allTrue(partial), L"allTrue of partial bits", LINE_INFO());
            Assert::IsFalse(allFalse(partial), L"allFalse of partial bits", LINE_INFO());
        }

        TEST_METHOD(computesVectorMinAndMax)
        {
            // extrema in the upper half, so that both halves take part; above INT32_MAX compared unsigned
            const v_ui32_8 given{3u, 1u, 4u, 2u, 0xFFFFFFFFu, 0u, 5u, 7u};
            Assert::IsTrue(allTrue(v_ui32_8{0u} == minv(given)), L"minv mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_ui32_8{0xFFFFFFFFu} == maxv(given)), L"maxv mismatch", LINE_INFO());
            Assert::AreEqual(UInt_32{0}, min(given), L"min mismatch", LINE_INFO());
            Assert::AreEqual(UInt_32{0xFFFFFFFFu}, max(given), L"max mismatch", LINE_INFO());
            const v_ui32_8 lower{1u, 3u, 4u, 2u, 8u, 6u, 5u, 9u};
            Assert::AreEqual(UInt_32{1}, min(lower), L"min in lower half", LINE_INFO());
            Assert::AreEqual(UInt_32{9}, max(lower), L"max in upper half", LINE_INFO());
        }

        TEST_METHOD(computesHorizontalSum)
        {
            const v_ui32_8 given{1u, 2u, 3u, 4u, 5u, 6u, 7u, 100u};
            Assert::AreEqual(UInt_32{128}, horizontalSum(given), L"horizontalSum mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_ui32_8{128u} == horizontalSumv(given)), L"horizontalSumv mismatch", LINE_INFO());
        }

        TEST_METHOD(computesDotProduct)
        {
            const v_ui32_8 a{1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u};
            const v_ui32_8 b{1u, 0u, 1u, 0u, 1u, 0u, 1u, 10u};
            // 1 + 3 + 5 + 7 + 80 = 96
            Assert::AreEqual(UInt_32{96}, dot(a, b), L"dot mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_ui32_8{96u} == dotv(a, b)), L"dotv mismatch", LINE_INFO());
        }

        TEST_METHOD(computesManhattanLengthAndDistance)
        {
            const v_ui32_8 given{1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u};
            Assert::AreEqual(UInt_32{36}, length(given), L"length mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_ui32_8{36u} == lengthv(given)), L"lengthv mismatch", LINE_INFO());
            // components of b above those of a must not wrap around
            const v_ui32_8 a{1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u};
            const v_ui32_8 b{2u, 0u, 3u, 8u, 5u, 6u, 7u, 18u};
            // 1 + 2 + 0 + 4 + 0 + 0 + 0 + 10 = 17
            Assert::AreEqual(UInt_32{17}, distance(a, b), L"distance mismatch", LINE_INFO());
            Assert::AreEqual(UInt_32{17}, distance(b, a), L"distance is symmetric", LINE_INFO());
            Assert::IsTrue(allTrue(v_ui32_8{17u} == distancev(a, b)), L"distancev mismatch", LINE_INFO());
        }

        TEST_METHOD(computesMultiplyAddFamily)
        {
            const v_ui32_8 a{1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u};
            const v_ui32_8 b{2u};
            const v_ui32_8 c{1u};
            // a * b = {2, 4, 6, 8, 10, 12, 14, 16}
            Assert::IsTrue(
                allTrue(v_ui32_8{3u, 5u, 7u, 9u, 11u, 13u, 15u, 17u} == multiplyAdd(a, b, c)), L"multiplyAdd",
                LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_ui32_8{1u, 3u, 5u, 7u, 9u, 11u, 13u, 15u} == multiplySub(a, b, c)), L"multiplySub",
                LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_ui32_8{1u, 5u, 5u, 9u, 9u, 13u, 13u, 17u} == multiplyAddSub(a, b, c)), L"multiplyAddSub",
                LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_ui32_8{3u, 3u, 7u, 7u, 11u, 11u, 15u, 15u} == multiplySubAdd(a, b, c)), L"multiplySubAdd",
                LINE_INFO()
            );
            const v_ui32_8 large{100u};
            Assert::IsTrue(
                allTrue(v_ui32_8{98u, 96u, 94u, 92u, 90u, 88u, 86u, 84u} == negativeMultiplyAdd(a, b, large)),
                L"negativeMultiplyAdd", LINE_INFO()
            );
            // -(a * b) - c wraps around
            Assert::IsTrue(
                allTrue(
                    v_ui32_8{
                        UInt_32(-3), UInt_32(-5), UInt_32(-7), UInt_32(-9), UInt_32(-11), UInt_32(-13), UInt_32(-15),
                        UInt_32(-17)
                    } == negativeMultiplySub(a, b, c)
                ),
                L"negativeMultiplySub", LINE_INFO()
            );
        }

        TEST_METHOD(computesAddSubtract)
        {
            const v_ui32_8 a{10u, 20u, 30u, 40u, 50u, 60u, 70u, 80u};
            const v_ui32_8 b{1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u};
            const v_ui32_8 expected{9u, 22u, 27u, 44u, 45u, 66u, 63u, 88u};
            Assert::IsTrue(allTrue(expected == addSubtract(a, b)), L"addSubtract mismatch", LINE_INFO());
        }

        TEST_METHOD(computesModulo)
        {
            const v_ui32_8 a{7u, 8u, 9u, 10u, 11u, 12u, 13u, 0xFFFFFFFFu};
            const v_ui32_8 b{3u, 3u, 3u, 4u, 4u, 5u, 5u, 0x80000000u};
            const v_ui32_8 expected{1u, 2u, 0u, 2u, 3u, 2u, 3u, 0x7FFFFFFFu};
            Assert::IsTrue(allTrue(expected == modulo(a, b)), L"modulo mismatch", LINE_INFO());
        }

        TEST_METHOD(findsArgMinAndArgMax)
        {
            const v_ui32_8 given{3u, 1u, 4u, 2u, 0xFFFFFFFFu, 0u, 5u, 7u};
            Assert::AreEqual(ASizeT{5}, argmin(given), L"argmin mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{4}, argmax(given), L"argmax mismatch", LINE_INFO());
            const v_ui32_8 ends{0u, 1u, 4u, 2u, 8u, 6u, 5u, 9u};
            Assert::AreEqual(ASizeT{0}, argmin(ends), L"argmin X1 mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{7}, argmax(ends), L"argmax X8 mismatch", LINE_INFO());
        }

        TEST_METHOD(picksHighestIndexOfEqualExtrema)
        {
            Assert::AreEqual(ASizeT{6}, argmin(v_ui32_8{1u, 5u, 1u, 7u, 2u, 3u, 1u, 4u}), L"argmin tie", LINE_INFO());
            Assert::AreEqual(ASizeT{3}, argmax(v_ui32_8{7u, 5u, 1u, 7u, 2u, 3u, 1u, 4u}), L"argmax tie", LINE_INFO());
        }
    };
}
