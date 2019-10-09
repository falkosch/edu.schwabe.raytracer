#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(ReplaceComponent256sTest)
        {
        public:

            TEST_METHOD(replacesComponent) {
                auto expectedReplacement = 5.0f;
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::X8>();
                auto givenPack = _mm256_load_ps(given.data());
                std::array<Float_32, VectorSizes::X8> expected{};
                std::array<Float_32, VectorSizes::X8> actual{};

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X1>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
                    _mm256_store_ps(actual.data(), replaceComponent<VectorIndices::X1>(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X2>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
                    _mm256_store_ps(actual.data(), replaceComponent<VectorIndices::X2>(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X3>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
                    _mm256_store_ps(actual.data(), replaceComponent<VectorIndices::X3>(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X4>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
                    _mm256_store_ps(actual.data(), replaceComponent<VectorIndices::X4>(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X5>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
                    _mm256_store_ps(actual.data(), replaceComponent<VectorIndices::X5>(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X6>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
                    _mm256_store_ps(actual.data(), replaceComponent<VectorIndices::X6>(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X7>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
                    _mm256_store_ps(actual.data(), replaceComponent<VectorIndices::X7>(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X8>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
                    _mm256_store_ps(actual.data(), replaceComponent<VectorIndices::X8>(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(replacesXYZW) {
                auto expectedReplacement = 5.0f;
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::X8>();
                auto givenPack = _mm256_load_ps(given.data());
                std::array<Float_32, VectorSizes::X8> expected{};
                std::array<Float_32, VectorSizes::X8> actual{};

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
                    _mm256_store_ps(actual.data(), replaceX(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::Y>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
                    _mm256_store_ps(actual.data(), replaceY(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::Z>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
                    _mm256_store_ps(actual.data(), replaceZ(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::W>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
                    _mm256_store_ps(actual.data(), replaceW(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(readsXi) {
                auto expectedReplacement = 5.0f;
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::X8>();
                auto givenPack = _mm256_load_ps(given.data());
                std::array<Float_32, VectorSizes::X8> expected{};
                std::array<Float_32, VectorSizes::X8> actual{};

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X1>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
                    _mm256_store_ps(actual.data(), replaceX1(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X2>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
                    _mm256_store_ps(actual.data(), replaceX2(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X3>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
                    _mm256_store_ps(actual.data(), replaceX3(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X4>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
                    _mm256_store_ps(actual.data(), replaceX4(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X5>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
                    _mm256_store_ps(actual.data(), replaceX5(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X6>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
                    _mm256_store_ps(actual.data(), replaceX6(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X7>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
                    _mm256_store_ps(actual.data(), replaceX7(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    Mutate::copyAndReplaceAt<VectorIndices::X8>(given.cbegin(), given.cend(), expected.begin(), expectedReplacement);
                    _mm256_store_ps(actual.data(), replaceX8(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }
            }
        };
    }
}