#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(MaskWTest)
        {
        public:

            TEST_METHOD(returnsPackedFloat4_128Mask) {
                typedef BoolTypes<Float_32>::Type BT;
                std::array<BT, VectorSizes::W> expected{
                    BT{ }, BT{ }, BT{ }, MaskAll<BT>()
                };

                std::array<BoolTypes<Float_32>::Type, VectorSizes::W> actual{ };
                _mm_store_si128(
                    reinterpret_cast<PackedInts_128 *>(actual.data()),
                    _mm_castps_si128(MaskW<PackedFloat4_128>())
                );

                Assert::AreEqual(expected, actual);
            }

            TEST_METHOD(returnsPackedFloat8_256Mask) {
                typedef BoolTypes<Float_32>::Type BT;
                std::array<BT, VectorSizes::X8> expected{
                    BT{ }, BT{ }, BT{ }, MaskAll<BT>(),
                    BT{ }, BT{ }, BT{ }, MaskAll<BT>()
                };

                std::array<BT, VectorSizes::X8> actual{ };
                _mm256_store_si256(
                    reinterpret_cast<PackedInts_256 *>(actual.data()),
                    _mm256_castps_si256(MaskW<PackedFloat8_256>())
                );

                Assert::AreEqual(expected, actual);
            }

            TEST_METHOD(returnsPackedFloat4_256Mask) {
                typedef BoolTypes<Float_64>::Type BT;
                std::array<BT, VectorSizes::W> expected{
                    BT{ }, BT{ }, BT{ }, MaskAll<BT>()
                };

                std::array<BT, VectorSizes::W> actual{ };
                _mm256_store_si256(
                    reinterpret_cast<PackedInts_256 *>(actual.data()),
                    _mm256_castpd_si256(MaskW<PackedFloat4_256>())
                );

                Assert::AreEqual(expected, actual);
            }
        };
    }
}