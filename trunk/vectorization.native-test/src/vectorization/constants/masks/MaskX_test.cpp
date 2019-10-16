#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(MaskXTest)
        {
        public:

            TEST_METHOD(returnsPackedFloat2_128Mask) {
                auto actual = MaskX<PackedFloat2_128>();
                Assert::AreEqual(
                    ASizeT{ 128 },
                    bitcount(
                        swizzle<VectorIndices::X, VectorIndices::X>(actual)
                    )
                );
                Assert::AreEqual(
                    ASizeT{ 0 },
                    bitcount(
                        swizzle<VectorIndices::Y, VectorIndices::Y>(actual)
                    )
                );
            }

            TEST_METHOD(returnsPackedFloat4_128Mask) {
                auto actual = MaskX<PackedFloat4_128>();
                Assert::AreEqual(
                    ASizeT{ 128 },
                    bitcount(
                        swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(actual)
                    )
                );
                Assert::AreEqual(
                    ASizeT{ 0 },
                    bitcount(
                        swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(actual)
                    )
                );
            }

            TEST_METHOD(returnsPackedFloat8_256Mask) {
                auto actual = MaskX<PackedFloat8_256>();
                Assert::AreEqual(
                    ASizeT{ 256 },
                    bitcount(
                        swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(actual)
                    )
                );
                Assert::AreEqual(
                    ASizeT{ 0 },
                    bitcount(
                        swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(actual)
                    )
                );
            }

            TEST_METHOD(returnsPackedFloat4_256Mask) {
                auto actual = MaskX<PackedFloat4_256>();
                Assert::AreEqual(
                    ASizeT{ 256 },
                    bitcount(
                        swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::X, VectorIndices::X>(actual)
                    )
                );
                Assert::AreEqual(
                    ASizeT{ 0 },
                    bitcount(
                        swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(actual)
                    )
                );
            }
        };
    }
}