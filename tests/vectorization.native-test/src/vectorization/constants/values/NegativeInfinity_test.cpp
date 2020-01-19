#include "vectorization_tests.h"

#include <limits>

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(ValueNegativeInfinityTest)
        {
        public:

            TEST_METHOD(returnsFloat_32) {
                Assert::AreEqual(-std::numeric_limits<Float_32>::infinity(), NegativeInfinity<Float_32>());
            }

            TEST_METHOD(returnsFloat_64) {
                Assert::AreEqual(-std::numeric_limits<Float_64>::infinity(), NegativeInfinity<Float_64>());
            }

            TEST_METHOD(returnsPackedFloat2_128) {
                std::array<Float_64, VectorSizes::Y> expected{
                    -std::numeric_limits<Float_64>::infinity(),
                    -std::numeric_limits<Float_64>::infinity()
                };
                std::array<Float_64, VectorSizes::Y> actual{ };
                _mm_store_pd(actual.data(), NegativeInfinity<PackedFloat2_128>());
                Assert::AreEqual(expected, actual);
            }

            TEST_METHOD(returnsPackedFloat4_128) {
                std::array<Float_32, VectorSizes::W> expected{
                    -std::numeric_limits<Float_32>::infinity(),
                    -std::numeric_limits<Float_32>::infinity(),
                    -std::numeric_limits<Float_32>::infinity(),
                    -std::numeric_limits<Float_32>::infinity()
                };
                std::array<Float_32, VectorSizes::W> actual{ };
                _mm_store_ps(actual.data(), NegativeInfinity<PackedFloat4_128>());
                Assert::AreEqual(expected, actual);
            }

            TEST_METHOD(returnsPackedFloat4_256) {
                std::array<Float_64, VectorSizes::W> expected{
                    -std::numeric_limits<Float_64>::infinity(),
                    -std::numeric_limits<Float_64>::infinity(),
                    -std::numeric_limits<Float_64>::infinity(),
                    -std::numeric_limits<Float_64>::infinity()
                };
                std::array<Float_64, VectorSizes::W> actual{ };
                _mm256_store_pd(actual.data(), NegativeInfinity<PackedFloat4_256>());
                Assert::AreEqual(expected, actual);
            }

            TEST_METHOD(returnsPackedFloat8_256) {
                std::array<Float_32, VectorSizes::X8> expected{
                    -std::numeric_limits<Float_32>::infinity(),
                    -std::numeric_limits<Float_32>::infinity(),
                    -std::numeric_limits<Float_32>::infinity(),
                    -std::numeric_limits<Float_32>::infinity(),
                    -std::numeric_limits<Float_32>::infinity(),
                    -std::numeric_limits<Float_32>::infinity(),
                    -std::numeric_limits<Float_32>::infinity(),
                    -std::numeric_limits<Float_32>::infinity()
                };
                std::array<Float_32, VectorSizes::X8> actual{ };
                _mm256_store_ps(actual.data(), NegativeInfinity<PackedFloat8_256>());
                Assert::AreEqual(expected, actual);
            }
        };
    }
}