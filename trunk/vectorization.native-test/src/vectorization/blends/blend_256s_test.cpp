#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(Blend256sTest)
        {
        public:

            TEST_METHOD(genericallyBlendsIdentities) {
                std::array<Float_32, VectorSizes::X8> givenOnBitNotSet{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f };
                std::array<Float_32, VectorSizes::X8> givenOnBitSet{ 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f };
                std::array<Float_32, VectorSizes::X8> actual{};

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        givenOnBitNotSet.at(VectorIndices::X1), givenOnBitNotSet.at(VectorIndices::X2), givenOnBitNotSet.at(VectorIndices::X3), givenOnBitNotSet.at(VectorIndices::X4),
                        givenOnBitNotSet.at(VectorIndices::X5), givenOnBitNotSet.at(VectorIndices::X6), givenOnBitNotSet.at(VectorIndices::X7), givenOnBitNotSet.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), blend<
                        false, false, false, false,
                        false, false, false, false
                    >(_mm256_load_ps(givenOnBitNotSet.data()), _mm256_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        givenOnBitSet.at(VectorIndices::X1), givenOnBitSet.at(VectorIndices::X2), givenOnBitSet.at(VectorIndices::X3), givenOnBitSet.at(VectorIndices::X4),
                        givenOnBitSet.at(VectorIndices::X5), givenOnBitSet.at(VectorIndices::X6), givenOnBitSet.at(VectorIndices::X7), givenOnBitSet.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), blend<
                        true, true, true, true,
                        true, true, true, true
                    >(_mm256_load_ps(givenOnBitNotSet.data()), _mm256_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallyBlendsIndividuals) {
                std::array<Float_32, VectorSizes::X8> givenOnBitNotSet{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f };
                std::array<Float_32, VectorSizes::X8> givenOnBitSet{ 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f };
                std::array<Float_32, VectorSizes::X8> actual{};

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        givenOnBitSet.at(VectorIndices::X1), givenOnBitNotSet.at(VectorIndices::X2), givenOnBitNotSet.at(VectorIndices::X3), givenOnBitNotSet.at(VectorIndices::X4),
                        givenOnBitNotSet.at(VectorIndices::X5), givenOnBitNotSet.at(VectorIndices::X6), givenOnBitNotSet.at(VectorIndices::X7), givenOnBitNotSet.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), blend<
                        true, false, false, false,
                        false, false, false, false
                    >(_mm256_load_ps(givenOnBitNotSet.data()), _mm256_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        givenOnBitNotSet.at(VectorIndices::X1), givenOnBitSet.at(VectorIndices::X2), givenOnBitNotSet.at(VectorIndices::X3), givenOnBitNotSet.at(VectorIndices::X4),
                        givenOnBitNotSet.at(VectorIndices::X5), givenOnBitNotSet.at(VectorIndices::X6), givenOnBitNotSet.at(VectorIndices::X7), givenOnBitNotSet.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), blend<
                        false, true, false, false,
                        false, false, false, false
                    >(_mm256_load_ps(givenOnBitNotSet.data()), _mm256_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        givenOnBitNotSet.at(VectorIndices::X1), givenOnBitNotSet.at(VectorIndices::X2), givenOnBitSet.at(VectorIndices::X3), givenOnBitNotSet.at(VectorIndices::X4),
                        givenOnBitNotSet.at(VectorIndices::X5), givenOnBitNotSet.at(VectorIndices::X6), givenOnBitNotSet.at(VectorIndices::X7), givenOnBitNotSet.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), blend<
                        false, false, true, false,
                        false, false, false, false
                    >(_mm256_load_ps(givenOnBitNotSet.data()), _mm256_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        givenOnBitNotSet.at(VectorIndices::X1), givenOnBitNotSet.at(VectorIndices::X2), givenOnBitNotSet.at(VectorIndices::X3), givenOnBitSet.at(VectorIndices::X4),
                        givenOnBitNotSet.at(VectorIndices::X5), givenOnBitNotSet.at(VectorIndices::X6), givenOnBitNotSet.at(VectorIndices::X7), givenOnBitNotSet.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), blend<
                        false, false, false, true,
                        false, false, false, false
                    >(_mm256_load_ps(givenOnBitNotSet.data()), _mm256_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        givenOnBitNotSet.at(VectorIndices::X1), givenOnBitNotSet.at(VectorIndices::X2), givenOnBitNotSet.at(VectorIndices::X3), givenOnBitNotSet.at(VectorIndices::X4),
                        givenOnBitSet.at(VectorIndices::X5), givenOnBitNotSet.at(VectorIndices::X6), givenOnBitNotSet.at(VectorIndices::X7), givenOnBitNotSet.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), blend<
                        false, false, false, false,
                        true, false, false, false
                    >(_mm256_load_ps(givenOnBitNotSet.data()), _mm256_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        givenOnBitNotSet.at(VectorIndices::X1), givenOnBitNotSet.at(VectorIndices::X2), givenOnBitNotSet.at(VectorIndices::X3), givenOnBitNotSet.at(VectorIndices::X4),
                        givenOnBitNotSet.at(VectorIndices::X5), givenOnBitSet.at(VectorIndices::X6), givenOnBitNotSet.at(VectorIndices::X7), givenOnBitNotSet.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), blend<
                        false, false, false, false,
                        false, true, false, false
                    >(_mm256_load_ps(givenOnBitNotSet.data()), _mm256_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        givenOnBitNotSet.at(VectorIndices::X1), givenOnBitNotSet.at(VectorIndices::X2), givenOnBitNotSet.at(VectorIndices::X3), givenOnBitNotSet.at(VectorIndices::X4),
                        givenOnBitNotSet.at(VectorIndices::X5), givenOnBitNotSet.at(VectorIndices::X6), givenOnBitSet.at(VectorIndices::X7), givenOnBitNotSet.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), blend<
                        false, false, false, false,
                        false, false, true, false
                    >(_mm256_load_ps(givenOnBitNotSet.data()), _mm256_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        givenOnBitNotSet.at(VectorIndices::X1), givenOnBitNotSet.at(VectorIndices::X2), givenOnBitNotSet.at(VectorIndices::X3), givenOnBitNotSet.at(VectorIndices::X4),
                        givenOnBitNotSet.at(VectorIndices::X5), givenOnBitNotSet.at(VectorIndices::X6), givenOnBitNotSet.at(VectorIndices::X7), givenOnBitSet.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), blend<
                        false, false, false, false,
                        false, false, false, true
                    >(_mm256_load_ps(givenOnBitNotSet.data()), _mm256_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallyBlendsPairs) {
                std::array<Float_32, VectorSizes::X8> givenOnBitNotSet{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f };
                std::array<Float_32, VectorSizes::X8> givenOnBitSet{ 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f };
                std::array<Float_32, VectorSizes::X8> actual{};

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        givenOnBitSet.at(VectorIndices::X1), givenOnBitSet.at(VectorIndices::X2), givenOnBitNotSet.at(VectorIndices::X3), givenOnBitNotSet.at(VectorIndices::X4),
                        givenOnBitSet.at(VectorIndices::X5), givenOnBitNotSet.at(VectorIndices::X6), givenOnBitSet.at(VectorIndices::X7), givenOnBitNotSet.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), blend<
                        true, true, false, false,
                        true, false, true, false
                    >(_mm256_load_ps(givenOnBitNotSet.data()), _mm256_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        givenOnBitNotSet.at(VectorIndices::X1), givenOnBitNotSet.at(VectorIndices::X2), givenOnBitSet.at(VectorIndices::X3), givenOnBitSet.at(VectorIndices::X4),
                        givenOnBitNotSet.at(VectorIndices::X5), givenOnBitSet.at(VectorIndices::X6), givenOnBitNotSet.at(VectorIndices::X7), givenOnBitSet.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), blend<
                        false, false, true, true,
                        false, true, false, true
                    >(_mm256_load_ps(givenOnBitNotSet.data()), _mm256_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        givenOnBitSet.at(VectorIndices::X1), givenOnBitNotSet.at(VectorIndices::X2), givenOnBitNotSet.at(VectorIndices::X3), givenOnBitSet.at(VectorIndices::X4),
                        givenOnBitNotSet.at(VectorIndices::X5), givenOnBitSet.at(VectorIndices::X6), givenOnBitSet.at(VectorIndices::X7), givenOnBitNotSet.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), blend<
                        true, false, false, true,
                        false, true, true, false
                    >(_mm256_load_ps(givenOnBitNotSet.data()), _mm256_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallyBlendsTriplets) {
                std::array<Float_32, VectorSizes::X8> givenOnBitNotSet{ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f };
                std::array<Float_32, VectorSizes::X8> givenOnBitSet{ 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f, 16.0f };
                std::array<Float_32, VectorSizes::X8> actual{};

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        givenOnBitSet.at(VectorIndices::X1), givenOnBitSet.at(VectorIndices::X2), givenOnBitSet.at(VectorIndices::X3), givenOnBitNotSet.at(VectorIndices::X4),
                        givenOnBitSet.at(VectorIndices::X5), givenOnBitNotSet.at(VectorIndices::X6), givenOnBitSet.at(VectorIndices::X7), givenOnBitSet.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), blend<
                        true, true, true, false,
                        true, false, true, true
                    >(_mm256_load_ps(givenOnBitNotSet.data()), _mm256_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> expected{
                        givenOnBitSet.at(VectorIndices::X1), givenOnBitSet.at(VectorIndices::X2), givenOnBitNotSet.at(VectorIndices::X3), givenOnBitSet.at(VectorIndices::X4),
                        givenOnBitNotSet.at(VectorIndices::X5), givenOnBitSet.at(VectorIndices::X6), givenOnBitSet.at(VectorIndices::X7), givenOnBitSet.at(VectorIndices::X8)
                    };
                    _mm256_store_ps(actual.data(), blend<
                        true, true, false, true,
                        false, true, true, true
                    >(_mm256_load_ps(givenOnBitNotSet.data()), _mm256_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }
            }
        };
    }
}