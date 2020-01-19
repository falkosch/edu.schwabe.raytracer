#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(SwizzledBlends256dTest)
        {
        public:

            TEST_METHOD(genericallySwizzleBlendsIdentity) {
                std::array<Float_64, VectorSizes::W> givenOnBitNotSet{ 1.0, 2.0, 3.0, 4.0 };
                auto givenOnBitNotSetPack = _mm256_load_pd(givenOnBitNotSet.data());
                std::array<Float_64, VectorSizes::W> givenOnBitSet{ 5.0, 6.0, 7.0, 8.0 };
                auto givenOnBitSetPack = _mm256_load_pd(givenOnBitSet.data());
                std::array<Float_64, VectorSizes::W> actual{ };

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), swizzledBlend<
                        VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W,
                        false, false, false, false
                    >(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), swizzledBlend<
                        VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W,
                        true, true, true, true
                    >(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallySwizzleBlendsBroadcast) {
                std::array<Float_64, VectorSizes::W> givenOnBitNotSet{ 1.0, 2.0, 3.0, 4.0 };
                auto givenOnBitNotSetPack = _mm256_load_pd(givenOnBitNotSet.data());
                std::array<Float_64, VectorSizes::W> givenOnBitSet{ 5.0, 6.0, 7.0, 8.0 };
                auto givenOnBitSetPack = _mm256_load_pd(givenOnBitSet.data());
                std::array<Float_64, VectorSizes::W> actual{ };

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::X)
                    };
                    _mm256_store_pd(actual.data(), swizzledBlend<
                        VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X,
                        false, false, false, false
                    >(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Y)
                    };
                    _mm256_store_pd(actual.data(), swizzledBlend<
                        VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y,
                        true, true, true, true
                    >(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::Z)
                    };
                    _mm256_store_pd(actual.data(), swizzledBlend<
                        VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z,
                        false, true, false, true
                    >(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::W), givenOnBitNotSet.at(VectorIndices::W), givenOnBitSet.at(VectorIndices::W), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm256_store_pd(actual.data(), swizzledBlend<
                        VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W,
                        true, false, true, false
                    >(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallySwizzleBlendsAny) {
                std::array<Float_64, VectorSizes::W> givenOnBitNotSet{ 1.0, 2.0, 3.0, 4.0 };
                auto givenOnBitNotSetPack = _mm256_load_pd(givenOnBitNotSet.data());
                std::array<Float_64, VectorSizes::W> givenOnBitSet{ 5.0, 6.0, 7.0, 8.0 };
                auto givenOnBitSetPack = _mm256_load_pd(givenOnBitSet.data());
                std::array<Float_64, VectorSizes::W> actual{ };

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::W), givenOnBitSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::Y)
                    };
                    _mm256_store_pd(actual.data(), swizzledBlend<
                        VectorIndices::X, VectorIndices::W, VectorIndices::Z, VectorIndices::Y,
                        false, true, true, false
                    >(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::W), givenOnBitNotSet.at(VectorIndices::Z)
                    };
                    _mm256_store_pd(actual.data(), swizzledBlend<
                        VectorIndices::Y, VectorIndices::X, VectorIndices::W, VectorIndices::Z,
                        true, true, true, false
                    >(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
                }
            }
        };
    }
}