#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(ReplaceComponent128dTest)
        {
        public:

            TEST_METHOD(replacesComponent) {
                auto expectedReplacement = 5.0;
                auto given = StandardSample::ofArrayType<Float_64, VectorSizes::Y>();
                auto givenPack = _mm_load_pd(given.data());
                std::array<Float_64, VectorSizes::Y> expected{};
                std::array<Float_64, VectorSizes::Y> actual{};

                {
                    std::copy(given.cbegin(), given.cend(), expected.begin());
                    expected.at(VectorIndices::X) = expectedReplacement;
                    _mm_store_pd(actual.data(), replaceComponent<VectorIndices::X>(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    std::copy(given.cbegin(), given.cend(), expected.begin());
                    expected.at(VectorIndices::Y) = expectedReplacement;
                    _mm_store_pd(actual.data(), replaceComponent<VectorIndices::Y>(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(replacesXY) {
                auto expectedReplacement = 5.0;
                auto given = StandardSample::ofArrayType<Float_64, VectorSizes::Y>();
                auto givenPack = _mm_load_pd(given.data());
                std::array<Float_64, VectorSizes::Y> expected{};
                std::array<Float_64, VectorSizes::Y> actual{};

                {
                    std::copy(given.cbegin(), given.cend(), expected.begin());
                    expected.at(VectorIndices::X) = expectedReplacement;
                    _mm_store_pd(actual.data(), replaceX(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    std::copy(given.cbegin(), given.cend(), expected.begin());
                    expected.at(VectorIndices::Y) = expectedReplacement;
                    _mm_store_pd(actual.data(), replaceY(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(readsXi) {
                auto expectedReplacement = 5.0;
                auto given = StandardSample::ofArrayType<Float_64, VectorSizes::X2>();
                auto givenPack = _mm_load_pd(given.data());
                std::array<Float_64, VectorSizes::X2> expected{};
                std::array<Float_64, VectorSizes::X2> actual{};

                {
                    std::copy(given.cbegin(), given.cend(), expected.begin());
                    expected.at(VectorIndices::X1) = expectedReplacement;
                    _mm_store_pd(actual.data(), replaceX1(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }

                {
                    std::copy(given.cbegin(), given.cend(), expected.begin());
                    expected.at(VectorIndices::X2) = expectedReplacement;
                    _mm_store_pd(actual.data(), replaceX2(givenPack, expectedReplacement));
                    Assert::AreNotEqual(given, actual, L"wrong replace component match", LINE_INFO());
                    Assert::AreEqual(expected, actual, L"replace component mismatch", LINE_INFO());
                }
            }
        };
    }
}