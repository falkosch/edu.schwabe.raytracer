#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(SwizzledBlends128sTest)
        {
        public:

            TEST_METHOD(genericallySwizzleBlendsIdentity) {
                std::array<Float_32, VectorSizes::W> givenOnBitNotSet{ 1.0f, 2.0f, 3.0f, 4.0f };
                auto givenOnBitNotSetPack = _mm_load_ps(givenOnBitNotSet.data());
                std::array<Float_32, VectorSizes::W> givenOnBitSet{ 5.0f, 6.0f, 7.0f, 8.0f };
                auto givenOnBitSetPack = _mm_load_ps(givenOnBitSet.data());
                std::array<Float_32, VectorSizes::W> actual{ };

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), swizzledBlend<
                        VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W,
                        false, false, false, false
                    >(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Z), givenOnBitSet.at(VectorIndices::W)
                    };
                    _mm_store_ps(actual.data(), swizzledBlend<
                        VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::W,
                        true, true, true, true
                    >(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallySwizzleBlendsBroadcast) {
                std::array<Float_32, VectorSizes::W> givenOnBitNotSet{ 1.0f, 2.0f, 3.0f, 4.0f };
                auto givenOnBitNotSetPack = _mm_load_ps(givenOnBitNotSet.data());
                std::array<Float_32, VectorSizes::W> givenOnBitSet{ 5.0f, 6.0f, 7.0f, 8.0f };
                auto givenOnBitSetPack = _mm_load_ps(givenOnBitSet.data());
                std::array<Float_32, VectorSizes::W> actual{ };

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::X), givenOnBitNotSet.at(VectorIndices::X)
                    };
                    _mm_store_ps(actual.data(), swizzledBlend<
                        VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X,
                        false, false, false, false
                    >(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::X)
                    };
                    _mm_store_ps(actual.data(), swizzledBlend<
                        VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X,
                        true, true, true, true
                    >(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Y)
                    };
                    _mm_store_ps(actual.data(), swizzledBlend<
                        VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y,
                        false, true, false, true
                    >(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::Y), givenOnBitNotSet.at(VectorIndices::Y)
                    };
                    _mm_store_ps(actual.data(), swizzledBlend<
                        VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y,
                        true, false, true, false
                    >(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallySwizzleBlendsAny) {
                std::array<Float_32, VectorSizes::W> givenOnBitNotSet{ 1.0f, 2.0f, 3.0f, 4.0f };
                auto givenOnBitNotSetPack = _mm_load_ps(givenOnBitNotSet.data());
                std::array<Float_32, VectorSizes::W> givenOnBitSet{ 5.0f, 6.0f, 7.0f, 8.0f };
                auto givenOnBitSetPack = _mm_load_ps(givenOnBitSet.data());
                std::array<Float_32, VectorSizes::W> actual{ };

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitNotSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::W), givenOnBitSet.at(VectorIndices::Z), givenOnBitNotSet.at(VectorIndices::Y)
                    };
                    _mm_store_ps(actual.data(), swizzledBlend<
                        VectorIndices::X, VectorIndices::W, VectorIndices::Z, VectorIndices::Y,
                        false, true, true, false
                    >(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::W> expected{
                        givenOnBitSet.at(VectorIndices::Y), givenOnBitSet.at(VectorIndices::X), givenOnBitSet.at(VectorIndices::W), givenOnBitNotSet.at(VectorIndices::Z)
                    };
                    _mm_store_ps(actual.data(), swizzledBlend<
                        VectorIndices::Y, VectorIndices::X, VectorIndices::W, VectorIndices::Z,
                        true, true, true, false
                    >(givenOnBitNotSetPack, givenOnBitSetPack));
                    Assert::AreEqual(expected, actual, L"swizzle blend value mismatch", LINE_INFO());
                }
            }
        };
    }
}