#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(MaskAllTest)
        {
        public:

            TEST_METHOD(returnsBoolMask) {
                Assert::AreEqual(ASizeT{ 1 }, bitcount(MaskAll<bool>()));
            }

            TEST_METHOD(returnsInt_8Mask) {
                Assert::AreEqual(ASizeT{ 8 }, bitcount(MaskAll<Int_8>()));
            }

            TEST_METHOD(returnsUInt_8Mask) {
                Assert::AreEqual(ASizeT{ 8 }, bitcount(MaskAll<UInt_8>()));
            }

            TEST_METHOD(returnsInt_16Mask) {
                Assert::AreEqual(ASizeT{ 16 }, bitcount(MaskAll<Int_16>()));
            }

            TEST_METHOD(returnsUInt_16Mask) {
                Assert::AreEqual(ASizeT{ 16 }, bitcount(MaskAll<UInt_16>()));
            }

            TEST_METHOD(returnsInt_32Mask) {
                Assert::AreEqual(ASizeT{ 32 }, bitcount(MaskAll<Int_32>()));
            }

            TEST_METHOD(returnsUInt_32Mask) {
                Assert::AreEqual(ASizeT{ 32 }, bitcount(MaskAll<UInt_32>()));
            }

            TEST_METHOD(returnsInt_64Mask) {
                Assert::AreEqual(ASizeT{ 64 }, bitcount(MaskAll<Int_64>()));
            }

            TEST_METHOD(returnsUInt_64Mask) {
                Assert::AreEqual(ASizeT{ 64 }, bitcount(MaskAll<UInt_64>()));
            }

            TEST_METHOD(returnsFloat_32Mask) {
                Assert::AreEqual(ASizeT{ 32 }, bitcount(MaskAll<Float_32>()));
            }

            TEST_METHOD(returnsFloat_64Mask) {
                Assert::AreEqual(ASizeT{ 64 }, bitcount(MaskAll<Float_64>()));
            }

            TEST_METHOD(returnsPackedInts_128Mask) {
                Assert::AreEqual(ASizeT{ 128 }, bitcount(MaskAll<PackedInts_128>()));
            }

            TEST_METHOD(returnsPackedFloat4_128Mask) {
                Assert::AreEqual(ASizeT{ 128 }, bitcount(MaskAll<PackedFloat4_128>()));
            }

            TEST_METHOD(returnsPackedFloat2_128Mask) {
                Assert::AreEqual(ASizeT{ 128 }, bitcount(MaskAll<PackedFloat2_128>()));
            }

            TEST_METHOD(returnsPackedInts_256Mask) {
                Assert::AreEqual(ASizeT{ 256 }, bitcount(MaskAll<PackedInts_256>()));
            }

            TEST_METHOD(returnsPackedFloat8_256Mask) {
                Assert::AreEqual(ASizeT{ 256 }, bitcount(MaskAll<PackedFloat8_256>()));
            }

            TEST_METHOD(returnsPackedFloat4_256Mask) {
                Assert::AreEqual(ASizeT{ 256 }, bitcount(MaskAll<PackedFloat4_256>()));
            }
        };
    }
}