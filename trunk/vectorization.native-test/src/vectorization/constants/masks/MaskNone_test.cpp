#include "ValueBits.h"

#include <CppUnitTest.h>
#include <vectorization.h>

#include <bitset>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(MaskNoneTest)
        {
        public:

            template<typename T>
            static void testBitsCount() {
                ValueBits<T> actualValue{ MaskNone<T>() };
                auto actualBits = std::bitset<sizeof(T) * CHAR_BIT>(actualValue.bits);
                Assert::AreEqual(size_t{ 0 }, actualBits.count());
            }

            TEST_METHOD(returnsBoolMask) {
                auto actualBits = std::bitset<sizeof(bool) * CHAR_BIT>(MaskNone<bool>());
                Assert::AreEqual(size_t{ 0 }, actualBits.count());
            }

            TEST_METHOD(returnsInt_8Mask) {
                testBitsCount<Int_8>();
            }

            TEST_METHOD(returnsUInt_8Mask) {
                testBitsCount<UInt_8>();
            }

            TEST_METHOD(returnsInt_16Mask) {
                testBitsCount<Int_16>();
            }

            TEST_METHOD(returnsUInt_16Mask) {
                testBitsCount<UInt_16>();
            }

            TEST_METHOD(returnsInt_32Mask) {
                testBitsCount<Int_32>();
            }

            TEST_METHOD(returnsUInt_32Mask) {
                testBitsCount<UInt_32>();
            }

            TEST_METHOD(returnsInt_64Mask) {
                testBitsCount<Int_64>();
            }

            TEST_METHOD(returnsUInt_64Mask) {
                testBitsCount<UInt_64>();
            }

            TEST_METHOD(returnsFloat_32Mask) {
                testBitsCount<Float_32>();
            }

            TEST_METHOD(returnsFloat_64Mask) {
                testBitsCount<Float_64>();
            }

            TEST_METHOD(returnsPackedInts_128Mask) {
                auto actual = MaskNone<PackedInts_128>();
                Assert::AreEqual(size_t{ 0 }, popcnt128(actual));
            }

            TEST_METHOD(returnsPackedFloat2_128Mask) {
                auto actual = _mm_castpd_si128(MaskNone<PackedFloat2_128>());
                Assert::AreEqual(size_t{ 0 }, popcnt128(actual));
            }

            TEST_METHOD(returnsPackedFloat4_128Mask) {
                auto actual = _mm_castps_si128(MaskNone<PackedFloat4_128>());
                Assert::AreEqual(size_t{ 0 }, popcnt128(actual));
            }

            TEST_METHOD(returnsPackedInts_256Mask) {
                auto actual = MaskNone<PackedInts_256>();
                Assert::AreEqual(size_t{ 0 }, popcnt256(actual));
            }

            TEST_METHOD(returnsPackedFloat4_256Mask) {
                auto actual = _mm256_castpd_si256(MaskNone<PackedFloat4_256>());
                Assert::AreEqual(size_t{ 0 }, popcnt256(actual));
            }

            TEST_METHOD(returnsPackedFloat8_256Mask) {
                auto actual = _mm256_castps_si256(MaskNone<PackedFloat8_256>());
                Assert::AreEqual(size_t{ 0 }, popcnt256(actual));
            }
        };
    }
}