#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(ReplaceComponent128dTest)
        {
        public:

            TEST_METHOD(replacesComponent) {
                auto givenReplacement = 5.0;
                auto given = StandardSample::ofArrayType<Float_64, VectorSizes::Y>();
                auto givenPack = _mm_load_pd(given.data());
                std::array<Float_64, VectorSizes::Y> expected{ };
                std::array<Float_64, VectorSizes::Y> actual{ };

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_pd(actual.data(), replaceComponent<VectorIndices::X>(givenPack, givenReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::Y>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_pd(actual.data(), replaceComponent<VectorIndices::Y>(givenPack, givenReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(replacesXY) {
                auto givenReplacement = 5.0;
                auto given = StandardSample::ofArrayType<Float_64, VectorSizes::Y>();
                auto givenPack = _mm_load_pd(given.data());
                std::array<Float_64, VectorSizes::Y> expected{ };
                std::array<Float_64, VectorSizes::Y> actual{ };

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_pd(actual.data(), replaceX(givenPack, givenReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::Y>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_pd(actual.data(), replaceY(givenPack, givenReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(replacesXi) {
                auto givenReplacement = 5.0;
                auto given = StandardSample::ofArrayType<Float_64, VectorSizes::X2>();
                auto givenPack = _mm_load_pd(given.data());
                std::array<Float_64, VectorSizes::X2> expected{ };
                std::array<Float_64, VectorSizes::X2> actual{ };

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X1>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_pd(actual.data(), replaceX1(givenPack, givenReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X2>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_pd(actual.data(), replaceX2(givenPack, givenReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(replacesComponentPackedFloat2_128AtIndex) {
                auto givenReplacement = 5.0;
                auto givenReplacementPack = _mm_set1_pd(givenReplacement);
                auto given = StandardSample::ofArrayType<Float_64, VectorSizes::X2>();
                auto givenPack = _mm_load_pd(given.data());
                std::array<Float_64, VectorSizes::X2> expected{ };
                std::array<Float_64, VectorSizes::X2> actual{ };

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X1>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_pd(actual.data(), replaceComponent(givenPack, givenReplacementPack, VectorIndices::X1));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X2>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_pd(actual.data(), replaceComponent(givenPack, givenReplacementPack, VectorIndices::X2));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    _mm_store_pd(actual.data(), replaceComponent(givenPack, givenReplacementPack, VectorIndices::X3));
                    Assert::AreEqual(given, actual, L"wrong replace component mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(replacesComponentFloat_64AtIndex) {
                auto givenReplacement = 5.0;
                auto given = StandardSample::ofArrayType<Float_64, VectorSizes::X2>();
                auto givenPack = _mm_load_pd(given.data());
                std::array<Float_64, VectorSizes::X2> expected{ };
                std::array<Float_64, VectorSizes::X2> actual{ };

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X1>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_pd(actual.data(), replaceComponent(givenPack, givenReplacement, VectorIndices::X1));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X2>(given.cbegin(), given.cend(), expected.begin(), givenReplacement);
                    _mm_store_pd(actual.data(), replaceComponent(givenPack, givenReplacement, VectorIndices::X2));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    _mm_store_pd(actual.data(), replaceComponent(givenPack, givenReplacement, VectorIndices::X3));
                    Assert::AreEqual(given, actual, L"wrong replace component mismatch", LINE_INFO());
                }
            }
        };
    }
}