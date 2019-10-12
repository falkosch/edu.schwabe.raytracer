#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(Blend128sTest)
        {
        public:

            TEST_METHOD(genericallyBlendsIdentities) {
                std::array<Float_32, VectorSizes::W> givenOnBitNotSet{ 1.0f, 2.0f, 3.0f, 4.0f };
                std::array<Float_32, VectorSizes::W> givenOnBitSet{ 5.0f, 6.0f, 7.0f, 8.0f };
                std::array<Float_32, VectorSizes::W> actual{};

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), blend<false, false, false, false>(_mm_load_ps(givenOnBitNotSet.data()), _mm_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), blend<true, true, true, true>(_mm_load_ps(givenOnBitNotSet.data()), _mm_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallyBlendsIndividuals) {
                std::array<Float_32, VectorSizes::W> givenOnBitNotSet{ 1.0f, 2.0f, 3.0f, 4.0f };
                std::array<Float_32, VectorSizes::W> givenOnBitSet{ 5.0f, 6.0f, 7.0f, 8.0f };
                std::array<Float_32, VectorSizes::W> actual{};

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), blend<true, false, false, false>(_mm_load_ps(givenOnBitNotSet.data()), _mm_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), blend<false, true, false, false>(_mm_load_ps(givenOnBitNotSet.data()), _mm_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), blend<false, false, true, false>(_mm_load_ps(givenOnBitNotSet.data()), _mm_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), blend<false, false, false, true>(_mm_load_ps(givenOnBitNotSet.data()), _mm_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallyBlendsPairs) {
                std::array<Float_32, VectorSizes::W> givenOnBitNotSet{ 1.0f, 2.0f, 3.0f, 4.0f };
                std::array<Float_32, VectorSizes::W> givenOnBitSet{ 5.0f, 6.0f, 7.0f, 8.0f };
                std::array<Float_32, VectorSizes::W> actual{};

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), blend<true, true, false, false>(_mm_load_ps(givenOnBitNotSet.data()), _mm_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), blend<true, false, true, false>(_mm_load_ps(givenOnBitNotSet.data()), _mm_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), blend<false, true, false, true>(_mm_load_ps(givenOnBitNotSet.data()), _mm_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), blend<false, false, true, true>(_mm_load_ps(givenOnBitNotSet.data()), _mm_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallyBlendsTriplets) {
                std::array<Float_32, VectorSizes::W> givenOnBitNotSet{ 1.0f, 2.0f, 3.0f, 4.0f };
                std::array<Float_32, VectorSizes::W> givenOnBitSet{ 5.0f, 6.0f, 7.0f, 8.0f };
                std::array<Float_32, VectorSizes::W> actual{};

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), blend<false, true, true, true>(_mm_load_ps(givenOnBitNotSet.data()), _mm_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), blend<true, false, true, true>(_mm_load_ps(givenOnBitNotSet.data()), _mm_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), blend<true, true, false, true>(_mm_load_ps(givenOnBitNotSet.data()), _mm_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), blend<true, true, true, false>(_mm_load_ps(givenOnBitNotSet.data()), _mm_load_ps(givenOnBitSet.data())));
                    Assert::AreEqual(expected, actual, L"blend value mismatch", LINE_INFO());
                }
            }
        };
    }
}