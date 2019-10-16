#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(v_f32_4_ConstantsTest)
        {
        public:

            TEST_METHOD(hasMaskAll) {
                Assert::AreEqual(ASizeT{ 128 }, bitcount(MaskAll<v_f32_4>().components));
            }

            TEST_METHOD(hasMaskNone) {
                Assert::AreEqual(ASizeT{ 0 }, bitcount(MaskNone<v_f32_4>().components));
            }

            TEST_METHOD(hasMaskX) {
                auto actual = MaskX<v_f32_4>().components;
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

            TEST_METHOD(hasMaskXY) {
                auto actual = MaskXY<v_f32_4>().components;
                Assert::AreEqual(
                    ASizeT{ 128 },
                    bitcount(
                        swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::X, VectorIndices::Y>(actual)
                    )
                );
                Assert::AreEqual(
                    ASizeT{ 0 },
                    bitcount(
                        swizzle<VectorIndices::Z, VectorIndices::W, VectorIndices::Z, VectorIndices::W>(actual)
                    )
                );
            }

            TEST_METHOD(hasMaskXYZ) {
                auto actual = MaskXYZ<v_f32_4>().components;
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

            TEST_METHOD(hasMaskXYZW) {
                Assert::AreEqual(ASizeT{ 128 }, bitcount(MaskXYZW<v_f32_4>().components));
            }

            TEST_METHOD(hasMaskY) {
                auto actual = MaskY<v_f32_4>().components;
                Assert::AreEqual(
                    ASizeT{ 128 },
                    bitcount(
                        swizzle<VectorIndices::Y, VectorIndices::Y, VectorIndices::Y, VectorIndices::Y>(actual)
                    )
                );
                Assert::AreEqual(
                    ASizeT{ 0 },
                    bitcount(
                        swizzle<VectorIndices::X, VectorIndices::Z, VectorIndices::W, VectorIndices::W>(actual)
                    )
                );
            }

            TEST_METHOD(hasMaskYZ) {
                auto actual = MaskYZ<v_f32_4>().components;
                Assert::AreEqual(
                    ASizeT{ 128 },
                    bitcount(
                        swizzle<VectorIndices::Y, VectorIndices::Z, VectorIndices::Y, VectorIndices::Z>(actual)
                    )
                );
                Assert::AreEqual(
                    ASizeT{ 0 },
                    bitcount(
                        swizzle<VectorIndices::X, VectorIndices::X, VectorIndices::W, VectorIndices::W>(actual)
                    )
                );
            }

            TEST_METHOD(hasMaskYZW) {
                auto actual = MaskYZW<v_f32_4>().components;
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

            TEST_METHOD(hasMaskZ) {
                auto actual = MaskZ<v_f32_4>().components;
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

            TEST_METHOD(hasMaskZW) {
                auto actual = MaskZW<v_f32_4>().components;
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

            TEST_METHOD(hasMaskW) {
                auto actual = MaskW<v_f32_4>().components;
                Assert::AreEqual(
                    ASizeT{ 128 },
                    bitcount(
                        swizzle<VectorIndices::W, VectorIndices::W, VectorIndices::W, VectorIndices::W>(actual)
                    )
                );
                Assert::AreEqual(
                    ASizeT{ 0 },
                    bitcount(
                        swizzle<VectorIndices::X, VectorIndices::Y, VectorIndices::Z, VectorIndices::Z>(actual)
                    )
                );
            }
        };
    }
}