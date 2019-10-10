#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(BlendMasked128sTest)
        {
        public:

            TEST_METHOD(variadicBlendsTwoFloat4_32WithFloat4_32BitMask) {
                std::array<Float_32, VectorSizes::W> givenOnBitNotSet{ 1.0f, 2.0f, 3.0f, 4.0f };
                std::array<Float_32, VectorSizes::W> givenOnBitSet{ 5.0f, 6.0f, 7.0f, 8.0f };
                std::array<Float_32, VectorSizes::W> actual{};

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), blendMasked(_mm_load_ps(givenOnBitNotSet.data()), _mm_load_ps(givenOnBitSet.data()), MaskNone<PackedFloat4_128>()));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), blendMasked(_mm_load_ps(givenOnBitNotSet.data()), _mm_load_ps(givenOnBitSet.data()), MaskX<PackedFloat4_128>()));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), blendMasked(_mm_load_ps(givenOnBitNotSet.data()), _mm_load_ps(givenOnBitSet.data()), MaskXY<PackedFloat4_128>()));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), blendMasked(_mm_load_ps(givenOnBitNotSet.data()), _mm_load_ps(givenOnBitSet.data()), MaskXYZ<PackedFloat4_128>()));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), blendMasked(_mm_load_ps(givenOnBitNotSet.data()), _mm_load_ps(givenOnBitSet.data()), MaskXYZW<PackedFloat4_128>()));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(variadicBlendsTwoFloat4_32WithBool4_32BitMask) {
                std::array<Float_32, VectorSizes::W> givenOnBitNotSet{ 1.0f, 2.0f, 3.0f, 4.0f };
                std::array<Float_32, VectorSizes::W> givenOnBitSet{ 5.0f, 6.0f, 7.0f, 8.0f };
                std::array<Float_32, VectorSizes::W> actual{};

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), blendMasked(_mm_load_ps(givenOnBitNotSet.data()), _mm_load_ps(givenOnBitSet.data()), _mm_castps_si128(MaskNone<PackedFloat4_128>())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), blendMasked(_mm_load_ps(givenOnBitNotSet.data()), _mm_load_ps(givenOnBitSet.data()), _mm_castps_si128(MaskX<PackedFloat4_128>())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), blendMasked(_mm_load_ps(givenOnBitNotSet.data()), _mm_load_ps(givenOnBitSet.data()), _mm_castps_si128(MaskXY<PackedFloat4_128>())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), blendMasked(_mm_load_ps(givenOnBitNotSet.data()), _mm_load_ps(givenOnBitSet.data()), _mm_castps_si128(MaskXYZ<PackedFloat4_128>())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), blendMasked(_mm_load_ps(givenOnBitNotSet.data()), _mm_load_ps(givenOnBitSet.data()), MaskXYZW<PackedFloat4_128>()));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }
            }
        };
    }
}