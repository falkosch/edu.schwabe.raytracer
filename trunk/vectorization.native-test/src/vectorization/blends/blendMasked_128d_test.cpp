#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(BlendMasked128dTest)
        {
        public:

            TEST_METHOD(variadicBlendsTwoFloat2_64WithFloat2_64BitMask) {
                std::array<Float_64, VectorSizes::Y> givenOnBitNotSet{ 1.0, 2.0 };
                auto givenOnBitNotSetPack = _mm_load_pd(givenOnBitNotSet.data());
                std::array<Float_64, VectorSizes::Y> givenOnBitSet{ 3.0, 4.0 };
                auto givenOnBitSetPack = _mm_load_pd(givenOnBitSet.data());
                std::array<Float_64, VectorSizes::Y> actual{ };

                {
                    std::array<Float_64, VectorSizes::Y> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y)
                    };
                    _mm_store_pd(actual.data(), blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, MaskNone<PackedFloat2_128>()));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::Y> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y)
                    };
                    _mm_store_pd(actual.data(), blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, MaskX<PackedFloat2_128>()));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::Y> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y)
                    };
                    _mm_store_pd(actual.data(), blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, MaskXY<PackedFloat2_128>()));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(variadicBlendsTwoFloat2_64WithBool2_64BitMask) {
                std::array<Float_64, VectorSizes::Y> givenOnBitNotSet{ 1.0, 2.0 };
                auto givenOnBitNotSetPack = _mm_load_pd(givenOnBitNotSet.data());
                std::array<Float_64, VectorSizes::Y> givenOnBitSet{ 3.0, 4.0 };
                auto givenOnBitSetPack = _mm_load_pd(givenOnBitSet.data());
                std::array<Float_64, VectorSizes::Y> actual{ };

                {
                    std::array<Float_64, VectorSizes::Y> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y)
                    };
                    _mm_store_pd(actual.data(), blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, _mm_castpd_si128(MaskNone<PackedFloat2_128>())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::Y> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y)
                    };
                    _mm_store_pd(actual.data(), blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, _mm_castpd_si128(MaskX<PackedFloat2_128>())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::Y> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y)
                    };
                    _mm_store_pd(actual.data(), blendMasked(givenOnBitNotSetPack, givenOnBitSetPack, _mm_castpd_si128(MaskXY<PackedFloat2_128>())));
                    Assert::AreEqual(expected, actual, L"blend masked value mismatch", LINE_INFO());
                }
            }
        };
    }
}