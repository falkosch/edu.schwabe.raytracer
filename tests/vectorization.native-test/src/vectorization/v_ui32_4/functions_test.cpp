#include "vectorization_tests.h"

namespace vectorization::test
{
    TEST_CLASS (v_ui32_4_FunctionsTest)
    {
        public
        :
        TEST_METHOD(zerosWComponent)
        {
            const v_ui32_4 given{1u, 2u, 3u, 4u};
            const v_ui32_4 expected{1u, 2u, 3u, 0u};
            const auto actual = zeroW(given);
            Assert::IsTrue(allTrue(expected == actual), L"zeroW value mismatch", LINE_INFO());
        }

        TEST_METHOD(computesSqr)
        {
            const v_ui32_4 given{2u, 3u, 4u, 0u};
            const v_ui32_4 expected{4u, 9u, 16u, 0u};
            const auto actual = sqr(given);
            Assert::IsTrue(allTrue(expected == actual), L"sqr value mismatch", LINE_INFO());
        }

        TEST_METHOD(computesComponentWiseMin)
        {
            const v_ui32_4 a{1u, 5u, 3u, 8u};
            const v_ui32_4 b{4u, 2u, 6u, 1u};
            const v_ui32_4 expected{1u, 2u, 3u, 1u};
            const auto actual = min(a, b);
            Assert::IsTrue(allTrue(expected == actual), L"min value mismatch", LINE_INFO());
        }

        TEST_METHOD(computesScalarMin)
        {
            const v_ui32_4 given{3u, 1u, 4u, 2u};
            Assert::AreEqual(UInt_32{1}, min(given), L"scalar min mismatch", LINE_INFO());
        }

        TEST_METHOD(computesScalarMin3)
        {
            const v_ui32_4 given{3u, 1u, 4u, 0u};
            Assert::AreEqual(UInt_32{1}, min3(given), L"scalar min3 mismatch", LINE_INFO());
        }

        TEST_METHOD(computesComponentWiseMax)
        {
            const v_ui32_4 a{1u, 5u, 3u, 8u};
            const v_ui32_4 b{4u, 2u, 6u, 1u};
            const v_ui32_4 expected{4u, 5u, 6u, 8u};
            const auto actual = max(a, b);
            Assert::IsTrue(allTrue(expected == actual), L"max value mismatch", LINE_INFO());
        }

        TEST_METHOD(computesScalarMax)
        {
            const v_ui32_4 given{3u, 1u, 4u, 2u};
            Assert::AreEqual(UInt_32{4}, max(given), L"scalar max mismatch", LINE_INFO());
        }

        TEST_METHOD(computesScalarMax3)
        {
            const v_ui32_4 given{3u, 1u, 4u, 99u};
            Assert::AreEqual(UInt_32{4}, max3(given), L"scalar max3 mismatch", LINE_INFO());
        }

        TEST_METHOD(clampsValues)
        {
            const v_ui32_4 given{0u, 3u, 15u, 7u};
            const v_ui32_4 lo{1u, 1u, 1u, 1u};
            const v_ui32_4 hi{10u, 10u, 10u, 10u};
            const v_ui32_4 expected{1u, 3u, 10u, 7u};
            const auto actual = clamp(given, lo, hi);
            Assert::IsTrue(allTrue(expected == actual), L"clamp value mismatch", LINE_INFO());
        }

        TEST_METHOD(checksAnyTrue)
        {
            Assert::IsTrue(anyTrue(v_ui32_4{1u, 0u, 0u, 0u}), L"anyTrue should be true", LINE_INFO());
            Assert::IsTrue(anyTrue(v_ui32_4{0u, 0u, 0u, 1u}), L"anyTrue should be true", LINE_INFO());
            Assert::IsFalse(anyTrue(Zero<v_ui32_4>()), L"anyTrue should be false", LINE_INFO());
        }

        TEST_METHOD(checksAllTrue)
        {
            Assert::IsTrue(allTrue(MaskAll<v_ui32_4>()), L"allTrue should be true", LINE_INFO());
            Assert::IsFalse(allTrue(v_ui32_4{UInt_32{0xFFFFFFFF}, 0u, UInt_32{0xFFFFFFFF}, UInt_32{0xFFFFFFFF}}),
                            L"allTrue should be false", LINE_INFO());
        }

        TEST_METHOD(checksAnyFalse)
        {
            Assert::IsTrue(anyFalse(v_ui32_4{UInt_32{0xFFFFFFFF}, 0u, UInt_32{0xFFFFFFFF}, UInt_32{0xFFFFFFFF}}),
                           L"anyFalse should be true", LINE_INFO());
            Assert::IsFalse(anyFalse(MaskAll<v_ui32_4>()), L"anyFalse should be false", LINE_INFO());
        }

        TEST_METHOD(checksAllFalse)
        {
            Assert::IsTrue(allFalse(Zero<v_ui32_4>()), L"allFalse should be true", LINE_INFO());
            Assert::IsFalse(allFalse(v_ui32_4{0u, 1u, 0u, 0u}), L"allFalse should be false", LINE_INFO());
        }

        TEST_METHOD(computesLength)
        {
            const v_ui32_4 given{1u, 2u, 3u, 4u};
            Assert::AreEqual(UInt_32{10}, length(given), L"length mismatch", LINE_INFO());
        }

        TEST_METHOD(computesLength3)
        {
            const v_ui32_4 given{1u, 2u, 3u, 99u};
            Assert::AreEqual(UInt_32{6}, length3(given), L"length3 mismatch", LINE_INFO());
        }

        TEST_METHOD(computesLengthV)
        {
            const v_ui32_4 given{1u, 2u, 3u, 4u};
            const auto actual = lengthv(given);
            Assert::AreEqual(UInt_32{10}, x(actual), L"lengthv x mismatch", LINE_INFO());
            Assert::AreEqual(UInt_32{10}, y(actual), L"lengthv y mismatch", LINE_INFO());
            Assert::AreEqual(UInt_32{10}, z(actual), L"lengthv z mismatch", LINE_INFO());
            Assert::AreEqual(UInt_32{10}, w(actual), L"lengthv w mismatch", LINE_INFO());
        }

        TEST_METHOD(computesLength3V)
        {
            const v_ui32_4 given{1u, 2u, 3u, 99u};
            const auto actual = length3v(given);
            Assert::AreEqual(UInt_32{6}, x(actual), L"length3v x mismatch", LINE_INFO());
            Assert::AreEqual(UInt_32{6}, y(actual), L"length3v y mismatch", LINE_INFO());
            Assert::AreEqual(UInt_32{6}, z(actual), L"length3v z mismatch", LINE_INFO());
            Assert::AreEqual(UInt_32{6}, w(actual), L"length3v w mismatch", LINE_INFO());
        }

        TEST_METHOD(computesDistance)
        {
            const v_ui32_4 a{10u, 20u, 30u, 40u};
            const v_ui32_4 b{1u, 2u, 3u, 4u};
            Assert::AreEqual(UInt_32{90}, distance(a, b), L"distance mismatch", LINE_INFO());
        }

        TEST_METHOD(computesDistance3)
        {
            const v_ui32_4 a{10u, 20u, 30u, 40u};
            const v_ui32_4 b{1u, 2u, 3u, 4u};
            Assert::AreEqual(UInt_32{54}, distance3(a, b), L"distance3 mismatch", LINE_INFO());
        }

        TEST_METHOD(computesDistanceWithoutWrapAround)
        {
            // Components of b above those of a must not wrap around.
            const v_ui32_4 a{1u, 20u, 3u, 40u};
            const v_ui32_4 b{10u, 2u, 30u, 4u};
            // 9 + 18 + 27 + 36 = 90, without w: 54
            Assert::AreEqual(UInt_32{90}, distance(a, b), L"distance mismatch", LINE_INFO());
            Assert::AreEqual(UInt_32{90}, distance(b, a), L"distance is symmetric", LINE_INFO());
            Assert::AreEqual(UInt_32{54}, distance3(a, b), L"distance3 mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_ui32_4{90u} == distancev(a, b)), L"distancev mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_ui32_4{54u} == distance3v(a, b)), L"distance3v mismatch", LINE_INFO());
        }

