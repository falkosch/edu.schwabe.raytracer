#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(ValueRadianToDegreeTest)
        {
        public:

            TEST_METHOD(returnsFloat_32) {
                Assert::AreNotEqual(0.0f, RadianToDegree<Float_32>());
            }

            TEST_METHOD(returnsFloat_64) {
                Assert::AreNotEqual(0.0, RadianToDegree<Float_64>());
            }

            TEST_METHOD(returnsPackedFloat2_128) {
                std::array<Float_64, VectorSizes::Y> notExpected{
                    0.0, 0.0
                };
                std::array<Float_64, VectorSizes::Y> actual{ };
                _mm_store_pd(actual.data(), RadianToDegree<PackedFloat2_128>());
                Assert::AreNotEqual(notExpected, actual);
            }

            TEST_METHOD(returnsPackedFloat4_128) {
                std::array<Float_32, VectorSizes::W> notExpected{
                    0.0f, 0.0f, 0.0f, 0.0f
                };
                std::array<Float_32, VectorSizes::W> actual{ };
                _mm_store_ps(actual.data(), RadianToDegree<PackedFloat4_128>());
                Assert::AreNotEqual(notExpected, actual);
            }

            TEST_METHOD(returnsPackedFloat4_256) {
                std::array<Float_64, VectorSizes::W> notExpected{
                    0.0, 0.0, 0.0, 0.0
                };
                std::array<Float_64, VectorSizes::W> actual{ };
                _mm256_store_pd(actual.data(), RadianToDegree<PackedFloat4_256>());
                Assert::AreNotEqual(notExpected, actual);
            }

            TEST_METHOD(returnsPackedFloat8_256) {
                std::array<Float_32, VectorSizes::X8> notExpected{
                    0.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 0.0f
                };
                std::array<Float_32, VectorSizes::X8> actual{ };
                _mm256_store_ps(actual.data(), RadianToDegree<PackedFloat8_256>());
                Assert::AreNotEqual(notExpected, actual);
            }
        };
    }
}