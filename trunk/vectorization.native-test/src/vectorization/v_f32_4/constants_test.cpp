#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(v_f32_4_ConstantsTest)
        {
        public:

            TEST_METHOD(hasMaskAll) {
                auto expected = MaskAll<v_f32_4::VectorBoolType>();
                auto actual = _mm_castps_si128(MaskAll<v_f32_4>());

                Assert::IsTrue(allTrue(expected == actual), L"constant mismatch", LINE_INFO());
            }

            //TEST_METHOD(hasMaskNone) {
            //    auto expected = MaskNone<v_f32_4::VectorBoolType>();
            //    auto actual = _mm_castps_si128(MaskNone<v_f32_4>());

            //    Assert::IsTrue(allTrue(expected == actual), L"constant mismatch", LINE_INFO());
            //}

            TEST_METHOD(hasMaskX) {
                auto expected = MaskX<v_f32_4::VectorBoolType>();
                auto actual = _mm_castps_si128(MaskX<v_f32_4>());

                Assert::IsTrue(allTrue(expected == actual), L"constant mismatch", LINE_INFO());
            }

            TEST_METHOD(hasMaskY) {
                auto expected = MaskY<v_f32_4::VectorBoolType>();
                auto actual = _mm_castps_si128(MaskY<v_f32_4>());

                Assert::IsTrue(allTrue(expected == actual), L"constant mismatch", LINE_INFO());
            }

            TEST_METHOD(hasMaskZ) {
                auto expected = MaskZ<v_f32_4::VectorBoolType>();
                auto actual = _mm_castps_si128(MaskZ<v_f32_4>());

                Assert::IsTrue(allTrue(expected == actual), L"constant mismatch", LINE_INFO());
            }

            TEST_METHOD(hasMaskW) {
                auto expected = MaskW<v_f32_4::VectorBoolType>();
                auto actual = _mm_castps_si128(MaskW<v_f32_4>());

                Assert::IsTrue(allTrue(expected == actual), L"constant mismatch", LINE_INFO());
            }
        };
    }
}