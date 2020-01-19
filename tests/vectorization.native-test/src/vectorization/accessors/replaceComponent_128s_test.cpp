#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(ReplaceComponent128sTest)
        {
        public:

            TEST_METHOD(replacesComponent) {
                auto givenReplacement = 5.0f;
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::W>();
                auto givenPack = _mm_load_ps(given.data());
                std::array<Float_32, VectorSizes::W> expected{ };
                std::array<Float_32, VectorSizes::W> actual{ };

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_ps(actual.data(), replaceComponent<VectorIndices::X>(givenPack, givenReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::Y>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_ps(actual.data(), replaceComponent<VectorIndices::Y>(givenPack, givenReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::Z>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_ps(actual.data(), replaceComponent<VectorIndices::Z>(givenPack, givenReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::W>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_ps(actual.data(), replaceComponent<VectorIndices::W>(givenPack, givenReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(replacesXYZW) {
                auto givenReplacement = 5.0f;
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::W>();
                auto givenPack = _mm_load_ps(given.data());
                std::array<Float_32, VectorSizes::W> expected{ };
                std::array<Float_32, VectorSizes::W> actual{ };

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_ps(actual.data(), replaceX(givenPack, givenReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::Y>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_ps(actual.data(), replaceY(givenPack, givenReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::Z>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_ps(actual.data(), replaceZ(givenPack, givenReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::W>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_ps(actual.data(), replaceW(givenPack, givenReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(replacesXi) {
                auto givenReplacement = 5.0f;
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::X4>();
                auto givenPack = _mm_load_ps(given.data());
                std::array<Float_32, VectorSizes::X4> expected{ };
                std::array<Float_32, VectorSizes::X4> actual{ };

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X1>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_ps(actual.data(), replaceX1(givenPack, givenReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X2>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_ps(actual.data(), replaceX2(givenPack, givenReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X3>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_ps(actual.data(), replaceX3(givenPack, givenReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X4>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_ps(actual.data(), replaceX4(givenPack, givenReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(replacesComponentPackedFloat4_128AtIndex) {
                auto givenReplacement = 5.0f;
                auto givenReplacementPack = _mm_set1_ps(givenReplacement);
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::W>();
                auto givenPack = _mm_load_ps(given.data());
                std::array<Float_32, VectorSizes::W> expected{ };
                std::array<Float_32, VectorSizes::W> actual{ };

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X1>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_ps(actual.data(), replaceComponent(givenPack, givenReplacementPack, VectorIndices::X1));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X2>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_ps(actual.data(), replaceComponent(givenPack, givenReplacementPack, VectorIndices::X2));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X3>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_ps(actual.data(), replaceComponent(givenPack, givenReplacementPack, VectorIndices::X3));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X4>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_ps(actual.data(), replaceComponent(givenPack, givenReplacementPack, VectorIndices::X4));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    _mm_store_ps(actual.data(), replaceComponent(givenPack, givenReplacementPack, VectorIndices::X5));
                    Assert::AreEqual(given, actual, L"wrong replace component mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(replacesComponentFloat_32AtIndex) {
                auto givenReplacement = 5.0f;
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::W>();
                auto givenPack = _mm_load_ps(given.data());
                std::array<Float_32, VectorSizes::W> expected{ };
                std::array<Float_32, VectorSizes::W> actual{ };

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X1>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_ps(actual.data(), replaceComponent(givenPack, givenReplacement, VectorIndices::X1));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X2>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_ps(actual.data(), replaceComponent(givenPack, givenReplacement, VectorIndices::X2));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X3>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_ps(actual.data(), replaceComponent(givenPack, givenReplacement, VectorIndices::X3));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X4>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_ps(actual.data(), replaceComponent(givenPack, givenReplacement, VectorIndices::X4));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    _mm_store_ps(actual.data(), replaceComponent(givenPack, givenReplacement, VectorIndices::X5));
                    Assert::AreEqual(given, actual, L"wrong replace component mismatch", LINE_INFO());
                }
            }
        };
    }
}