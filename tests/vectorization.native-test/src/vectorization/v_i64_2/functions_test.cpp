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
            Assert::AreEqual(Int_64{-5LL}, y(actual), L"maxv Y mismatch", LINE_INFO());
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
            Assert::AreEqual(Int_64{5LL}, x(actual), L"lengthv X mismatch", LINE_INFO());
            Assert::AreEqual(Int_64{5LL}, y(actual), L"lengthv Y mismatch", LINE_INFO());
        }

        TEST_METHOD(computesScalarLength)
        {
            const v_i64_2 given{Int_64{10LL}, Int_64{-5LL}};
            Assert::AreEqual(Int_64{5LL}, length(given), L"length mismatch", LINE_INFO());
        }

        TEST_METHOD(computesDistance)
        {
            const v_i64_2 a{Int_64{10LL}, Int_64{-5LL}};
            const v_i64_2 b{Int_64{7LL}, Int_64{-10LL}};
            Assert::AreEqual(Int_64{8LL}, distance(a, b), L"distance mismatch", LINE_INFO());
        }
    };
}
