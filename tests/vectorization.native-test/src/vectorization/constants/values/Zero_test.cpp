#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(ValueZeroTest)
        {
        public:

            TEST_METHOD(returnsBool) {
                Assert::IsFalse(Zero<bool>());
            }

            TEST_METHOD(returnsInt_8) {
                Assert::AreEqual(Int_8{ 0 }, Zero<Int_8>());
            }

            TEST_METHOD(returnsUInt_8) {
                Assert::AreEqual(UInt_8{ 0 }, Zero<UInt_8>());
            }

            TEST_METHOD(returnsInt_16) {
                Assert::AreEqual(Int_16{ 0 }, Zero<Int_16>());
            }

            TEST_METHOD(returnsUInt_16) {
                Assert::AreEqual(UInt_16{ 0 }, Zero<UInt_16>());
            }

            TEST_METHOD(returnsInt_32) {
                Assert::AreEqual(Int_32{ 0 }, Zero<Int_32>());
            }

            TEST_METHOD(returnsUInt_32) {
                Assert::AreEqual(UInt_32{ 0 }, Zero<UInt_32>());
            }

            TEST_METHOD(returnsInt_64) {
                Assert::AreEqual(Int_64{ 0 }, Zero<Int_64>());
            }

            TEST_METHOD(returnsUInt_64) {
                Assert::AreEqual(UInt_64{ 0 }, Zero<UInt_64>());
            }

            TEST_METHOD(returnsFloat_32) {
                Assert::AreEqual(0.0f, Zero<Float_32>());
            }

            TEST_METHOD(returnsFloat_64) {
                Assert::AreEqual(0.0, Zero<Float_64>());
            }

            TEST_METHOD(returnsPackedInts_128) {
                Assert::AreEqual(size_t{ 0 }, bitcount(Zero<PackedInts_128>()));
            }

            TEST_METHOD(returnsPackedFloat2_128) {
                std::array<Float_64, VectorSizes::Y> expected{
                    0.0, 0.0
                };
                std::array<Float_64, VectorSizes::Y> actual{ };
                _mm_store_pd(actual.data(), Zero<PackedFloat2_128>());
                Assert::AreEqual(expected, actual);
            }

            TEST_METHOD(returnsPackedFloat4_128) {
                std::array<Float_32, VectorSizes::W> expected{
                    0.0f, 0.0f, 0.0f, 0.0f
                };
                std::array<Float_32, VectorSizes::W> actual{ };
                _mm_store_ps(actual.data(), Zero<PackedFloat4_128>());
                Assert::AreEqual(expected, actual);
            }

            TEST_METHOD(returnsPackedInts_256) {
                Assert::AreEqual(size_t{ 0 }, bitcount(Zero<PackedInts_256>()));
            }

            TEST_METHOD(returnsPackedFloat4_256) {
                std::array<Float_64, VectorSizes::W> expected{
                    0.0, 0.0, 0.0, 0.0
                };
                std::array<Float_64, VectorSizes::W> actual{ };
                _mm256_store_pd(actual.data(), Zero<PackedFloat4_256>());
                Assert::AreEqual(expected, actual);
            }

            TEST_METHOD(returnsPackedFloat8_256) {
                std::array<Float_32, VectorSizes::X8> expected{
                    0.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 0.0f
                };
                std::array<Float_32, VectorSizes::X8> actual{ };
                _mm256_store_ps(actual.data(), Zero<PackedFloat8_256>());
                Assert::AreEqual(expected, actual);
            }
        };
    }
}