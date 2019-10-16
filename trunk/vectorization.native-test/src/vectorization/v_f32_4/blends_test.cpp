#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(v_f32_4_BlendsTest)
        {
        public:

            TEST_METHOD(genericallyBlendsIdentities) {
                v_f32_4 givenOnBitNotSet{ 1.0f, 2.0f, 3.0f, 4.0f };
                v_f32_4 givenOnBitSet{ 5.0f, 6.0f, 7.0f, 8.0f };

                {
                    v_f32_4 expected{
                        x(givenOnBitNotSet), y(givenOnBitNotSet), z(givenOnBitNotSet), w(givenOnBitNotSet)
                    };
                    auto actual = blend<false, false, false, false>(givenOnBitNotSet, givenOnBitSet);
                    Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x(givenOnBitSet), y(givenOnBitSet), z(givenOnBitSet), w(givenOnBitSet)
                    };
                    auto actual = blend<true, true, true, true>(givenOnBitNotSet, givenOnBitSet);
                    Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallyBlendsIndividuals) {
                v_f32_4 givenOnBitNotSet{ 1.0f, 2.0f, 3.0f, 4.0f };
                v_f32_4 givenOnBitSet{ 5.0f, 6.0f, 7.0f, 8.0f };

                {
                    v_f32_4 expected{
                        x(givenOnBitSet), y(givenOnBitNotSet), z(givenOnBitNotSet), w(givenOnBitNotSet)
                    };
                    auto actual = blend<true, false, false, false>(givenOnBitNotSet, givenOnBitSet);
                    Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x(givenOnBitNotSet), y(givenOnBitSet), z(givenOnBitNotSet), w(givenOnBitNotSet)
                    };
                    auto actual = blend<false, true, false, false>(givenOnBitNotSet, givenOnBitSet);
                    Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x(givenOnBitNotSet), y(givenOnBitNotSet), z(givenOnBitSet), w(givenOnBitNotSet)
                    };
                    auto actual = blend<false, false, true, false>(givenOnBitNotSet, givenOnBitSet);
                    Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x(givenOnBitNotSet), y(givenOnBitNotSet), z(givenOnBitNotSet), w(givenOnBitSet)
                    };
                    auto actual = blend<false, false, false, true>(givenOnBitNotSet, givenOnBitSet);
                    Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallyBlendsPairs) {
                v_f32_4 givenOnBitNotSet{ 1.0f, 2.0f, 3.0f, 4.0f };
                v_f32_4 givenOnBitSet{ 5.0f, 6.0f, 7.0f, 8.0f };

                {
                    v_f32_4 expected{
                        x(givenOnBitSet), y(givenOnBitSet), z(givenOnBitNotSet), w(givenOnBitNotSet)
                    };
                    auto actual = blend<true, true, false, false>(givenOnBitNotSet, givenOnBitSet);
                    Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x(givenOnBitSet), y(givenOnBitNotSet), z(givenOnBitSet), w(givenOnBitNotSet)
                    };
                    auto actual = blend<true, false, true, false>(givenOnBitNotSet, givenOnBitSet);
                    Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x(givenOnBitNotSet), y(givenOnBitSet), z(givenOnBitNotSet), w(givenOnBitSet)
                    };
                    auto actual = blend<false, true, false, true>(givenOnBitNotSet, givenOnBitSet);
                    Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x(givenOnBitNotSet), y(givenOnBitNotSet), z(givenOnBitSet), w(givenOnBitSet)
                    };
                    auto actual = blend<false, false, true, true>(givenOnBitNotSet, givenOnBitSet);
                    Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallyBlendsTriplets) {
                v_f32_4 givenOnBitNotSet{ 1.0f, 2.0f, 3.0f, 4.0f };
                v_f32_4 givenOnBitSet{ 5.0f, 6.0f, 7.0f, 8.0f };

                {
                    v_f32_4 expected{
                        x(givenOnBitNotSet), y(givenOnBitSet), z(givenOnBitSet), w(givenOnBitSet)
                    };
                    auto actual = blend<false, true, true, true>(givenOnBitNotSet, givenOnBitSet);
                    Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x(givenOnBitSet), y(givenOnBitNotSet), z(givenOnBitSet), w(givenOnBitSet)
                    };
                    auto actual = blend<true, false, true, true>(givenOnBitNotSet, givenOnBitSet);
                    Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x(givenOnBitSet), y(givenOnBitSet), z(givenOnBitNotSet), w(givenOnBitSet)
                    };
                    auto actual = blend<true, true, false, true>(givenOnBitNotSet, givenOnBitSet);
                    Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x(givenOnBitSet), y(givenOnBitSet), z(givenOnBitSet), w(givenOnBitNotSet)
                    };
                    auto actual = blend<true, true, true, false>(givenOnBitNotSet, givenOnBitSet);
                    Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(hasShortFormsForBlends) {
                v_f32_4 givenOnBitNotSet{ 1.0f, 2.0f, 3.0f, 4.0f };
                v_f32_4 givenOnBitSet{ 5.0f, 6.0f, 7.0f, 8.0f };

                {
                    v_f32_4 expected{
                        x(givenOnBitNotSet), y(givenOnBitSet), z(givenOnBitSet), w(givenOnBitSet)
                    };
                    auto actual = x_yzw(givenOnBitNotSet, givenOnBitSet);
                    Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(variadicBlendsTwoFloat4_32WithFloat4_32BitMask) {
                v_f32_4 givenOnBitNotSet{ 1.0f, 2.0f, 3.0f, 4.0f };
                v_f32_4 givenOnBitSet{ 5.0f, 6.0f, 7.0f, 8.0f };

                {
                    v_f32_4 expected{
                        x(givenOnBitNotSet), y(givenOnBitNotSet), z(givenOnBitNotSet), w(givenOnBitNotSet)
                    };
                    auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskNone<v_f32_4>());
                    Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x(givenOnBitSet), y(givenOnBitNotSet), z(givenOnBitNotSet), w(givenOnBitNotSet)
                    };
                    auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskX<v_f32_4>());
                    Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x(givenOnBitSet), y(givenOnBitSet), z(givenOnBitNotSet), w(givenOnBitNotSet)
                    };
                    auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskXY<v_f32_4>());
                    Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x(givenOnBitSet), y(givenOnBitSet), z(givenOnBitSet), w(givenOnBitNotSet)
                    };
                    auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskXYZ<v_f32_4>());
                    Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x(givenOnBitSet), y(givenOnBitSet), z(givenOnBitSet), w(givenOnBitSet)
                    };
                    auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskXYZW<v_f32_4>());
                    Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(variadicBlendsTwoFloat4_32WithBool4_32BitMask) {
                v_f32_4 givenOnBitNotSet{ 1.0f, 2.0f, 3.0f, 4.0f };
                v_f32_4 givenOnBitSet{ 5.0f, 6.0f, 7.0f, 8.0f };

                {
                    v_f32_4 expected{
                        x(givenOnBitNotSet), y(givenOnBitNotSet), z(givenOnBitNotSet), w(givenOnBitNotSet)
                    };
                    auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskNone<v_f32_4>());
                    Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x(givenOnBitSet), y(givenOnBitNotSet), z(givenOnBitNotSet), w(givenOnBitNotSet)
                    };
                    auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskX<v_f32_4>());
                    Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x(givenOnBitSet), y(givenOnBitSet), z(givenOnBitNotSet), w(givenOnBitNotSet)
                    };
                    auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskXY<v_f32_4>());
                    Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x(givenOnBitSet), y(givenOnBitSet), z(givenOnBitSet), w(givenOnBitNotSet)
                    };
                    auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskXYZ<v_f32_4>());
                    Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x(givenOnBitSet), y(givenOnBitSet), z(givenOnBitSet), w(givenOnBitSet)
                    };
                    auto actual = blendMasked(givenOnBitNotSet, givenOnBitSet, MaskXYZW<v_f32_4>());
                    Assert::IsTrue(allTrue(expected == actual), L"blend value mismatch", LINE_INFO());
                }
            }
        };
    }
}