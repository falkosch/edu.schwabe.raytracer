#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(MaskZTest)
        {
        public:

            TEST_METHOD(returnsPackedFloat4_128Mask) {
                auto actual = MaskZ<PackedFloat4_128>();
                Assert::AreEqual(
                    ASizeT{ 128 },
                    bitcount(
                        swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(actual)
                    )
                );
                Assert::AreEqual(
                    ASizeT{ 0 },
                    bitcount(
                        swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(actual)
                    )
                );
            }

            TEST_METHOD(returnsPackedFloat8_256Mask) {
                auto actual = MaskZ<PackedFloat8_256>();
                Assert::AreEqual(
                    ASizeT{ 256 },
                    bitcount(
                        swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(actual)
                    )
                );
                Assert::AreEqual(
                    ASizeT{ 0 },
                    bitcount(
                        swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(actual)
                    )
                );
            }

            TEST_METHOD(returnsPackedFloat4_256Mask) {
                auto actual = MaskZ<PackedFloat4_256>();
                Assert::AreEqual(
                    ASizeT{ 256 },
                    bitcount(
                        swizzle<VectorIndices::Z, VectorIndices::Z, VectorIndices::Z, VectorIndices::Z>(actual)
                    )
                );
                Assert::AreEqual(
                    ASizeT{ 0 },
                    bitcount(
                        swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::W, VectorIndices::W>(actual)
                    )
                );
            }
        };
    }
}