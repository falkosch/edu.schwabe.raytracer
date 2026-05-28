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
    };
}
