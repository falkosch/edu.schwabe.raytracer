#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(ValueHalfTest)
        {
        public:

            TEST_METHOD(returnsFloat_32) {
                Assert::AreEqual(0.5f, Half<Float_32>());
            }

            TEST_METHOD(returnsFloat_64) {
                Assert::AreEqual(0.5, Half<Float_64>());
            }

            TEST_METHOD(returnsPackedFloat2_128) {
                std::array<Float_64, VectorSizes::Y> expected{
                    0.5, 0.5
                };
                std::array<Float_64, VectorSizes::Y> actual{ };
                _mm_store_pd(actual.data(), Half<PackedFloat2_128>());
                Assert::AreEqual(expected, actual);
            }

            TEST_METHOD(returnsPackedFloat4_128) {
                std::array<Float_32, VectorSizes::W> expected{
                    0.5f, 0.5f, 0.5f, 0.5f
                };
                std::array<Float_32, VectorSizes::W> actual{ };
                _mm_store_ps(actual.data(), Half<PackedFloat4_128>());
                Assert::AreEqual(expected, actual);
            }

            TEST_METHOD(returnsPackedFloat4_256) {
                std::array<Float_64, VectorSizes::W> expected{
                    0.5, 0.5, 0.5, 0.5
                };
                std::array<Float_64, VectorSizes::W> actual{ };
                _mm256_store_pd(actual.data(), Half<PackedFloat4_256>());
                Assert::AreEqual(expected, actual);
            }

            TEST_METHOD(returnsPackedFloat8_256) {
                std::array<Float_32, VectorSizes::X8> expected{
                    0.5f, 0.5f, 0.5f, 0.5f,
                    0.5f, 0.5f, 0.5f, 0.5f
                };
                std::array<Float_32, VectorSizes::X8> actual{ };
                _mm256_store_ps(actual.data(), Half<PackedFloat8_256>());
                Assert::AreEqual(expected, actual);
            }
        };
    }
}