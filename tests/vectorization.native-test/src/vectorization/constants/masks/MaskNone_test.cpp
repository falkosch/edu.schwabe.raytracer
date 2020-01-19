#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(MaskNoneTest)
        {
        public:

            TEST_METHOD(returnsBoolMask) {
                Assert::AreEqual(ASizeT{ 0 }, bitcount(MaskNone<bool>()));
            }

            TEST_METHOD(returnsInt_8Mask) {
                Assert::AreEqual(ASizeT{ 0 }, bitcount(MaskNone<Int_8>()));
            }

            TEST_METHOD(returnsUInt_8Mask) {
                Assert::AreEqual(ASizeT{ 0 }, bitcount(MaskNone<UInt_8>()));
            }

            TEST_METHOD(returnsInt_16Mask) {
                Assert::AreEqual(ASizeT{ 0 }, bitcount(MaskNone<Int_16>()));
            }

            TEST_METHOD(returnsUInt_16Mask) {
                Assert::AreEqual(ASizeT{ 0 }, bitcount(MaskNone<UInt_16>()));
            }

            TEST_METHOD(returnsInt_32Mask) {
                Assert::AreEqual(ASizeT{ 0 }, bitcount(MaskNone<Int_32>()));
            }

            TEST_METHOD(returnsUInt_32Mask) {
                Assert::AreEqual(ASizeT{ 0 }, bitcount(MaskNone<UInt_32>()));
            }

            TEST_METHOD(returnsInt_64Mask) {
                Assert::AreEqual(ASizeT{ 0 }, bitcount(MaskNone<Int_64>()));
            }

            TEST_METHOD(returnsUInt_64Mask) {
                Assert::AreEqual(ASizeT{ 0 }, bitcount(MaskNone<UInt_64>()));
            }

            TEST_METHOD(returnsFloat_32Mask) {
                Assert::AreEqual(ASizeT{ 0 }, bitcount(MaskNone<Float_32>()));
            }

            TEST_METHOD(returnsFloat_64Mask) {
                Assert::AreEqual(ASizeT{ 0 }, bitcount(MaskNone<Float_64>()));
            }

            TEST_METHOD(returnsPackedInts_128Mask) {
                Assert::AreEqual(ASizeT{ 0 }, bitcount(MaskNone<PackedInts_128>()));
            }

            TEST_METHOD(returnsPackedFloat4_128Mask) {
                Assert::AreEqual(ASizeT{ 0 }, bitcount(MaskNone<PackedFloat4_128>()));
            }

            TEST_METHOD(returnsPackedFloat2_128Mask) {
                Assert::AreEqual(ASizeT{ 0 }, bitcount(MaskNone<PackedFloat2_128>()));
            }

            TEST_METHOD(returnsPackedInts_256Mask) {
                Assert::AreEqual(ASizeT{ 0 }, bitcount(MaskNone<PackedInts_256>()));
            }

            TEST_METHOD(returnsPackedFloat8_256Mask) {
                Assert::AreEqual(ASizeT{ 0 }, bitcount(MaskNone<PackedFloat8_256>()));
            }

            TEST_METHOD(returnsPackedFloat4_256Mask) {
                Assert::AreEqual(ASizeT{ 0 }, bitcount(MaskNone<PackedFloat4_256>()));
            }
        };
    }
}