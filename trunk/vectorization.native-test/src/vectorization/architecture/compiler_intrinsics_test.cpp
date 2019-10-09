#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(CompilerIntrinsicsTest)
        {
        public:

            TEST_METHOD(generatesMasksForFloat4_32) {
                Assert::AreEqual(
                    0 + 0 + 0 + 0,
                    BLEND_MASK_FLOAT4_32(0, 0, 0, 0),
                    L" 0 != MASK(0,0,0,0)",
                    LINE_INFO()
                );

                Assert::AreEqual(
                    1 + 0 + 4 + 0,
                    BLEND_MASK_FLOAT4_32(1, 0, 1, 0),
                    L" 5 != MASK(1,0,1,0)",
                    LINE_INFO()
                );

                Assert::AreEqual(
                    0 + 2 + 0 + 8,
                    BLEND_MASK_FLOAT4_32(0, 1, 0, 1),
                    L"10 != MASK(0,1,0,1)",
                    LINE_INFO()
                );

                Assert::AreEqual(
                    1 + 2 + 4 + 8,
                    BLEND_MASK_FLOAT4_32(1, 1, 1, 1),
                    L"15 != MASK(1,1,1,1)",
                    LINE_INFO()
                );
            }

            TEST_METHOD(generatesMasksForInt4_32) {
                Assert::AreEqual(
                    0 + 0 + 0 + 0 + 0 + 0 + 0 + 0,
                    BLEND_MASK_INT4_32(0, 0, 0, 0),
                    L"  0 != MASK(0,0,0,0)",
                    LINE_INFO()
                );

                Assert::AreEqual(
                    1 + 2 + 0 + 0 + 16 + 32 + 0 + 0,
                    BLEND_MASK_INT4_32(1, 0, 1, 0),
                    L" 51 != MASK(1,0,1,0)",
                    LINE_INFO()
                );

                Assert::AreEqual(
                    0 + 0 + 4 + 8 + 0 + 0 + 64 + 128,
                    BLEND_MASK_INT4_32(0, 1, 0, 1),
                    L"204 != MASK(0,1,0,1)",
                    LINE_INFO()
                );

                Assert::AreEqual(
                    1 + 2 + 4 + 8 + 16 + 32 + 64 + 128,
                    BLEND_MASK_INT4_32(1, 1, 1, 1),
                    L"255 != MASK(1,1,1,1)",
                    LINE_INFO()
                );
            }

            TEST_METHOD(convertsPackedUInt_32ToFloat32) {
                std::array<UInt_32, VectorSizes::W> given{ 0, 1, 2, 3 };
                std::array<Float_32, VectorSizes::W> expected{ 0.0f, 1.0f, 2.0f, 3.0f };
                std::array<Float_32, VectorSizes::W> actual{};

                auto givenPacked = _mm_load_si128(reinterpret_cast<PackedInts_128 *>(given.data()));
                _mm_store_ps(actual.data(), _mm_cvtepu32_ps(givenPacked));

                Assert::AreEqual(expected, actual, L"conversion mismatch", LINE_INFO());
            }
        };
    }
}