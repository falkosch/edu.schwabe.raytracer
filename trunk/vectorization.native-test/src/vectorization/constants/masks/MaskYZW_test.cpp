#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(MaskYZWTest)
        {
        public:

            TEST_METHOD(returnsPackedFloat4_128Mask) {
                auto actual = MaskYZW<PackedFloat4_128>();
                Assert::AreEqual(
                    ASizeT{ 128 },
                    bitcount(
                        swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(actual)
                    )
                );
                Assert::AreEqual(
                    ASizeT{ 0 },
                    bitcount(
                        swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(actual)
                    )
                );
            }

            TEST_METHOD(returnsPackedFloat8_256Mask) {
                auto actual = MaskYZW<PackedFloat8_256>();
                Assert::AreEqual(
                    ASizeT{ 256 },
                    bitcount(
                        swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(actual)
                    )
                );
                Assert::AreEqual(
                    ASizeT{ 0 },
                    bitcount(
                        swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(actual)
                    )
                );
            }

            TEST_METHOD(returnsPackedFloat4_256Mask) {
                auto actual = MaskYZW<PackedFloat4_256>();
                Assert::AreEqual(
                    ASizeT{ 256 },
                    bitcount(
                        swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(actual)
                    )
                );
                Assert::AreEqual(
                    ASizeT{ 0 },
                    bitcount(
                        swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(actual)
                    )
                );
            }
        };
    }
}