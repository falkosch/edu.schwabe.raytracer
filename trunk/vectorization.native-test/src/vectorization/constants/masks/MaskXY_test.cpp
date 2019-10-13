#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(MaskXYTest)
        {
        public:

            TEST_METHOD(returnsPackedFloat2_128Mask) {
                typedef BoolTypes<Float_64>::Type BT;
                std::array<BT, VectorSizes::Y> expected{
                    MaskAll<BT>(), MaskAll<BT>()
                };

                std::array<BoolTypes<Float_64>::Type, VectorSizes::Y> actual{ };
                _mm_store_si128(
                    reinterpret_cast<PackedInts_128 *>(actual.data()),
                    _mm_castpd_si128(MaskXY<PackedFloat2_128>())
                );

                Assert::AreEqual(expected, actual);
            }

            TEST_METHOD(returnsPackedFloat4_128Mask) {
                typedef BoolTypes<Float_32>::Type BT;
                std::array<BT, VectorSizes::W> expected{
                    MaskAll<BT>(), MaskAll<BT>(), BT{ }, BT{ }
                };

                std::array<BoolTypes<Float_32>::Type, VectorSizes::W> actual{ };
                _mm_store_si128(
                    reinterpret_cast<PackedInts_128 *>(actual.data()),
                    _mm_castps_si128(MaskXY<PackedFloat4_128>())
                );

                Assert::AreEqual(expected, actual);
            }

            TEST_METHOD(returnsPackedFloat8_256Mask) {
                typedef BoolTypes<Float_32>::Type BT;
                std::array<BT, VectorSizes::X8> expected{
                    MaskAll<BT>(), MaskAll<BT>(), BT{ }, BT{ },
                    MaskAll<BT>(), MaskAll<BT>(), BT{ }, BT{ }
                };

                std::array<BT, VectorSizes::X8> actual{ };
                _mm256_store_si256(
                    reinterpret_cast<PackedInts_256 *>(actual.data()),
                    _mm256_castps_si256(MaskXY<PackedFloat8_256>())
                );

                Assert::AreEqual(expected, actual);
            }

            TEST_METHOD(returnsPackedFloat4_256Mask) {
                typedef BoolTypes<Float_64>::Type BT;
                std::array<BT, VectorSizes::W> expected{
                    MaskAll<BT>(), MaskAll<BT>(), BT{ }, BT{ }
                };

                std::array<BT, VectorSizes::W> actual{ };
                _mm256_store_si256(
                    reinterpret_cast<PackedInts_256 *>(actual.data()),
                    _mm256_castpd_si256(MaskXY<PackedFloat4_256>())
                );

                Assert::AreEqual(expected, actual);
            }
        };
    }
}