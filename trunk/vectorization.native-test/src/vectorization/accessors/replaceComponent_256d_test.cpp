#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(ReplaceComponent256dTest)
        {
        public:

            TEST_METHOD(replacesComponent) {
                auto expectedReplacement = 5.0;
                auto given = StandardSample::ofArrayType<Float_64, VectorSizes::W>();
                auto givenPack = _mm256_load_pd(given.data());
                std::array<Float_64, VectorSizes::W> expected{};
                std::array<Float_64, VectorSizes::W> actual{};

                {
                    std::copy(given.cbegin(), given.cend(), expected.begin());
                    expected.at(VectorIndices::X) = expectedReplacement;
                    _mm256_store_pd(actual.data(), replaceComponent<VectorIndices::X>(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    std::copy(given.cbegin(), given.cend(), expected.begin());
                    expected.at(VectorIndices::Y) = expectedReplacement;
                    _mm256_store_pd(actual.data(), replaceComponent<VectorIndices::Y>(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    std::copy(given.cbegin(), given.cend(), expected.begin());
                    expected.at(VectorIndices::Z) = expectedReplacement;
                    _mm256_store_pd(actual.data(), replaceComponent<VectorIndices::Z>(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    std::copy(given.cbegin(), given.cend(), expected.begin());
                    expected.at(VectorIndices::W) = expectedReplacement;
                    _mm256_store_pd(actual.data(), replaceComponent<VectorIndices::W>(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(replacesXYZW) {
                auto expectedReplacement = 5.0;
                auto given = StandardSample::ofArrayType<Float_64, VectorSizes::W>();
                auto givenPack = _mm256_load_pd(given.data());
                std::array<Float_64, VectorSizes::W> expected{};
                std::array<Float_64, VectorSizes::W> actual{};

                {
                    std::copy(given.cbegin(), given.cend(), expected.begin());
                    expected.at(VectorIndices::X) = expectedReplacement;
                    _mm256_store_pd(actual.data(), replaceX(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    std::copy(given.cbegin(), given.cend(), expected.begin());
                    expected.at(VectorIndices::Y) = expectedReplacement;
                    _mm256_store_pd(actual.data(), replaceY(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    std::copy(given.cbegin(), given.cend(), expected.begin());
                    expected.at(VectorIndices::Z) = expectedReplacement;
                    _mm256_store_pd(actual.data(), replaceZ(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    std::copy(given.cbegin(), given.cend(), expected.begin());
                    expected.at(VectorIndices::W) = expectedReplacement;
                    _mm256_store_pd(actual.data(), replaceW(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(readsXi) {
                auto expectedReplacement = 5.0;
                auto given = StandardSample::ofArrayType<Float_64, VectorSizes::X4>();
                auto givenPack = _mm256_load_pd(given.data());
                std::array<Float_64, VectorSizes::X4> expected{};
                std::array<Float_64, VectorSizes::X4> actual{};

                {
                    std::copy(given.cbegin(), given.cend(), expected.begin());
                    expected.at(VectorIndices::X1) = expectedReplacement;
                    _mm256_store_pd(actual.data(), replaceX1(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    std::copy(given.cbegin(), given.cend(), expected.begin());
                    expected.at(VectorIndices::X2) = expectedReplacement;
                    _mm256_store_pd(actual.data(), replaceX2(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    std::copy(given.cbegin(), given.cend(), expected.begin());
                    expected.at(VectorIndices::X3) = expectedReplacement;
                    _mm256_store_pd(actual.data(), replaceX3(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    std::copy(given.cbegin(), given.cend(), expected.begin());
                    expected.at(VectorIndices::X4) = expectedReplacement;
                    _mm256_store_pd(actual.data(), replaceX4(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }
            }
        };
    }
}