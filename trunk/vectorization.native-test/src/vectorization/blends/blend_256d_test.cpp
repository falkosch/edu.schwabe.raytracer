#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(Blend256dTest)
        {
        public:

            TEST_METHOD(genericallyBlendsIdentities) {
                std::array<Float_64, VectorSizes::W> givenOnBitNotSet{ 1.0, 2.0, 3.0, 4.0 };
                std::array<Float_64, VectorSizes::W> givenOnBitSet{ 5.0, 6.0, 7.0, 8.0 };
                std::array<Float_64, VectorSizes::W> actual{};

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<false, false, false, false>(_mm256_load_pd(givenOnBitNotSet.data()), _mm256_load_pd(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<true, true, true, true>(_mm256_load_pd(givenOnBitNotSet.data()), _mm256_load_pd(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallyBlendsIndividuals) {
                std::array<Float_64, VectorSizes::W> givenOnBitNotSet{ 1.0, 2.0, 3.0, 4.0 };
                std::array<Float_64, VectorSizes::W> givenOnBitSet{ 5.0, 6.0, 7.0, 8.0 };
                std::array<Float_64, VectorSizes::W> actual{};

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<true, false, false, false>(_mm256_load_pd(givenOnBitNotSet.data()), _mm256_load_pd(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<false, true, false, false>(_mm256_load_pd(givenOnBitNotSet.data()), _mm256_load_pd(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<false, false, true, false>(_mm256_load_pd(givenOnBitNotSet.data()), _mm256_load_pd(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<false, false, false, true>(_mm256_load_pd(givenOnBitNotSet.data()), _mm256_load_pd(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallyBlendsPairs) {
                std::array<Float_64, VectorSizes::W> givenOnBitNotSet{ 1.0, 2.0, 3.0, 4.0 };
                std::array<Float_64, VectorSizes::W> givenOnBitSet{ 5.0, 6.0, 7.0, 8.0 };
                std::array<Float_64, VectorSizes::W> actual{};

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<true, true, false, false>(_mm256_load_pd(givenOnBitNotSet.data()), _mm256_load_pd(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<true, false, true, false>(_mm256_load_pd(givenOnBitNotSet.data()), _mm256_load_pd(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<false, true, false, true>(_mm256_load_pd(givenOnBitNotSet.data()), _mm256_load_pd(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<false, false, true, true>(_mm256_load_pd(givenOnBitNotSet.data()), _mm256_load_pd(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallyBlendsTriplets) {
                std::array<Float_64, VectorSizes::W> givenOnBitNotSet{ 1.0, 2.0, 3.0, 4.0 };
                std::array<Float_64, VectorSizes::W> givenOnBitSet{ 5.0, 6.0, 7.0, 8.0 };
                std::array<Float_64, VectorSizes::W> actual{};

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<false, true, true, true>(_mm256_load_pd(givenOnBitNotSet.data()), _mm256_load_pd(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<true, false, true, true>(_mm256_load_pd(givenOnBitNotSet.data()), _mm256_load_pd(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<true, true, false, true>(_mm256_load_pd(givenOnBitNotSet.data()), _mm256_load_pd(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<true, true, true, false>(_mm256_load_pd(givenOnBitNotSet.data()), _mm256_load_pd(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }
            }
        };
    }
}