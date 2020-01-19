#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(Component128dTest)
        {
        public:

            TEST_METHOD(readsComponent) {
                auto expectedComponents = StandardSample::ofArrayType<Float_64, VectorSizes::Y>();
                auto givenPack = _mm_load_pd(expectedComponents.data());

                {
                    std::array<Float_64, VectorSizes::Y> actual{
                        component<VectorIndices::X>(givenPack),
                        component<VectorIndices::Y>(givenPack)
                    };
                    Assert::AreEqual(expectedComponents, actual, L"component value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::Y> actual{
                        component<VectorIndices::Y>(givenPack),
                        component<VectorIndices::X>(givenPack)
                    };
                    Assert::AreNotEqual(expectedComponents, actual, L"wrong component match", LINE_INFO());
                }
            }

            TEST_METHOD(readsXY) {
                auto expectedComponents = StandardSample::ofArrayType<Float_64, VectorSizes::Y>();
                auto givenPack = _mm_load_pd(expectedComponents.data());

                {
                    std::array<Float_64, VectorSizes::Y> actual{
                        x(givenPack),
                        y(givenPack)
                    };
                    Assert::AreEqual(expectedComponents, actual, L"component value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::Y> actual{
                        y(givenPack),
                        x(givenPack)
                    };
                    Assert::AreNotEqual(expectedComponents, actual, L"wrong component match", LINE_INFO());
                }
            }

            TEST_METHOD(readsXi) {
                auto expectedComponents = StandardSample::ofArrayType<Float_64, VectorSizes::Y>();
                auto givenPack = _mm_load_pd(expectedComponents.data());

                {
                    std::array<Float_64, VectorSizes::Y> actual{
                        x1(givenPack),
                        x2(givenPack)
                    };
                    Assert::AreEqual(expectedComponents, actual, L"component value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::Y> actual{
                        x2(givenPack),
                        x1(givenPack)
                    };
                    Assert::AreNotEqual(expectedComponents, actual, L"wrong component match", LINE_INFO());
                }
            }
        };
    }
}