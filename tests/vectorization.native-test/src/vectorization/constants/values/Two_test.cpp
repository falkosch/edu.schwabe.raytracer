#include "vectorization_tests.h"

namespace vectorization
{
    namespace test
    {
        TEST_CLASS(ValueTwoTest)
        {
        public:

            TEST_METHOD(returnsInt_8) {
                Assert::AreEqual(Int_8{ 2 }, Two<Int_8>());
            }

            TEST_METHOD(returnsUInt_8) {
                Assert::AreEqual(UInt_8{ 2 }, Two<UInt_8>());
            }

            TEST_METHOD(returnsInt_16) {
                Assert::AreEqual(Int_16{ 2 }, Two<Int_16>());
            }

            TEST_METHOD(returnsUInt_16) {
                Assert::AreEqual(UInt_16{ 2 }, Two<UInt_16>());
            }

            TEST_METHOD(returnsInt_32) {
                Assert::AreEqual(Int_32{ 2 }, Two<Int_32>());
            }

            TEST_METHOD(returnsUInt_32) {
                Assert::AreEqual(UInt_32{ 2 }, Two<UInt_32>());
            }

            TEST_METHOD(returnsInt_64) {
                Assert::AreEqual(Int_64{ 2 }, Two<Int_64>());
            }

            TEST_METHOD(returnsUInt_64) {
                Assert::AreEqual(UInt_64{ 2 }, Two<UInt_64>());
            }

            TEST_METHOD(returnsFloat_32) {
                Assert::AreEqual(2.0f, Two<Float_32>());
            }

            TEST_METHOD(returnsFloat_64) {
                Assert::AreEqual(2.0, Two<Float_64>());
            }

            TEST_METHOD(returnsPackedFloat2_128) {
                std::array<Float_64, VectorSizes::Y> expected{
                    2.0, 2.0
                };
                std::array<Float_64, VectorSizes::Y> actual{ };
                _mm_store_pd(actual.data(), Two<PackedFloat2_128>());
                Assert::AreEqual(expected, actual);
            }

            TEST_METHOD(returnsPackedFloat4_128) {
                std::array<Float_32, VectorSizes::W> expected{
                    2.0f, 2.0f, 2.0f, 2.0f
                };
                std::array<Float_32, VectorSizes::W> actual{ };
                _mm_store_ps(actual.data(), Two<PackedFloat4_128>());
                Assert::AreEqual(expected, actual);
            }

            TEST_METHOD(returnsPackedFloat4_256) {
                std::array<Float_64, VectorSizes::W> expected{
                    2.0, 2.0, 2.0, 2.0
                };
                std::array<Float_64, VectorSizes::W> actual{ };
                _mm256_store_pd(actual.data(), Two<PackedFloat4_256>());
                Assert::AreEqual(expected, actual);
            }

            TEST_METHOD(returnsPackedFloat8_256) {
                std::array<Float_32, VectorSizes::X8> expected{
                    2.0f, 2.0f, 2.0f, 2.0f,
                    2.0f, 2.0f, 2.0f, 2.0f
                };
                std::array<Float_32, VectorSizes::X8> actual{ };
                _mm256_store_ps(actual.data(), Two<PackedFloat8_256>());
                Assert::AreEqual(expected, actual);
            }
        };
    }
}