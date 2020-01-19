#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(MaskXYZTest)
        {
        public:

            TEST_METHOD(returnsPackedFloat4_128Mask) {
                auto actual = MaskXYZ<PackedFloat4_128>();
                Assert::AreEqual(
                    ASizeT{ 128 },
                    bitcount(
                        swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::Z>(actual)
                    )
                );
                Assert::AreEqual(
                    ASizeT{ 0 },
                    bitcount(
                        swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(actual)
                    )
                );
            }

            TEST_METHOD(returnsPackedFloat8_256Mask) {
                auto actual = MaskXYZ<PackedFloat8_256>();
                Assert::AreEqual(
                    ASizeT{ 256 },
                    bitcount(
                        swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::Z>(actual)
                    )
                );
                Assert::AreEqual(
                    ASizeT{ 0 },
                    bitcount(
                        swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(actual)
                    )
                );
            }

            TEST_METHOD(returnsPackedFloat4_256Mask) {
                auto actual = MaskXYZ<PackedFloat4_256>();
                Assert::AreEqual(
                    ASizeT{ 256 },
                    bitcount(
                        swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::Z>(actual)
                    )
                );
                Assert::AreEqual(
                    ASizeT{ 0 },
                    bitcount(
                        swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(actual)
                    )
                );
            }
        };
    }
}