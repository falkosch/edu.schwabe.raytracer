#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(MaskZWTest)
        {
        public:

            TEST_METHOD(returnsPackedFloat4_128Mask) {
                auto actual = MaskZW<PackedFloat4_128>();
                Assert::AreEqual(
                    ASizeT{ 128 },
                    bitcount(
                        swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(actual)
                    )
                );
                Assert::AreEqual(
                    ASizeT{ 0 },
                    bitcount(
                        swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(actual)
                    )
                );
            }

            TEST_METHOD(returnsPackedFloat8_256Mask) {
                auto actual = MaskZW<PackedFloat8_256>();
                Assert::AreEqual(
                    ASizeT{ 256 },
                    bitcount(
                        swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(actual)
                    )
                );
                Assert::AreEqual(
                    ASizeT{ 0 },
                    bitcount(
                        swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(actual)
                    )
                );
            }

            TEST_METHOD(returnsPackedFloat4_256Mask) {
                auto actual = MaskZW<PackedFloat4_256>();
                Assert::AreEqual(
                    ASizeT{ 256 },
                    bitcount(
                        swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(actual)
                    )
                );
                Assert::AreEqual(
                    ASizeT{ 0 },
                    bitcount(
                        swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(actual)
                    )
                );
            }
        };
    }
}