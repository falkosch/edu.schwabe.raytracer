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
                auto givenOnBitNotSetPack = _mm256_load_pd(givenOnBitNotSet.data());
                std::array<Float_64, VectorSizes::W> givenOnBitSet{ 5.0, 6.0, 7.0, 8.0 };
                auto givenOnBitSetPack = _mm256_load_pd(givenOnBitSet.data());
                std::array<Float_64, VectorSizes::W> actual{ };

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<false, false, false, false>(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<true, true, true, true>(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallyBlendsIndividuals) {
                std::array<Float_64, VectorSizes::W> givenOnBitNotSet{ 1.0, 2.0, 3.0, 4.0 };
                auto givenOnBitNotSetPack = _mm256_load_pd(givenOnBitNotSet.data());
                std::array<Float_64, VectorSizes::W> givenOnBitSet{ 5.0, 6.0, 7.0, 8.0 };
                auto givenOnBitSetPack = _mm256_load_pd(givenOnBitSet.data());
                std::array<Float_64, VectorSizes::W> actual{ };

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<true, false, false, false>(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<false, true, false, false>(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<false, false, true, false>(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<false, false, false, true>(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallyBlendsPairs) {
                std::array<Float_64, VectorSizes::W> givenOnBitNotSet{ 1.0, 2.0, 3.0, 4.0 };
                auto givenOnBitNotSetPack = _mm256_load_pd(givenOnBitNotSet.data());
                std::array<Float_64, VectorSizes::W> givenOnBitSet{ 5.0, 6.0, 7.0, 8.0 };
                auto givenOnBitSetPack = _mm256_load_pd(givenOnBitSet.data());
                std::array<Float_64, VectorSizes::W> actual{ };

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<true, true, false, false>(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<true, false, true, false>(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<false, true, false, true>(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<false, false, true, true>(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallyBlendsTriplets) {
                std::array<Float_64, VectorSizes::W> givenOnBitNotSet{ 1.0, 2.0, 3.0, 4.0 };
                auto givenOnBitNotSetPack = _mm256_load_pd(givenOnBitNotSet.data());
                std::array<Float_64, VectorSizes::W> givenOnBitSet{ 5.0, 6.0, 7.0, 8.0 };
                auto givenOnBitSetPack = _mm256_load_pd(givenOnBitSet.data());
                std::array<Float_64, VectorSizes::W> actual{ };

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<false, true, true, true>(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<true, false, true, true>(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<true, true, false, true>(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), blend<true, true, true, false>(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }
            }
        };
    }
}