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
    };
}
