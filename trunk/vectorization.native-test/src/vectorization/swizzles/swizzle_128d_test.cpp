#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(Swizzle128dTest)
        {
        public:

            TEST_METHOD(genericallySwizzlesIdentityAndItsReverse) {
                auto given = StandardSample::ofArrayType<Float_64, VectorSizes::Y>();
                std::array<Float_64, VectorSizes::Y> actual{ };

                {
                    std::array<Float_64, VectorSizes::Y> expected{
                        given.at(VectorIndices::X), given.at(VectorIndices::Y),
                    };
                    _mm_store_pd(actual.data(), swizzle<VectorIndices::X, VectorIndices::Y>(_mm_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::Y> expected{
                        given.at(VectorIndices::Y), given.at(VectorIndices::X),
                    };
                    _mm_store_pd(actual.data(), swizzle<VectorIndices::Y, VectorIndices::X>(_mm_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(genericallySwizzlesBroadcast) {
                auto given = StandardSample::ofArrayType<Float_64, VectorSizes::Y>();
                std::array<Float_64, VectorSizes::Y> actual{ };

                {
                    std::array<Float_64, VectorSizes::Y> expected{
                        given.at(VectorIndices::X), given.at(VectorIndices::X),
                    };
                    _mm_store_pd(actual.data(), swizzle<VectorIndices::X, VectorIndices::X>(_mm_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::Y> expected{
                        given.at(VectorIndices::Y), given.at(VectorIndices::Y),
                    };
                    _mm_store_pd(actual.data(), swizzle<VectorIndices::Y, VectorIndices::Y>(_mm_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }

            TEST_METHOD(hasShortFormsForSwizzles) {
                auto given = StandardSample::ofArrayType<Float_64, VectorSizes::Y>();
                std::array<Float_64, VectorSizes::Y> actual{ };

                {
                    std::array<Float_64, VectorSizes::Y> expected{
                        given.at(VectorIndices::X), given.at(VectorIndices::X),
                    };
                    _mm_store_pd(actual.data(), xx(_mm_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::Y> expected{
                        given.at(VectorIndices::Y), given.at(VectorIndices::X),
                    };
                    _mm_store_pd(actual.data(), yx(_mm_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }

                {
                    std::array<Float_64, VectorSizes::Y> expected{
                        given.at(VectorIndices::Y), given.at(VectorIndices::Y),
                    };
                    _mm_store_pd(actual.data(), yy(_mm_load_pd(given.data())));
                    Assert::AreEqual(expected, actual, L"swizzle value mismatch", LINE_INFO());
                }
            }
        };
    }
}