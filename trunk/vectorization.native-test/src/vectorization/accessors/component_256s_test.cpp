#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(Component256sTest)
        {
        public:

            TEST_METHOD(readsComponent) {
                auto expectedComponents = StandardSample::ofArrayType<Float_32, VectorSizes::X8>();
                auto givenPack = _mm256_load_ps(expectedComponents.data());

                {
                    std::array<Float_32, VectorSizes::X8> actual{
                        component<VectorIndices::X1>(givenPack),
                        component<VectorIndices::X2>(givenPack),
                        component<VectorIndices::X3>(givenPack),
                        component<VectorIndices::X4>(givenPack),
                        component<VectorIndices::X5>(givenPack),
                        component<VectorIndices::X6>(givenPack),
                        component<VectorIndices::X7>(givenPack),
                        component<VectorIndices::X8>(givenPack)
                    };
                    Assert::AreEqual(expectedComponents, actual, L"component value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> actual{
                        component<VectorIndices::X8>(givenPack),
                        component<VectorIndices::X1>(givenPack),
                        component<VectorIndices::X2>(givenPack),
                        component<VectorIndices::X3>(givenPack),
                        component<VectorIndices::X4>(givenPack),
                        component<VectorIndices::X5>(givenPack),
                        component<VectorIndices::X6>(givenPack),
                        component<VectorIndices::X7>(givenPack)
                    };
                    Assert::AreNotEqual(expectedComponents, actual, L"wrong component match", LINE_INFO());
                }
            }

            TEST_METHOD(readsXYZW) {
                auto given = StandardSample::ofArrayType<Float_32, VectorSizes::X8>();
                auto givenPack = _mm256_load_ps(given.data());
                auto expectedComponents = std::vector<Float_32>(given.cbegin(), given.cbegin() + VectorSizes::W);

                {
                    std::vector<Float_32> actual{
                        x(givenPack),
                        y(givenPack),
                        z(givenPack),
                        w(givenPack)
                    };
                    Assert::AreEqual(expectedComponents, actual, L"component value mismatch", LINE_INFO());
                }

                {
                    std::vector<Float_32> actual{
                        w(givenPack),
                        x(givenPack),
                        y(givenPack),
                        z(givenPack)
                    };
                    Assert::AreNotEqual(expectedComponents, actual, L"wrong component match", LINE_INFO());
                }
            }

            TEST_METHOD(readsXi) {
                auto expectedComponents = StandardSample::ofArrayType<Float_32, VectorSizes::X8>();
                auto givenPack = _mm256_load_ps(expectedComponents.data());

                {
                    std::array<Float_32, VectorSizes::X8> actual{
                        x1(givenPack),
                        x2(givenPack),
                        x3(givenPack),
                        x4(givenPack),
                        x5(givenPack),
                        x6(givenPack),
                        x7(givenPack),
                        x8(givenPack)
                    };
                    Assert::AreEqual(expectedComponents, actual, L"component value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_32, VectorSizes::X8> actual{
                        x8(givenPack),
                        x1(givenPack),
                        x2(givenPack),
                        x3(givenPack),
                        x4(givenPack),
                        x5(givenPack),
                        x6(givenPack),
                        x7(givenPack)
                    };
                    Assert::AreNotEqual(expectedComponents, actual, L"wrong component match", LINE_INFO());
                }
            }
        };
    }
}