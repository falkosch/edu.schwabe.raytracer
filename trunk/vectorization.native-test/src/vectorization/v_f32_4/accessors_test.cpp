#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(v_f32_4_AccessorsTest)
        {
        public:

            TEST_METHOD(readsComponent) {
                auto given = StandardSample::ofVectorType<v_f32_4>();

                {
                    v_f32_4 actual{
                        component<VectorIndices::X>(given),
                        component<VectorIndices::Y>(given),
                        component<VectorIndices::Z>(given),
                        component<VectorIndices::W>(given)
                    };
                    Assert::IsTrue(allTrue(given == actual), L"component value mismatch", LINE_INFO());
                }

                {
                    v_f32_4 actual{
                        component<VectorIndices::W>(given),
                        component<VectorIndices::X>(given),
                        component<VectorIndices::Y>(given),
                        component<VectorIndices::Z>(given)
                    };
                    Assert::IsTrue(allTrue(given != actual), L"wrong component match", LINE_INFO());
                }
            }

            TEST_METHOD(readsXYZW) {
                auto given = StandardSample::ofVectorType<v_f32_4>();

                {
                    v_f32_4 actual{
                        x(given), y(given), z(given), w(given)
                    };
                    Assert::IsTrue(allTrue(given == actual), L"component value mismatch", LINE_INFO());
                }

                {
                    v_f32_4 actual{
                        w(given), x(given), y(given), z(given)
                    };
                    Assert::IsTrue(allTrue(given != actual), L"wrong component match", LINE_INFO());
                }
            }

            TEST_METHOD(readsXi) {
                auto given = StandardSample::ofVectorType<v_f32_4>();

                {
                    v_f32_4 actual{
                        x1(given), x2(given), x3(given), x4(given)
                    };
                    Assert::IsTrue(allTrue(given == actual), L"component value mismatch", LINE_INFO());
                }

                {
                    v_f32_4 actual{
                        x4(given), x1(given), x2(given), x3(given)
                    };
                    Assert::IsTrue(allTrue(given != actual), L"wrong component match", LINE_INFO());
                }
            }

            TEST_METHOD(replacesComponent) {
                auto expectedReplacement = 5.0f;
                auto given = StandardSample::ofVectorType<v_f32_4>();

                {
                    v_f32_4 expected{
                        expectedReplacement, y(given), z(given), w(given)
                    };
                    auto actual = replaceComponent<VectorIndices::X>(given, expectedReplacement);
                    Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
                    Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x(given), expectedReplacement, z(given), w(given)
                    };
                    auto actual = replaceComponent<VectorIndices::Y>(given, expectedReplacement);
                    Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
                    Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x(given), y(given), expectedReplacement, w(given)
                    };
                    auto actual = replaceComponent<VectorIndices::Z>(given, expectedReplacement);
                    Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
                    Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x(given), y(given), z(given), expectedReplacement
                    };
                    auto actual = replaceComponent<VectorIndices::W>(given, expectedReplacement);
                    Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
                    Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(replacesXYZW) {
                auto expectedReplacement = 5.0f;
                auto given = StandardSample::ofVectorType<v_f32_4>();

                {
                    v_f32_4 expected{
                        expectedReplacement, y(given), z(given), w(given)
                    };
                    auto actual = replaceX(given, expectedReplacement);
                    Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
                    Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x(given), expectedReplacement, z(given), w(given)
                    };
                    auto actual = replaceY(given, expectedReplacement);
                    Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
                    Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x(given), y(given), expectedReplacement, w(given)
                    };
                    auto actual = replaceZ(given, expectedReplacement);
                    Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
                    Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x(given), y(given), z(given), expectedReplacement
                    };
                    auto actual = replaceW(given, expectedReplacement);
                    Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
                    Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(replacesXi) {
                auto expectedReplacement = 5.0f;
                auto given = StandardSample::ofVectorType<v_f32_4>();

                {
                    v_f32_4 expected{
                        expectedReplacement, x2(given), x3(given), x4(given)
                    };
                    auto actual = replaceX1(given, expectedReplacement);
                    Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
                    Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x1(given), expectedReplacement, x3(given), x4(given)
                    };
                    auto actual = replaceX2(given, expectedReplacement);
                    Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
                    Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x1(given), x2(given), expectedReplacement, x4(given)
                    };
                    auto actual = replaceX3(given, expectedReplacement);
                    Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
                    Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
                }

                {
                    v_f32_4 expected{
                        x1(given), x2(given), x3(given), expectedReplacement
                    };
                    auto actual = replaceX4(given, expectedReplacement);
                    Assert::IsFalse(allTrue(given == actual), L"wrong replace component match", LINE_INFO());
                    Assert::IsTrue(allTrue(expected == actual), L"replace component mismatch", LINE_INFO());
                }
            }
        };
    }
}