        TEST_METHOD(setsOneW)
        {
            const v_ui32_4 given{1u, 2u, 3u, 4u};
            const v_ui32_4 expected{1u, 2u, 3u, 1u};
            Assert::IsTrue(allTrue(expected == oneW(given)), L"oneW value mismatch", LINE_INFO());
        }

        TEST_METHOD(computesVectorMinAndMax)
        {
            const v_ui32_4 given{3u, 1u, 4u, 2u};
            Assert::IsTrue(allTrue(v_ui32_4{1u} == minv(given)), L"minv mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_ui32_4{4u} == maxv(given)), L"maxv mismatch", LINE_INFO());
        }

        TEST_METHOD(computesVectorMin3AndMax3)
        {
            const v_ui32_4 low{3u, 2u, 4u, 0u};
            Assert::IsTrue(allTrue(v_ui32_4{2u} == min3v(low)), L"min3v mismatch", LINE_INFO());
            const v_ui32_4 high{3u, 1u, 4u, 99u};
            Assert::IsTrue(allTrue(v_ui32_4{4u} == max3v(high)), L"max3v mismatch", LINE_INFO());
        }

        TEST_METHOD(computesHorizontalSum)
        {
            const v_ui32_4 given{1u, 2u, 3u, 40u};
            Assert::AreEqual(UInt_32{46}, horizontalSum(given), L"horizontalSum mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_ui32_4{46u} == horizontalSumv(given)), L"horizontalSumv mismatch", LINE_INFO());
            Assert::AreEqual(UInt_32{6}, horizontalSum3(given), L"horizontalSum3 mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_ui32_4{6u} == horizontalSum3v(given)), L"horizontalSum3v mismatch", LINE_INFO());
        }

        TEST_METHOD(computesDotProduct)
        {
            const v_ui32_4 a{1u, 2u, 3u, 4u};
            const v_ui32_4 b{5u, 6u, 7u, 8u};
            // 5 + 12 + 21 + 32 = 70, without w: 38
            Assert::AreEqual(UInt_32{70}, dot(a, b), L"dot mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_ui32_4{70u} == dotv(a, b)), L"dotv mismatch", LINE_INFO());
            Assert::AreEqual(UInt_32{38}, dot3(a, b), L"dot3 mismatch", LINE_INFO());
            Assert::IsTrue(allTrue(v_ui32_4{38u} == dot3v(a, b)), L"dot3v mismatch", LINE_INFO());
        }

        TEST_METHOD(computesMultiplyAddFamily)
        {
            const v_ui32_4 a{1u, 2u, 3u, 4u};
            const v_ui32_4 b{50u, 60u, 70u, 80u};
            const v_ui32_4 c{10u, 20u, 30u, 40u};
            // a * b = {50, 120, 210, 320}
            Assert::IsTrue(
                allTrue(v_ui32_4{60u, 140u, 240u, 360u} == multiplyAdd(a, b, c)), L"multiplyAdd", LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_ui32_4{40u, 100u, 180u, 280u} == multiplySub(a, b, c)), L"multiplySub", LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_ui32_4{40u, 140u, 180u, 360u} == multiplyAddSub(a, b, c)), L"multiplyAddSub", LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(v_ui32_4{60u, 100u, 240u, 280u} == multiplySubAdd(a, b, c)), L"multiplySubAdd", LINE_INFO()
            );
            // c - a * b and -(a * b) - c wrap around
            const v_ui32_4 small{1u, 1u, 1u, 1u};
            const v_ui32_4 large{100u, 100u, 100u, 100u};
            Assert::IsTrue(
                allTrue(v_ui32_4{90u, 80u, 70u, 60u} == negativeMultiplyAdd(small, c, large)), L"negativeMultiplyAdd",
                LINE_INFO()
            );
            Assert::IsTrue(
                allTrue(
                    v_ui32_4{UInt_32(-60), UInt_32(-140), UInt_32(-240), UInt_32(-360)} == negativeMultiplySub(a, b, c)
                ),
                L"negativeMultiplySub", LINE_INFO()
            );
        }

        TEST_METHOD(computesAddSubtract)
        {
            const v_ui32_4 a{10u, 20u, 30u, 40u};
            const v_ui32_4 b{1u, 2u, 3u, 4u};
            const v_ui32_4 expected{9u, 22u, 27u, 44u};
            Assert::IsTrue(allTrue(expected == addSubtract(a, b)), L"addSubtract mismatch", LINE_INFO());
        }

        TEST_METHOD(computesModulo)
        {
            const v_ui32_4 a{7u, 8u, 9u, 10u};
            const v_ui32_4 b{3u, 3u, 4u, 4u};
            const v_ui32_4 expected{1u, 2u, 1u, 2u};
            Assert::IsTrue(allTrue(expected == modulo(a, b)), L"modulo mismatch", LINE_INFO());
        }

        TEST_METHOD(findsArgMinAndArgMax)
        {
            Assert::AreEqual(ASizeT{1}, argmin(v_ui32_4{3u, 0u, 4u, 2u}), L"argmin mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{3}, argmin(v_ui32_4{3u, 1u, 4u, 0u}), L"argmin W mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{2}, argmax(v_ui32_4{3u, 1u, 4u, 2u}), L"argmax mismatch", LINE_INFO());
            // above INT32_MAX: compared unsigned
            Assert::AreEqual(ASizeT{0}, argmax(v_ui32_4{UInt_32(-1), 1u, 4u, 2u}), L"argmax X mismatch", LINE_INFO());
        }

        TEST_METHOD(picksHighestIndexOfEqualExtrema)
        {
            Assert::AreEqual(ASizeT{2}, argmin(v_ui32_4{1u, 5u, 1u, 7u}), L"argmin tie", LINE_INFO());
            Assert::AreEqual(ASizeT{3}, argmax(v_ui32_4{7u, 5u, 1u, 7u}), L"argmax tie", LINE_INFO());
            Assert::AreEqual(ASizeT{1}, argmin3(v_ui32_4{1u, 1u, 5u, 0u}), L"argmin3 tie", LINE_INFO());
            Assert::AreEqual(ASizeT{2}, argmax3(v_ui32_4{5u, 1u, 5u, 9u}), L"argmax3 tie", LINE_INFO());
        }

        TEST_METHOD(findsArgMin3AndArgMax3IgnoringW)
        {
            Assert::AreEqual(ASizeT{2}, argmin3(v_ui32_4{3u, 2u, 1u, 0u}), L"argmin3 mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{0}, argmin3(v_ui32_4{1u, 2u, 3u, 0u}), L"argmin3 X mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{1}, argmax3(v_ui32_4{1u, 3u, 2u, 99u}), L"argmax3 mismatch", LINE_INFO());
            Assert::AreEqual(ASizeT{0}, argmax3(v_ui32_4{4u, 3u, 2u, 99u}), L"argmax3 X mismatch", LINE_INFO());
        }

        TEST_METHOD(testsTruthBitwise)
        {
            // Components with only some bits set are neither fully true nor false.
            const v_ui32_4 partial{1u};
            Assert::IsTrue(anyTrue(partial), L"anyTrue of partial bits", LINE_INFO());
            Assert::IsTrue(anyFalse(partial), L"anyFalse of partial bits", LINE_INFO());
            Assert::IsFalse(allTrue(partial), L"allTrue of partial bits", LINE_INFO());
            Assert::IsFalse(allFalse(partial), L"allFalse of partial bits", LINE_INFO());
        }

        TEST_METHOD(checksAnyTrue3)
        {
            Assert::IsTrue(anyTrue3(v_ui32_4{0u, 0u, 1u, 0u}), L"should detect true in Z", LINE_INFO());
            Assert::IsFalse(anyTrue3(v_ui32_4{0u, 0u, 0u, 99u}), L"W should be ignored", LINE_INFO());
        }

        TEST_METHOD(checksAnyFalse3)
        {
            Assert::IsTrue(anyFalse3(v_ui32_4{UInt_32(-1), 0u, UInt_32(-1), UInt_32(-1)}), L"should detect false in Y",
                           LINE_INFO());
            Assert::IsFalse(anyFalse3(v_ui32_4{UInt_32(-1), UInt_32(-1), UInt_32(-1), 0u}), L"W should be ignored",
                            LINE_INFO());
        }

        TEST_METHOD(checksAllTrue3)
        {
            Assert::IsTrue(allTrue3(v_ui32_4{UInt_32(-1), UInt_32(-1), UInt_32(-1), 0u}), L"W should be ignored",
                           LINE_INFO());
            Assert::IsFalse(allTrue3(v_ui32_4{UInt_32(-1), 0u, UInt_32(-1), UInt_32(-1)}), L"Y is zero", LINE_INFO());
        }

        TEST_METHOD(checksAllFalse3)
        {
            Assert::IsTrue(allFalse3(v_ui32_4{0u, 0u, 0u, 99u}), L"W should be ignored", LINE_INFO());
            Assert::IsFalse(allFalse3(v_ui32_4{0u, 1u, 0u, 0u}), L"Y is nonzero", LINE_INFO());
        }
    };
}
